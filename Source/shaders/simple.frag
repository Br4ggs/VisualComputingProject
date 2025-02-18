#version 330 core
out vec4 FragColor;
in vec2 fragPos; // Interpolated position from the vertex shader

void main() {
    float red = fragPos.x * 0.5 + 0.5; // Map from [-1,1] to [0,1] for red
    float blue = 1.0 - red; // Opposite of red for blue
    float brightness = 1.0 - (fragPos.y * 0.5 + 0.5); // Map [-1,1] to [0,1] for white-to-black

    vec3 color = vec3(red, 0.0, blue) * brightness; // Apply brightness factor
    FragColor = vec4(color, 1.0);
}
