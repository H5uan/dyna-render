#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexcoords;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main(){
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    TexCoords = aTexcoords;
    FragPos = vec3(u_Model * vec4(aPos, 1.0));
    gl_Position = u_ViewProjection * u_Model * vec4(FragPos, 1.0);
}
