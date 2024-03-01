#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform vec3 viewPos;
uniform samplerCube u_Skybox;

struct Material {
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform Material material;
uniform PointLight light;
uniform float reflectivity;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = light.ambient * material.diffuseColor;
    vec3 diffuse = light.diffuse * diff * material.diffuseColor;
    vec3 specular = light.specular * spec * material.specularColor;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcReflection(vec3 viewDir, vec3 normal) {
    vec3 reflectDir = reflect(-viewDir, normal);
    vec3 envColor = texture(u_Skybox, reflectDir).rgb;
    return envColor;
}

void main() {
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 pointLight = CalcPointLight(light, norm, fs_in.FragPos, viewDir);
    vec3 reflection = CalcReflection(viewDir, norm);

    vec3 result = mix(pointLight, reflection, reflectivity);
    FragColor = vec4(result, 1.0);
}
