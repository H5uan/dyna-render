#type vertex

#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0);
}

#type fragment

#version 460 core

void main()
{
    // Only need z buffer
}