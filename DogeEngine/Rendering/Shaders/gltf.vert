#version 450

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_tex_coord;

layout(location = 0) out vec3 pos;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec2 tex_coord;

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(vertex_pos, 1.0f);
    normal = mat3(transpose(inverse(model))) * vertex_normal;
    tex_coord = vertex_tex_coord;
    pos = vec3(model * vec4(vertex_pos, 1.0)); //Convert vertex pos to world space for lighting calculations. So that light depends on where the object is and how it is rotated.
}