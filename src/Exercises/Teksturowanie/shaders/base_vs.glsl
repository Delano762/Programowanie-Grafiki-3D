#version 410

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in  vec2 a_vertex_uv_coords;

layout(std140) uniform Transformations {
    mat4 P;
};

out vec2 vertex_uv_coords;

void main() {
    gl_Position = P * a_vertex_position;
    vertex_uv_coords = a_vertex_uv_coords;
}
