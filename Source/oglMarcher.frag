#version 330 core

in vec3 color;

struct LinearCSGTreeNode {
	int op;
	int shape;
	vec2 _padding;
	vec4 position;
	vec4 dimensions;
	mat4 transform;
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
uniform vec3 u_spec_color;
uniform vec3 u_ambient_color;

uniform int u_max_steps;
uniform float u_max_distance;
uniform float u_epsilon;

uniform float u_shininess;
uniform float u_specular_strength;
uniform int u_fov;

uniform float u_smoothing_factor;

out vec4 fragment_colour;

int MAX_ITERATIONS = u_max_steps;
float THRESHOLD = u_epsilon;
float MAX_DISTANCE = u_max_distance;

// BLINN-PHONG constants
vec3 light_color = u_spec_color;
vec3 ambient_color = u_ambient_color;
float specular_strength = u_specular_strength;
float shininess = u_shininess;

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

float minComponent3(vec3 v) {
	return min(min(v.x, v.y), v.z);
}

// NOTE: from https://michael-crum.com/raymarching/
float opSmoothUnion( float d1, float d2, float k ) {
	float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0);
	return mix( d2, d1, h ) - k*h*(1.0-h);
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

			vec4 hpoint = vec4(point, 1.0);
			hpoint = nodes[i].transform * hpoint;
			hpoint = hpoint / nodes[i].scale;

			if (shape == SHAPE_SPHERE) {
				d = sphere_sdf(hpoint.xyz, nodes[i].dimensions.x);
			} else if (shape == SHAPE_BOX) {
				d = box_sdf(hpoint.xyz, nodes[i].dimensions.xyz);
			} else if (shape == SHAPE_CYL) {
				d = cylinder_sdf(hpoint.xyz, nodes[i].dimensions.x, nodes[i].dimensions.y);
			} else if (shape == SHAPE_PLANE) {
				d = plane_sdf(hpoint.xyz, nodes[i].dimensions.xyz, 1);
			}

			d = d * minComponent3(nodes[i].scale.xyz);

			stack[sp++] = d;

			if (d < min_dist) {
				min_dist = d;
				min_color = nodes[i].color.xyz;
			}

		} else if (op != NO_OP) {
			if (op == OP_UNI) {
				stack[sp - 2] = opSmoothUnion(stack[sp - 1], stack[sp - 2], u_smoothing_factor);
			} else if (op == OP_INT) {
				stack[sp - 2] = max(stack[sp - 1], stack[sp - 2]);
			} else if (op == OP_DIFF) {
				stack[sp - 2] = max(-stack[sp - 1], stack[sp - 2]);
			}
			sp--;
		}
	}

	out_color = min_color;
	return stack[0];
}

vec3 get_normal(vec3 point) {
	float epsilon = 0.0001;
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

mat3 create_look_at_matrix(vec3 camera_pos, vec3 look_at, vec3 up) {
	vec3 forward = normalize(look_at - camera_pos);
	vec3 right = normalize(cross(up, forward));
	vec3 camera_up = cross(forward, right);
	return mat3(right, camera_up, forward);
}

const vec3 up = vec3(0.0, 1.0, 0.0);
const float FOV = 1.0;

void main()
{
	vec2 xy_clip = ((gl_FragCoord.xy * 2.0 - window_dimensions) / window_dimensions.y);
	vec3 ray_direction = create_look_at_matrix(u_camera_position, u_look_at, up) * normalize(vec3(xy_clip.xy, FOV));
	vec3 ray_origin = u_camera_position;

	float total_distance = 0.;

	vec3 colour = u_background_color;
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
			vec3 light_dir = normalize(u_light_position - current_position);
			vec3 view_dir = normalize(-ray_direction);
			vec3 halfway_dir = normalize(light_dir + view_dir);
			vec3 normal = get_normal(current_position);

			float diff = max(dot(normal, light_dir), 0.0);
			float spec = 0.0;
			if (diff > 0.0) {
				spec = pow(max(dot(normal, halfway_dir), 0.0), shininess);
			}

			vec3 ambient = ambient_color * object_color;
			vec3 diffuse = diff * light_color * object_color;
			vec3 specular = specular_strength * spec * light_color;

			colour = ambient + diffuse + specular;

			break;
		}
	}

	fragment_colour = vec4(colour, 1.0);
}
