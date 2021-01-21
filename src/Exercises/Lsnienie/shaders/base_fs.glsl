#version 410

in vec2 vertex_uv_coords;
in vec3 vertex_normal_in_vs;
in vec3 vertex_position_in_vs;
layout(location=0) out vec4 vFragColor;
uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D shininess_map;
float M_PI = 3.1415926;

layout(std140) uniform Light {
    vec3 position_in_vs;
    vec3 color;
    vec3 a;
    vec3 ambient;
} light;

layout(std140) uniform Material {
    vec3 Kd;
    uint Kd_map;
    vec3 Ks;
    uint Ks_map;
    float Ns;
    uint Ns_map;
} material;

vec3 specular_color = vec3(1.0, 1.0, 1.0);
float shininess = 500.0;

void main() {
    vFragColor=texture(diffuse_map, vertex_uv_coords);

    vec3 normal = normalize(vertex_normal_in_vs);
    //vec3 light_vector = normalize(light.position_in_vs - vertex_position_in_vs);
    //float light_in = max(0.0,dot(normal, light_vector));

    if (gl_FrontFacing) {
        normal = -normal;
    }

    vec3 light_vector = light.position_in_vs - vertex_position_in_vs;
    float r = length(light_vector);
    light_vector/=r;
    float attenuation = 1.0f/(light.a[0]+light.a[1]*r+light.a[2]*r*r);
    float light_in = max(0.0, dot(normal, light_vector))*attenuation;

    vec4 diffuse_color;

    if (material.Kd_map>0) {
        diffuse_color.a = texture(diffuse_map, vertex_uv_coords).a;
        diffuse_color.rgb = texture(diffuse_map, vertex_uv_coords).rgb*material.Kd;
    } else {
        diffuse_color.a = 1;
        diffuse_color.rgb = material.Kd;
    }

    diffuse_color.rgb/=M_PI;

    vec3 specular_color;
    if (material.Ks_map>0) {
        specular_color = texture(specular_map, vertex_uv_coords).rgb*material.Ks;
    } else {
        specular_color = material.Ks;
    }

    float shininess;
    if (material.Ns_map>0) {
        shininess = texture(shininess_map, vertex_uv_coords).r*material.Ns;
    } else {
        shininess = material.Ns;
    }



    vec3 view_vector = normalize(-vertex_position_in_vs);
    vec3 half_vector = normalize(view_vector + light_vector);

    float specular = ( (shininess+8.0f) / (8.0f*M_PI) ) * pow( max( dot( half_vector, normal ), 0.0f ),shininess);

    vFragColor.a = diffuse_color.a;
    vFragColor.rgb = diffuse_color.rgb*light.ambient.rgb;
    vFragColor.rgb += light_in * diffuse_color.rgb * light.color;

    vFragColor.rgb += light_in*light.color*specular*specular_color;

    //vFragColor.a = diffuse_color.a;
    //vFragColor.rgb = diffuse_color.rgb*light.ambient.rgb;
    //vFragColor.rgb += light_in * diffuse_color.rgb*light.color;


    //
    //vFragColor.rgb = abs(material.Kd);
}
