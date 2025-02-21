#version 330 core

/*
Because we are working in a 2D quad, we want to have some alignment on where
everything is positioned. 

The center of the screen is (0, 0).
The bottom left is (-1, -1).
The top right is (1, 1).

The horizontal axis we can call x.
The vertical axis we can call y.
The z axis increases as we go into the screen.

World origin is at 0,0,0
*/

out vec4 FragColor;

in vec2 TexCoord;

uniform vec2 windowResolution;
uniform sampler2D aTexture;

vec3 sphere_position = vec3(0., 0., .0);
vec3 ray_origin = vec3(0, 0, -3);

float max_color_value = length(sphere_position - ray_origin);

float sphere_SDF(vec3 point, float radius) {
    return length(point) - radius;
}

float get_distance(vec3 point) {
    return sphere_SDF(point - sphere_position, 1.);
}

const int MAX_STEPS = 60;
const float THRESHOLD = 0.01;
float MAX_DISTANCE = max_color_value + 1.0;

void main() {

    // normalize to clipping space
    vec2 uv = (gl_FragCoord.xy * 2. - windowResolution) / windowResolution.y;

    vec3 ray_direction = normalize(vec3(uv, 1));
    vec3 pixel_colour = vec3(0);

    float distance_traveled = 0.;

    for (int i = 0; i < MAX_STEPS; ++i) {
        vec3 point = ray_origin + ray_direction * distance_traveled;
        float distance_to_scene = get_distance(point);
        distance_traveled += distance_to_scene;

        if (distance_to_scene < THRESHOLD || distance_traveled > MAX_DISTANCE) {
            break;
        }
    }

    if (distance_traveled > (MAX_DISTANCE - THRESHOLD)) {
        FragColor = texture(aTexture, TexCoord);
    } else {
        pixel_colour = vec3(smoothstep(2.0, max_color_value, distance_traveled));
        FragColor = vec4(pixel_colour, 1.0);
    }
}
