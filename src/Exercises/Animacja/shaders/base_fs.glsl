#version 410

layout(location=0) out vec4 vFragColor;
in vec3 vertex_color;

void main() {
    vFragColor.rgb = vertex_color;
    vFragColor.a=1.0;
}
