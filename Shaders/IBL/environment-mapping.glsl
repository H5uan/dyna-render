#type vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} vs_out;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main(){
    vs_out.Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    vs_out.FragPos = vec3(u_Model * vec4(aPos, 1.0));
    gl_Position = u_ViewProjection * vec4(vs_out.FragPos, 1.0);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} fs_in;

struct PointLight{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform vec3 u_CameraPos;
uniform samplerCube u_Skybox;
uniform PointLight light;
uniform Material material;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main(){
    // Environment View
    vec3 viewDir = normalize(fs_in.FragPos - u_CameraPos);

    vec3 norm = normalize(fs_in.Normal);
    vec3 viewFragDir = normalize(u_CameraPos - fs_in.FragPos);

    vec3 reflectDir = reflect(viewDir, normalize(fs_in.Normal));
    vec3 lighting = CalcPointLight(light, norm, fs_in.FragPos, viewFragDir);
    vec3 envColor = texture(u_Skybox, reflectDir).rgb;
    vec3 result = mix(lighting, envColor, 0.5);

    FragColor = vec4(result, 1.0);
}
