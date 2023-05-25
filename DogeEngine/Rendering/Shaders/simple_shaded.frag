#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

layout(location = 0) out vec4 FragColor;

layout(location = 5) uniform vec3 light_pos;

layout(location = 6) uniform vec3 color;

void main() {


    vec3 lightColor = vec3(1,1,1);
    vec3 ambient = vec3(0.1,0.1,0.1);
    vec3 object_color = color;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light_pos - pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 result = (ambient + diffuse) * object_color;
    FragColor = vec4(result, 1.0);
}