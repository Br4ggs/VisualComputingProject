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

uniform vec2 windowResolution;

float radius = 1.9; // radius of our sphere

float sphere_SDF(vec3 point) {
    return length(point) - radius;
}

float get_distance(vec3 point) {
    return sphere_SDF(point);
}

const int MAX_STEPS = 60;
const float THRESHOLD = 0.001;
const float MAX_DISTANCE = 10.0;

void main() {

    // normalize to clipping space
    vec2 uv = (gl_FragCoord.xy * 2. - windowResolution) / windowResolution.y;

    vec3 ray_origin = vec3(0, 0, -3);
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

    pixel_colour = vec3(smoothstep(0.0, MAX_DISTANCE, distance_traveled));

    FragColor = vec4(pixel_colour, 1.0);
}
