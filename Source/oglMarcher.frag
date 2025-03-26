#version 330 core

in vec3 color;

struct LinearCSGTreeNode {
	int op;
	int shape;
	vec2 _padding;
	vec4 position;
	vec4 dimensions;
	mat4 rotation;
	vec4 scale;
	vec4 color;
};


layout(std140) uniform CSGBuffer {
	LinearCSGTreeNode nodes[128];
};

uniform float loop_length;
uniform vec2 window_dimensions;

uniform vec3 u_camera_position;
uniform vec3 u_light_position;
uniform vec3 u_look_at; // NOTE: is actually used as look direction
uniform vec3 u_background_color;

uniform int u_max_steps;
uniform float u_max_distance;
uniform float u_epsilon;

out vec4 fragment_colour;

const int u_fov = 60; // TODO: make this configurable?

int MAX_ITERATIONS = u_max_steps;
float THRESHOLD = u_epsilon;
float MAX_DISTANCE = u_max_distance;

// BLINN-PHONG constants
const vec3 light_position = vec3(1.0, 3.0, -1.0);
const vec3 light_color = vec3(1.0, 1.0, 1.0);
const vec3 ambient_color = vec3(0.3, 0.3, 0.3);
//const vec3 object_color = vec3(0.3, 0.7, 0.3);
const float specular_strength = 0.5;
const float shininess = 64.0;

const int OP_UNI = 0;
const int OP_INT = 1;
const int OP_DIFF = 2;
const int OP_MOD = 3;
const int NO_OP = 4;

const int SHAPE_SPHERE = 0;
const int SHAPE_BOX = 1;
const int SHAPE_CYL = 2;
const int SHAPE_PLANE = 3;
const int NO_SHAPE = 4;

// Rodrigues formula for rotation
vec3 rotate_arbitrary_axis(vec3 p, vec3 axis, float angle) {
    float c = cos(angle);
    float s = sin(angle);
    float dot_prod = dot(axis, p);
    vec3 cross_prod = cross(axis, p);
    return p * c + cross_prod * s + axis * (dot_prod * (1.0 - c));
}

// SDF functions inspired by https://iquilezles.org/articles/distfunctions/
float plane_sdf(vec3 point, vec3 orientation, float distance)
{
	return dot(point, normalize(orientation)) + distance;
}

float cylinder_sdf(vec3 point, float height, float radius)
{
	vec2 distance = abs(vec2(length(point.xz), point.y)) - vec2(radius, height);
	return min(max(distance.x, distance.y), 0.0) + length(max(distance, 0.0));
}

float box_sdf(vec3 point, vec3 dimensions)
{
	vec3 q = abs(point) - dimensions;
	return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sphere_sdf(vec3 point, float radius)
{
	return length(point) - radius;
}

float get_distance(vec3 point, out vec3 out_color) {
	float stack[20];
	int sp = 0; // pointer to next empty slot

	float min_dist = MAX_DISTANCE;
	vec3 min_color = vec3(1.0);

	for (int i = 0; i < loop_length; ++i) {
		int shape = nodes[i].shape;
		int op = nodes[i].op;
		if (shape != NO_SHAPE) {
			float d;
			if (shape == SHAPE_SPHERE) {
				stack[sp++] = sphere_sdf(point - nodes[i].position.xyz, nodes[i].dimensions.x);
				d = sphere_sdf(point - nodes[i].position.xyz, nodes[i].dimensions.x);
			} else if (shape == SHAPE_BOX) {
				stack[sp++] = box_sdf(point - nodes[i].position.xyz, nodes[i].dimensions.xyz);
				d = box_sdf(point - nodes[i].position.xyz, nodes[i].dimensions.xyz);
			} else if (shape == SHAPE_CYL) {
				stack[sp++] = cylinder_sdf(point - nodes[i].position.xyz, nodes[i].dimensions.x, nodes[i].dimensions.y);
				d = cylinder_sdf(point - nodes[i].position.xyz, nodes[i].dimensions.x, nodes[i].dimensions.y);
			} else if (shape == SHAPE_PLANE) {
				stack[sp++] = plane_sdf(point - nodes[i].position.xyz, nodes[i].dimensions.xyz, 1);
				d = plane_sdf(point - nodes[i].position.xyz, nodes[i].dimensions.xyz, 1);
			}

			if (d < min_dist) {
				min_dist = d;
				min_color = nodes[i].color.xyz;
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

	out_color = min_color;
	return stack[0];
}

vec3 get_normal(vec3 point) {
	float epsilon = 0.001;
	vec3 dummy;
	return normalize(vec3(
		get_distance(point + vec3(epsilon, 0, 0), dummy) - get_distance(point - vec3(epsilon, 0, 0), dummy),
		get_distance(point + vec3(0, epsilon, 0), dummy) - get_distance(point - vec3(0, epsilon, 0), dummy),
		get_distance(point + vec3(0, 0, epsilon), dummy) - get_distance(point - vec3(0, 0, epsilon), dummy)
	));
}

vec3 get_shadow_scalar(vec3 point, vec3 light_pos) {
	vec3 light_dir = normalize(light_pos - point);
	float distance_to_light = length(light_pos - point);
	float shadow_distance = 0.0;
	vec3 new_point = point + (get_normal(point) * THRESHOLD);
	vec3 dummy;
	for(int i = 0; i < MAX_ITERATIONS * 100; ++i) {
		vec3 shadow_position = new_point + light_dir * shadow_distance;
		float df = get_distance(shadow_position, dummy);
		float distance = df;

        if (distance < THRESHOLD) {
            return vec3(0.4);
        }
        if (distance > distance_to_light) {
            return vec3(1.0);
        }
        shadow_distance += distance;
    }
    return vec3(1.0);
}

mat3 create_look_at_matrix(vec3 camera_pos, vec3 look_direction, vec3 up) {
	vec3 forward = normalize(look_direction);
	vec3 right = normalize(cross(forward, up));
	vec3 camera_up = cross(right, forward);
	return mat3(right, camera_up, forward);
}

void main()
{
	vec2 xy_clip = ((gl_FragCoord.xy * 2.0 - window_dimensions) / window_dimensions.y);
	vec3 ray_direction = normalize(vec3(xy_clip, 1.0 / tan(radians(u_fov * 0.5))));

	vec3 up = vec3(0.0, 1.0, 0.0);
	mat3 viewMat = create_look_at_matrix(u_camera_position, normalize(vec3(u_look_at.xy, 1.0)), up);

	ray_direction = viewMat * ray_direction;

	vec3 ray_origin = u_camera_position;

	float total_distance = 0.;

	vec3 colour = vec3(0.0);
	vec3 object_color;

	for(int i = 0; i < MAX_ITERATIONS; ++i) {
		vec3 current_position = ray_origin + ray_direction * total_distance;
		float distance = get_distance(current_position, object_color);
		total_distance += distance;

		if (total_distance > MAX_DISTANCE) {
			colour = u_background_color;
			break;
		}

		if (distance < THRESHOLD) {
			vec3 normal = get_normal(current_position);
			vec3 light_dir = normalize(u_light_position - current_position);
			vec3 view_dir = normalize(-current_position);
			vec3 halfway_dir = normalize(light_dir + view_dir);

			float diff = max(dot(normal, light_dir), 0.0);
			float spec = pow(max(dot(normal, halfway_dir), 0.0), shininess);

			vec3 ambient = ambient_color;
			vec3 diffuse = diff * light_color;
			vec3 specular = specular_strength * spec * light_color;
			vec3 shadow_scalar = get_shadow_scalar(current_position, u_light_position);

			colour = object_color * (ambient + diffuse) + (specular);
			colour *= shadow_scalar;

			break;
		}
	}

	fragment_colour = vec4(colour, 1.0);
}
