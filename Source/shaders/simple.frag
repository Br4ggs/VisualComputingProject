#version 330 core

out vec4 FragColor;

float sdf_sphere(vec3 position, float radius) {
   return length(position) - radius;
}

void main()
{
   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);
}
