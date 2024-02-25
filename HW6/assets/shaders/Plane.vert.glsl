#version 460 core
layout(location = 0) in vec3 i_Pos;

//out vec2 TexCoords;

//uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main(){
    gl_Position = u_ViewProjection * vec4(i_Pos, 1.0);
    //TexCoords = i_TexCoords;
}
