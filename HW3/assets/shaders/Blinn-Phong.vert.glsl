#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} vs_out;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

void main()
{
    vs_out.FragPos = aPos;
    vs_out.Normal = aNormal;
    gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0);
}