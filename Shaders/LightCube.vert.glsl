#version 460 core

layout(location = 0) in vec3 i_Pos;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main(){
    gl_Position = u_ViewProjection * u_Model * vec4(i_Pos, 1.0);
}