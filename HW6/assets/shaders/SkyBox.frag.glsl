#version 460 core
#extension GL_ARB_shading_language_include : enable

#include "SkyBox.vert.glsl"

layout(location = 0) out vec4 o_FragColor;
in vec3 TexCoords;

uniform samplerCube skybox;

void main(){
    o_FragColor = texture(skybox, TexCoords);
}