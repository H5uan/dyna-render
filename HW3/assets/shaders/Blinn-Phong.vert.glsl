#version 460 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Normal;

out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

void main() {

    fragNormal = mat3(transpose(inverse(u_Model))) * i_Normal;

    fragPos = vec3(u_Model * vec4(i_Pos, 1.0f));

    gl_Position = u_ViewProjection * u_Model * vec4(i_Pos, 1.0f);

}
