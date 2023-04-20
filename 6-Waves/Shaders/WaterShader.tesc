#version 400

layout (vertices = 4) out;

// Input vertex data, aggregated into patches
in vec2 uv_vs[];
in vec3 normal_vs[];
in vec3 position_vs[];
in vec3 LightDirection_cameraspace[];
in vec3 EyeDirection_cameraspace[];

// Output data : will be passed to TES.
out vec3 position_tesc[];
out vec2 uv_tesc[];
out vec3 normal_tesc[];
out vec3 light_tesc[];
out vec3 eye_tesc[];


// Uniform values that stay constant for the whole mesh.
uniform float outerTess;
uniform float innerTess;

void main() {


    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    position_tesc[gl_InvocationID] = position_vs[gl_InvocationID];

    uv_tesc[gl_InvocationID] = uv_vs[gl_InvocationID];
    light_tesc[gl_InvocationID] = LightDirection_cameraspace[gl_InvocationID];
    eye_tesc[gl_InvocationID] = EyeDirection_cameraspace[gl_InvocationID];

    gl_TessLevelOuter[0] = outerTess;
    gl_TessLevelOuter[1] = outerTess;
    gl_TessLevelOuter[2] = outerTess;
    gl_TessLevelOuter[3] = outerTess;

    gl_TessLevelInner[0] = innerTess;
    gl_TessLevelInner[1] = innerTess;

}
