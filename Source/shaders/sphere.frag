#version 330 core

out vec4 FragColor;
uniform vec2 iResolution; // Screen resolution
uniform float iTime;      // Time (not used here)

// Signed Distance Function (SDF) for a sphere
float sdfSphere(vec3 p, float r) {
    return length(p) - r;
}

// Ray marching function
float rayMarch(vec3 ro, vec3 rd) {
    float t = 0.0;
    const int MAX_STEPS = 100;
    const float EPSILON = 0.001;

    for (int i = 0; i < MAX_STEPS; i++) {
        vec3 p = ro + t * rd;
        float d = sdfSphere(p, 0.5); // Sphere with radius 0.5

        if (d < EPSILON) break; // Stop if close enough
        t += d;

        if (t > 10.0) return -1.0; // No hit
    }
    return t;
}

void main() {
    // Convert fragment coordinates to normalized device coordinates (NDC) in range [-1, 1]
    vec2 uv = (gl_FragCoord.xy / iResolution) * 2.0 - 1.0;
    uv.x *= iResolution.x / iResolution.y; // Maintain aspect ratio

    // Define ray origin and direction
    vec3 ro = vec3(0.0, 0.0, -2.0); // Camera position
    vec3 rd = normalize(vec3(uv, 1.0)); // Forward direction

    // Perform ray marching
    float t = rayMarch(ro, rd);

    if (t > 0.0) {
        // Simple shading based on distance (closer = brighter)
        float shade = 1.0 - (t / 2.0);
        FragColor = vec4(vec3(shade), 1.0);
    } else {
        // Background color (dark gray)
        FragColor = vec4(vec3(0.07), 1.0);
    }
}
