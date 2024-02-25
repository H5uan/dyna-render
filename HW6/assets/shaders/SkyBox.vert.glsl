#version 460 core

layout(location = 0) in vec3 i_Pos;

out vec3 TexCoords;

uniform mat4 u_ViewProjection;

void main(){
    TexCoords = i_Pos;
    vec4 pos = u_ViewProjection * vec4(i_Pos, 1.0);
    gl_Position = pos.xyww; // make sure the sky box's depth(z value) always the farest in NDC

}