#version 460 core

layout(location = 0) out vec4 o_FragColor;
in vec3 TexCoords;

uniform samplerCube skybox;

void main(){
    o_FragColor = texture(skybox, TexCoords);
}