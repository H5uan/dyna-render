#version 460 core

in vec3 fragNormal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

uniform vec3 u_ViewPos;

uniform vec3 ObjectColor;

// Material properties
float ambientStrength = 0.1;
float specularStrength = 0.5;
int shininess = 32;// Shininess factor for specular highlight

void main() {
    // ambient
    vec3 ambient = ambientStrength * u_LightColor;

    // diffuse
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(u_LightPos- fragPos); // point to light
    float lightDistance = length(u_LightPos - fragPos);
    vec3 diffuse = 
}
