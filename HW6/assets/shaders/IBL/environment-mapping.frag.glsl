#version 460 core

layout(location = 0) out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} fs_in;

uniform vec3 u_CameraPos;
uniform samplerCube u_Skybox;

void main(){
    vec3 viewDir = normalize(fs_in.FragPos - u_CameraPos);

    vec3 reflectDir = reflect(viewDir, normalize(fs_in.Normal));

    vec3 envColor = texture(u_Skybox, reflectDir).rgb;

    FragColor = vec4(envColor, 1.0);
}
