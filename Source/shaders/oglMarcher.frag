#version 330 core

in vec3 color;

struct LinearCSGTreeNode {
	int op;
	int shape;
	vec2 _padding;
	vec4 position;
	vec4 dimensions;
};


layout(std140) uniform CSGBuffer {
	LinearCSGTreeNode nodes[128];
};


uniform float loop_length;
uniform vec2 window_dimensions;

out vec4 fragment_colour;

const int MAX_ITERATIONS = 40;
const float THRESHOLD = 0.001;
const float MAX_DISTANCE = 50.0;
const float floor_level = -2.0;

// BLINN-PHONG constants
const vec3 light_position = vec3(1.0, 3.0, -1.0);
const vec3 light_color = vec3(1.0, 1.0, 1.0);
const vec3 ambient_color = vec3(0.3, 0.3, 0.3);
const vec3 object_color = vec3(0.3, 0.7, 0.3);
const float specular_strength = 0.5;
const float shininess = 64.0;

const int OP_UNI = 0;
const int OP_INT = 1;
const int OP_SUB = 2;
const int NO_OP = 3;

const int SHAPE_SPHERE = 0;
const int SHAPE_BOX = 1;
const int NO_SHAPE = 2;

float box_sdf(vec3 point, vec3 dimensions)
{
	vec3 q = abs(point) - dimensions;
	return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sphere_sdf(vec3 point, float radius)
{
	return length(point) - radius;
}

float get_distance(vec3 point) {
	float stack[20];
	int sp = 0; // pointer to next empty slot
	for (int i = 0; i < loop_length; ++i) {
		int shape = nodes[i].shape;
		int op = nodes[i].op;
		if (shape != NO_SHAPE) {
			if (shape == SHAPE_SPHERE) {
				stack[sp++] = sphere_sdf(point - nodes[i].position.xyz, nodes[i].dimensions.x);
			} else if (shape == SHAPE_BOX) {
				stack[sp++] = box_sdf(point - nodes[i].position.xyz, nodes[i].dimensions.xyz);
			}
		} else if (op != NO_OP) {
			if (op == OP_UNI) {
				stack[sp - 2] = min(stack[sp - 1], stack[sp - 2]);
			} else if (op == OP_INT) {
				stack[sp - 2] = max(stack[sp - 1], stack[sp - 2]);
			}
			sp--;
		}
	}
	return stack[0];
}

void main()
{

	vec2 xy_clip = ((gl_FragCoord.xy * 2 - window_dimensions) / window_dimensions.y) * 1.0;
	vec3 ray_direction = normalize(vec3(xy_clip, 1.0));
	vec3 ray_origin = vec3(0.0, 0.0, -4.0);

	float total_distance = 0.;
	vec3 colour = vec3(0.);

	for(int i = 0; i < MAX_ITERATIONS; ++i) {
		vec3 current_position = ray_origin + ray_direction * total_distance;
		float distance = get_distance(current_position);
		total_distance += distance;

		if (total_distance > MAX_DISTANCE) {
			colour = vec3(0.0);
			break;
		}

		if (distance < THRESHOLD) {
			colour = vec3(1.0);
			break;
		}
	}

	fragment_colour = vec4(colour, 1.0);
}

