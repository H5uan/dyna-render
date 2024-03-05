#version 460

layout(location = 0) out vec4 o_LightColor;

struct PointLight{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight light;

void main(){
    // Need a better way to do it.
    vec3 lightColor = (light.ambient + light.diffuse + light.specular) / 3.0;
    o_LightColor = vec4(lightColor, 1.0);
}