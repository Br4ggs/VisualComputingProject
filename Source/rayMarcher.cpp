#include "header/rayMarcher.h"

RayMarcher::RayMarcher(unsigned int width, unsigned int height)
	:width(width), height(height)
{
	data = new unsigned char[width * height * 3];
}

RayMarcher::~RayMarcher()
{
	delete[] data;
	data = nullptr;
}

void RayMarcher::render()
{
    glm::vec2 resolution(width, height);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //normalize screen coordinates
            glm::vec2 fragCoord(j, i);
            glm::vec2 uv = (2.0f * fragCoord - resolution) / resolution.y;

            //direction vectors
            glm::vec3 rayOrigin(-0.75, 0.0, -0.75);
            glm::vec3 lookat(0.75, 1.0, 0.75);
            glm::vec3 rayDirection = getCamera(rayOrigin, lookat) * glm::normalize(glm::vec3(uv.x, uv.y, FOV));

            //do rendering
            glm::vec3 background(0.5, 0.8, 0.9);
            glm::vec3 color(0, 0, 0);
            glm::vec2 object = rayMarch(rayOrigin, rayDirection);
            if (object.x < maxDist)
            {
                //render object using corresponding material
                glm::vec3 p = rayOrigin + object.x * rayDirection;
                glm::vec3 material = getMaterial(p, object.y);
                color += getLight(p, rayDirection, material);
                //add fog to mitigate ugly aliasing effects
                color = glm::mix(color, background, 1.0 - exp(-0.0008 * object.x * object.x));
            }
            else
            {
                //render background
                color += background - glm::max(0.95f * rayDirection.y, 0.0f);
            }

            //perform gamma correction
            color = glm::pow(color, glm::vec3(0.4545));

            //convert float to 8-bit color data for each separate channel
            unsigned int R = floor(color.x >= 1.0 ? 255 : color.x * 256.0);
            unsigned int G = floor(color.y >= 1.0 ? 255 : color.y * 256.0);
            unsigned int B = floor(color.z >= 1.0 ? 255 : color.z * 256.0);

            //set color
            data[((i * 1000) + j) * 3]     = R;
            data[((i * 1000) + j) * 3 + 1] = G;
            data[((i * 1000) + j) * 3 + 2] = B;
        }
    }
}

unsigned char* RayMarcher::getRenderData() const
{
    return &data[0];
}

glm::mat3 RayMarcher::getCamera(glm::vec3 rayOrigin, glm::vec3 lookAt) const
{
    glm::vec3 camForward = glm::normalize(lookAt - rayOrigin);
    glm::vec3 camRight = glm::normalize(glm::cross(glm::vec3(0, 1, 0), camForward));
    glm::vec3 camUp = glm::cross(camForward, camRight);

    return glm::mat3(camRight, camUp, camForward);
}

glm::vec3 RayMarcher::getMaterial(glm::vec3 point, float id) const
{
    glm::vec3 m(0.0);
    switch (int(id))
    {
    case 1:
        m = glm::vec3(0.9, 0.9, 0.0);
        break;
    case 2:
        //creates a checkerboard pattern
        m = glm::vec3(0.2 + 0.4 * glm::mod(floor(point.x) + floor(point.z), 2.0f));
        break;
    }
    return m;
}

glm::vec3 RayMarcher::getLight(glm::vec3 point, glm::vec3 rayDirection, glm::vec3 color) const
{
    glm::vec3 lightPos(0.0, 0.0, 0.0);
    glm::vec3 L = glm::normalize(lightPos - point);
    glm::vec3 N = getNormal(point);
    glm::vec3 V = -rayDirection;
    glm::vec3 R = glm::reflect(-L, N);

    //standard phong light model
    glm::vec3 specColor = glm::vec3(0.5);
    glm::vec3 specular = specColor * pow(glm::clamp(glm::dot(R, V), 0.0f, 1.0f), 10.0f);
    glm::vec3 diffuse = color * glm::clamp(glm::dot(L, N), 0.0f, 1.0f);
    glm::vec3 ambient = color * 0.05f;

    //shadows
    float d = rayMarch(point + N * 0.02f, glm::normalize(lightPos)).x;
    if (d < glm::length(lightPos - point)) return ambient;

    return diffuse + ambient + specular;
}

//returns an approximate of the normal
glm::vec3 RayMarcher::getNormal(glm::vec3 point) const
{
    glm::vec2 e(epsilon, 0.0);

    float comp1 = map(point - glm::vec3(e.x, e.y, e.y)).x;
    float comp2 = map(point - glm::vec3(e.y, e.x, e.y)).x;
    float comp3 = map(point - glm::vec3(e.y, e.y, e.x)).x;
    glm::vec3 n = glm::vec3(map(point).x) - glm::vec3(comp1, comp2, comp3);
    return glm::normalize(n);
}

glm::vec2 RayMarcher::rayMarch(glm::vec3 rayOrigin, glm::vec3 rayDirection) const
{
    glm::vec2 hit;
    glm::vec2 object(0, 0); //?

    for (int i = 0; i < maxSteps; i++)
    {
        glm::vec3 p = rayOrigin + object.x * rayDirection;
        hit = map(p);
        object.x += hit.x;
        object.y = hit.y;

        if (abs(hit.x) < epsilon || object.x > maxDist) break;
    }
    return object;
}

//TODO: move this stuff to a "scene" class
glm::vec2 RayMarcher::map(glm::vec3 point) const
{
    //mod3(&point, glm::vec3(5, 5, 5));
    point = repeat(point, glm::vec3(2, 2, 2));

    //a plane
    //float planeDist = sdfPlane(point, glm::vec3(0, 1, 0), 3.0);
    //float planeID = 2.0;
    //glm::vec2 plane = glm::vec2(planeDist, planeID);

    //a single sphere
    float sphereDist = sdfSphere(point, 1.3);
    float sphereID = 1.0;
    glm::vec2 sphere = glm::vec2(sphereDist, sphereID);

    //a box
    float boxDist = sdfBox(point, glm::vec3(1, 1, 1));
    float boxID = 1.0;
    glm::vec2 box(boxDist, boxID);

    //a cylinder, placed higher than the rest of the objects
    //glm::vec3 pc = point;
    //pc.y -= 4.0;
    //float cylinderDist = sdfCylinder(glm::vec3(pc.y, pc.x, pc.z), 1, 2); //rotate the cylinder by swapping the components around
    //float cylinderID = 1.0;
    //glm::vec2 cylinder(cylinderDist, cylinderID);

    glm::vec2 res = box;
    res = sdfDifference(res, sphere);
    //res = sdfUnion(res, cylinder);
    //res = sdfUnion(res, plane);
    return res;
}

float RayMarcher::sdfCylinder(glm::vec3 point, float radius, float height) const
{
    float d = glm::length(glm::vec2(point.x, point.z)) - radius;
    d = glm::max(d, abs(point.y) - height);
    return d;
}

float RayMarcher::sdfBox(glm::vec3 point, glm::vec3 dim) const
{
    glm::vec3 d = glm::abs(point) - dim;
    return glm::length(glm::max(d, glm::vec3(0))) + vmax(glm::min(d, glm::vec3(0)));
}

float RayMarcher::sdfSphere(glm::vec3 point, float radius) const
{
    return glm::length(point) - radius;
}

float RayMarcher::sdfPlane(glm::vec3 point, glm::vec3 normal, float distanceFromOrigin) const
{
    return glm::dot(point, normal) + distanceFromOrigin;
}

glm::vec2 RayMarcher::sdfUnion(glm::vec2 obj1, glm::vec2 obj2) const
{
    return (obj1.x < obj2.x) ? obj1 : obj2;
}

glm::vec2 RayMarcher::sdfIntersect(glm::vec2 obj1, glm::vec2 obj2) const
{
    return (obj1.x > obj2.x) ? obj1 : obj2;
}

//max(obj1, -obj2)
glm::vec2 RayMarcher::sdfDifference(glm::vec2 obj1, glm::vec2 obj2) const
{
    if (obj1.x > -obj2.x)
    {
        return obj1;
    }
    else
    {
        obj2.x *= -1;
        return obj2;
    }
}

//selects the largest component of the vector
float RayMarcher::vmax(glm::vec3 v) const
{
    return glm::max(glm::max(v.x, v.y), v.z);
}

glm::vec3 RayMarcher::repeat(glm::vec3 point, glm::vec3 scale) const
{
    glm::vec3 q = point - scale * glm::round(point / scale);
    return q;
}