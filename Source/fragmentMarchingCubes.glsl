// Example: replicate sphere tracer lighting
#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec3 Color;  // your per-vertex color

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor; // same as "specColor" in sphere tracer?

void main()
{
	vec3 N = normalize(Normal);
	vec3 L = normalize(lightPos - FragPos);
	vec3 V = normalize(viewPos - FragPos);
	vec3 R = reflect(-L, N);

	// same numbers as sphere tracer:
	float ambientStrength = 0.05;
	float shininess       = 10.0;

	vec3 ambient = ambientStrength * Color;
	vec3 diffuse = Color * max(dot(N, L), 0.0);
	vec3 specular = lightColor * pow(max(dot(R, V), 0.0), shininess);

	// sum them up
	vec3 finalColor = ambient + diffuse + specular;

	// gamma correct if you want an exact match
	finalColor = pow(finalColor, vec3(0.4545));

	FragColor = vec4(finalColor, 1.0);
}
