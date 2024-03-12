#type vertex
#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
    vs_out.FragPos = vec3(u_Model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    vs_out.TangentLightPos = TBN * lightPos;
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;

    gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0);
}

#type fragment
#version 460

out FragColor;

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D normalMap;

uniform vec3 viewPos;

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

uniform SpotLight spotLight;

struct ColoredMaterial{
    vec3 color;
    float shininess;
};

uniform ColoredMaterial material;

vec3 calSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 cameraDir){
    vec3 lightDir = normalize(fs_in.TangentLightPos-fragPos);

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

    //float shadow = calShadow(fs_in.FragPosLightSpace, light);

    //return (ambient + (1.0 - shadow) * diffuse + specular) * luminosity * intensity;
    return (ambient + diffuse + specular) * luminosity;
}

void main(){
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 cameraDir = normalize(fs_in.TangentViewPos - fs_in.TangnetFragPos);
    vec3 result = vec3(0.f, 0.f, 0.f);

    result += calSpotLight(spotLight, normal, fs_in.TangentFragPos, cameraDir);

    FragColor = vec4(result, 1.0f);
}