#type vertex
#version 460 core

layout (location = 0) in vec3 aPos;


uniform mat4 model;
uniform mat4 projectionView;

void main(){
    gl_Position = projectionView * model * vec4(aPos, 1.0);
}

#type fragment
#version 460 core

layout (location = 0) out vec4 FragColor;

void main(){
    FragColor = vec4(0.8f, 0.8f, 0.8f, 1.f);
}