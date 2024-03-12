#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main() {
    vec3 fragPosWorld = vec3(u_Model * vec4(aPos, 1.0));
    vs_out.FragPos = fragPosWorld;
    vs_out.Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    gl_Position = u_ViewProjection * vec4(fragPosWorld, 1.0);
}