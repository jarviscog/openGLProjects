#version 400

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

// Input vertex data, aggregated into triangles
in vec3 position_tese[];
in vec3 normal_tese[];
in vec3 eye_tese[];
in vec3 light_tese[];
in vec2 uv_tese[];

// Output data per vertex, passed to primitive assembly and rasterization
out vec3 normal_gs;
out vec3 eye_gs;
out vec3 light_gs;
out vec2 uv_gs;

// Uniform values that stay constant for the whole mesh.

uniform mat4 V;
uniform mat4 MVP;
uniform sampler2D displacementTexture;
uniform float time;

vec3 GetNormal(vec4 a, vec4 b, vec4 c) {
	vec3 x = b.xyz - a.xyz;
	vec3 y = c.xyz - b.xyz;
	return normalize(cross(x, y));
}


vec3 Gerstner(vec3 worldpos, float w, float A, float phi, float Q, vec2 D, int N) {
	// w = wave spread. Higher is more dense
	// A = amplitude
	// phi = speed
	vec3 pos;
	float Qact = Q / (w * A * N);
	pos.x = Qact * A * D.x * cos(dot((w * D), worldpos.xz) + phi * time);
	pos.y = A * sin(dot( w*D, worldpos.xz) + phi * time);
	pos.z = Qact * A * D.y * cos(dot( w * D, worldpos.xz) + phi * time);
	return pos;
}

void main() {
	vec4 pos[3];
	float dispStrength = 0.15;


	for(int i = 0; i < gl_in.length(); ++i) {

		eye_gs = eye_tese[i];
		light_gs = light_tese[i];
		uv_gs = uv_tese[i];


		pos[i] = vec4(position_tese[i], 1.0);

		pos[i].y += texture(displacementTexture, uv_tese[i]).r * dispStrength;

		pos[i] += vec4(Gerstner(position_tese[i], 2.0f, 0.10f, 1.1f, 0.75, vec2(0.3 , 0.6), 2), 5.0);
		pos[i] += vec4(Gerstner(position_tese[i], 2.0f, 0.5, 0.5f, 0.75, vec2(0.0, .866) , 2) , 5.0);

	}


	vec3 mynorm = GetNormal(pos[0], pos[1], pos[2]);
	normal_gs = ( V * vec4(mynorm, 0)).xyz;

	for(int i = 0; i < gl_in.length(); ++i) {

		uv_gs = uv_tese[i];
		gl_Position = MVP * pos[i];
		EmitVertex();
	}
	EndPrimitive();


}


