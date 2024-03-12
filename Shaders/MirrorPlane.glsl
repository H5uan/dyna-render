#type vertex
#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexcoords;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec4 ClipPos;

void main(){
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    TexCoords = aTexcoords;
    FragPos = vec3(u_Model * vec4(aPos, 1.0));
    ClipPos = u_ViewProjection * vec4(FragPos, 1.0);
    gl_Position = u_ViewProjection * vec4(FragPos, 1.0);
}

#type fragment
#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;
in vec4 ClipPos;

uniform vec3 u_CameraPos;
uniform vec3 u_VirtualCameraPos;

uniform samplerCube u_Skybox;
uniform sampler2D u_ReflectionTexture;


vec3 GetPlanarReflectionColor(sampler2D reflectionRT, vec4 refClipPos) {
    vec2 refUV = (refClipPos.xy / refClipPos.w) * 0.5 + 0.5;
    return texture(reflectionRT, refUV).rgb;
}

void main()
{
    vec3 viewDir = normalize(FragPos - u_CameraPos);

    vec3 reflectDir = reflect(viewDir, normalize(Normal));

    vec3 envColor = texture(u_Skybox, reflectDir).rgb;
    vec3 reflectionColor = GetPlanarReflectionColor(u_ReflectionTexture, ClipPos);
    vec3 color = mix(envColor, reflectionColor, 0.5);


    FragColor = vec4(color, 1.0);
}
