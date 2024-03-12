#type vertex

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform mat4 u_LightSpaceMVPMat;

void main(){
    vs_out.FragPos = vec3(u_Model * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(u_Model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = u_LightSpaceMVPMat * vec4(vs_out.FragPos, 1.0);
    gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0);
}

#type fragment
#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

/***
struct TexturedMaterial{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
***/

struct ColoredMaterial{
    vec3 color;
    float shininess;
};

uniform ColoredMaterial material;

uniform vec3 u_CameraPos;
uniform sampler2D u_ShadowMap;


// Light Definition
struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 calDirLight(DirLight light, vec3 normal, vec3 cameraDir){
    vec3 lightDir = normalize(-light.direction);

    // amibent
    vec3 ambient = light.ambient * material.color;
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.color;
    // specular
    vec3 halfwayDir = normalize(lightDir+cameraDir);
    float spec = pow(max(dot(cameraDir, halfwayDir), 0.f), material.shininess);
    vec3 specular = light.specular * spec * material.color;

    return (ambient + diffuse + specular);
}

struct PointLight
{
    vec3 position;

// attenuation
    float constant;
    float linear;
    float quadratic;

// light components
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 calPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 cameraDir){
    vec3 lightDir = normalize(light.position-fragPos);

    // amibent
    vec3 ambient = light.ambient;
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff;
    // specular
    vec3 halfwayDir = normalize(lightDir+cameraDir);
    float spec = pow(max(dot(cameraDir, halfwayDir), 0.f), material.shininess);
    vec3 specular = light.specular * spec;

    // attenuation
    float distance = length(light.position - fragPos);
    float luminosity = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    return (ambient + diffuse + specular) * luminosity;
}

struct SpotLight
{
    vec3 direction;// the center direction
    vec3 position;
    float innerCos;
    float outerCos;

// attenuation
    float constant;
    float linear;
    float quadratic;

// componennts
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
float calShadow(vec4 fragPosInLightSpace, SpotLight light);

vec3 calSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 cameraDir){
    vec3 lightDir = normalize(light.position-fragPos);

    // amibent
    vec3 ambient = light.ambient;
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff;
    // specular
    vec3 halfwayDir = normalize(lightDir+cameraDir);
    float spec = pow(max(dot(cameraDir, halfwayDir), 0.f), material.shininess);
    vec3 specular = light.specular * spec;

    // distance attenuation
    float distance = length(light.position - fragPos);
    float luminosity = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // direction attenuation
    float theta = dot(lightDir, -light.direction);
    float epsilon = light.innerCos - light.outerCos;// cosine: [1,0] (0-90 degeree)
    float intensity =  clamp((theta - light.outerCos) / epsilon, 0.0, 1.0);

    float shadow = calShadow(fs_in.FragPosLightSpace, light);

    return (ambient + (1.0 - shadow) * diffuse + specular) * luminosity * intensity;
}

uniform DirLight dirlight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform int isSpotLightOn;

// Shadow
float calShadow(vec4 fragPosInLightSpace, SpotLight light){
    vec3 projCoords = fragPosInLightSpace.xyz / fragPosInLightSpace.w;// equal to gl_position
    projCoords = projCoords * 0.5 + 0.5;// remap to [0,1]

    float closestDepth = texture(u_ShadowMap, projCoords.xy).r;// get current depth in the depth buffer
    float currentDepth = projCoords.z;// // get current camera space depth

    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.003);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}


void main(){
    vec3 norm = normalize(fs_in.Normal);
    vec3 cameraDir = normalize(u_CameraPos - fs_in.FragPos);
    vec3 result = vec3(0.f, 0.f, 0.f);

    result += calSpotLight(spotLight, norm, fs_in.FragPos, cameraDir);

    FragColor = vec4(result, 1.0f);
}