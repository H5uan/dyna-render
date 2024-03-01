#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

uniform vec3 u_CameraPos;
uniform vec3 u_VirtualCameraPos;

uniform samplerCube u_Skybox;
uniform sampler2D u_ReflectionTexture;


void main()
{
    vec3 viewDir = normalize(FragPos - u_CameraPos);

    vec3 reflectDir = reflect(viewDir, normalize(Normal));

    vec3 envColor = texture(u_Skybox, reflectDir).rgb;
    vec3 reflectionColor = texture(u_ReflectionTexture, TexCoords).rgb;
    vec3 color = mix(envColor, reflectionColor, 0.5);


    FragColor = vec4(color, 1.0);
}
