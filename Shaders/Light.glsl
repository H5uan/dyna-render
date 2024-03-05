#type vertex
#version 460 core

layout(location = 0) in vec3 i_Pos;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main(){
    gl_Position = u_ViewProjection * u_Model * vec4(i_Pos, 1.0);
}

#type fragment
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