#version 460 core

layout (location = 0) in vec3 i_Pos;
layout (location = 1) in vec3 i_Normal;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} vs_out;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main(){
    vs_out.Normal = mat3(transpose(inverse(u_Model))) * i_Normal;
    vs_out.FragPos = vec3(u_Model * vec4(i_Pos, 1.0));
    gl_Position = u_ViewProjection * vec4(vs_out.FragPos, 1.0);
}
