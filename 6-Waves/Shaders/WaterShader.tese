#version 400

layout (quads, equal_spacing) in;

in vec3 position_tesc[];
in vec2 uv_tesc[];
in vec3 normal_tesc[];
in vec3 light_tesc[];
in vec3 eye_tesc[];

out vec3 position_tese;
out vec2 uv_tese;
out vec3 normal_tese;
out vec3 light_tese;
out vec3 eye_tese;

void main() {

    vec4 p0= gl_in[0].gl_Position;
    vec4 p1= gl_in[1].gl_Position;
    vec4 p2= gl_in[2].gl_Position;
    vec4 p3= gl_in[3].gl_Position;

    vec4 m1 = mix(p0, p1, gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    vec4 m2 = mix(p3, p2, gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    gl_Position = mix(m1, m2, gl_TessCoord.y);

    vec3 c1 = mix(normal_tesc[0], normal_tesc[1], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    vec3 c2 = mix(normal_tesc[3], normal_tesc[2], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    normal_tese = mix(c1, c2, gl_TessCoord.y);

    c1 = mix(position_tesc[0], position_tesc[1], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    c2 = mix(position_tesc[3], position_tesc[2], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    position_tese = mix(c1, c2, gl_TessCoord.y);

    c1 = mix(eye_tesc[0], eye_tesc[1], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    c2 = mix(eye_tesc[3], eye_tesc[2], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    eye_tese = mix(c1, c2, gl_TessCoord.y);

    c1 = mix(light_tesc[0], light_tesc[1], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    c2 = mix(light_tesc[3], light_tesc[2], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    light_tese = mix(c1, c2, gl_TessCoord.y);

    vec2 u1 = mix(uv_tesc[0], uv_tesc[1], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    vec2 u2 = mix(uv_tesc[3], uv_tesc[2], gl_TessCoord.x);//may have to rearrange these numbers depending on your implementation
    uv_tese = mix(u1, u2, gl_TessCoord.y);

}