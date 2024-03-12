#version 460 core

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoord;

struct VertexOut{
    vec3 WorldPos;
    vec3 Normal;
    vec2 TexCoord;
};

layout(location = 0) out VertexOut vs_output;

uniform mat4 u_ModelViewProjection;

void main(){
    vs_output.TexCoord = TexCoord;
    vs_output.WorldPos = vec3(model * vec4(Pos, 1.0));

    gl_Position = u_ModelViewProjection * vec4(vs_output.WorldPos, 1.0);
}