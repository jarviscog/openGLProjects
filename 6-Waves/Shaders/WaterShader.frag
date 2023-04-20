#version 400

// Interpolated values from the vertex shaders
in vec3 normal_gs;
in vec3 position_gs;
in vec3 eye_gs;
in vec3 light_gs;
in vec2 uv_gs;

// Ouput data
out vec4 color_out;

uniform vec4 modelcolor;
uniform float alpha;
uniform sampler2D waterTexture;

void phongColor() {

    // Material properties
    vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    vec4 diffuse = texture(waterTexture, uv_gs);
    vec4 ambient = 0.8 * diffuse;
    vec4 specular = vec4(0.4, 0.4, 0.4, modelcolor.a);

    vec3 n = normalize(normal_gs);
    vec3 l = normalize(light_gs);

    float cosTheta = clamp(dot(n, l), 0, 1);

    vec3 E = normalize(eye_gs);
    vec3 R = reflect(-l, n);

    float cosAlpha = clamp(dot(E, R), 0, 1);


    color_out =
    ambient +
    diffuse * lightColor * cosTheta +
    specular * lightColor * pow(cosAlpha, alpha);

}

void main() {
    phongColor();
}

