#version 400

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

// Output data will be interpolated for each fragment.

//out vec3 vertexPosition_vs;
out vec3 normal_vs;
out vec3 LightDirection_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec2 uv_vs;
out vec3 position_vs;

// Values that stay constant for the whole mesh.
uniform mat4 V;
uniform vec3 lightPosition;
uniform vec2 texOffset;
uniform float texScale;
uniform float time;

void main(){

    gl_Position = vec4(vertexPosition_modelspace, 1);

    position_vs = vertexPosition_modelspace;

    normal_vs = vertexNormal_modelspace;


    vec3 vertexPosition_cameraspace = (V * vec4(vertexPosition_modelspace, 1)).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

    vec3 LightPosition_cameraspace = (V * vec4(lightPosition,1)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

    uv_vs = ( vertexPosition_modelspace.xz + texOffset + ( time *0.08) ) / texScale;



}
