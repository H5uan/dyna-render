#type vertex
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
} vs_out;


void main()
{
    vs_out.Position = aPos;
    vs_out.Normal = aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.Tangent = aTangent;
    vs_out.Bitangent = aBitangent;
}

#type tess_control
#version 460 core

layout(vertices = 3) out;

in VS_OUT {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
} tcs_in[];

out TCS_OUT {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
} tcs_out[];

uniform int outerLevel;
uniform int innerLevel;

void main()
{
    tcs_out[gl_InvocationID].Position = tcs_in[gl_InvocationID].Position;
    tcs_out[gl_InvocationID].Normal = tcs_in[gl_InvocationID].Normal;
    tcs_out[gl_InvocationID].TexCoords = tcs_in[gl_InvocationID].TexCoords;
    tcs_out[gl_InvocationID].Tangent = tcs_in[gl_InvocationID].Tangent;
    tcs_out[gl_InvocationID].Bitangent = tcs_in[gl_InvocationID].Bitangent;
    if (gl_InvocationID == 0) {
        gl_TessLevelInner[0] = innerLevel;
        gl_TessLevelOuter[0] = outerLevel;
        gl_TessLevelOuter[1] = outerLevel;
        gl_TessLevelOuter[2] = outerLevel;
    }
}

#type tess_evaluation
#version 460 core

layout(triangles, equal_spacing, ccw) in;


in TCS_OUT {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
} tes_in[];

out TES_OUT {
    vec2 TexCoords;
    vec3 FragPosWorldSpace;
    vec3 TangentLightPos;
    vec3 TangentLightDir;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec4 FragPosLightSpace;
} tes_out;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform mat4 u_LightSpaceMVPMat;

uniform sampler2D displacementMap;
uniform float displacementScale;

uniform vec3 lightPos;
uniform vec3 lightDir;
uniform vec3 viewPos;


void main(){
    vec3 pos = gl_TessCoord.x * tes_in[0].Position + gl_TessCoord.y * tes_in[1].Position + gl_TessCoord.z * tes_in[2].Position;

    vec3 normal = normalize(gl_TessCoord.x * tes_in[0].Normal + gl_TessCoord.y * tes_in[1].Normal + gl_TessCoord.z * tes_in[2].Normal);
    vec2 texCoords = gl_TessCoord.x * tes_in[0].TexCoords + gl_TessCoord.y * tes_in[1].TexCoords + gl_TessCoord.z * tes_in[2].TexCoords;

    vec3 tangent = normalize(gl_TessCoord.x * tes_in[0].Tangent + gl_TessCoord.y * tes_in[1].Tangent + gl_TessCoord.z * tes_in[2].Tangent);
    vec3 bitangent = normalize(gl_TessCoord.x * tes_in[0].Bitangent + gl_TessCoord.y * tes_in[1].Bitangent + gl_TessCoord.z * tes_in[2].Bitangent);

    float displacement = texture(displacementMap, texCoords).r;
    vec3 displacedPosition = pos + vec3(0.0, displacement * displacementScale, 0.0);

    vec4 worldPosition = u_Model * vec4(displacedPosition, 1.0);
    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    vec3 T = normalize(normalMatrix * tangent);
    vec3 N = normalize(normalMatrix * normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));

    tes_out.TexCoords = texCoords;
    tes_out.FragPosWorldSpace = displacedPosition;
    tes_out.TangentLightPos = TBN * lightPos;
    tes_out.TangentLightDir = TBN * lightDir;
    tes_out.TangentViewPos = TBN * viewPos;
    tes_out.TangentFragPos = TBN * vec3(worldPosition);
    tes_out.FragPosLightSpace = u_LightSpaceMVPMat * worldPosition;

    gl_Position = u_ViewProjection * vec4(worldPosition.xyz, 1.0);
}


#type fragment
#version 460 core
out vec4 FragColor;

in TES_OUT {
    vec2 TexCoords;
    vec3 FragPosWorldSpace;
    vec3 TangentLightPos;
    vec3 TangentLightDir;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec4 FragPosLightSpace;
} fs_in;

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

struct ColoredMaterial{
    vec3 color;
    float shininess;
};

uniform ColoredMaterial material;

uniform SpotLight spotLight;

uniform sampler2D texture_normal;

uniform sampler2D depthMap;

void main(){

    vec3 cameraDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);    vec2 texCoords = fs_in.TexCoords;
    vec3 norm = texture(texture_normal, texCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    vec3 color = material.color;
    vec3 ambient = 0.1 * color;
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * color;
    vec3 halfWayDir = normalize(lightDir+cameraDir);
    float spec = pow(max(dot(norm, halfWayDir), 0.0), material.shininess);
    vec3 specular = vec3(0.2) * spec;
    // distance attenuation
    float distance = length(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float luminosity = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * distance * distance);

    // direction attenuation
    float theta = dot(lightDir, -fs_in.TangentLightDir);
    float epsilon = spotLight.innerCos - spotLight.outerCos;// cosine: [1,0] (0-90 degeree)
    float intensity =  clamp((theta - spotLight.outerCos) / epsilon, 0.0, 1.0);

    vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(depthMap, projCoords.xy).r;// get current depth in the depth buffer
    float currentDepth = projCoords.z;// // get current camera space depth

    vec3 lightDirWorldSpace = normalize(spotLight.position - fs_in.FragPosWorldSpace);
    float bias = max(0.02 * (1.0-dot(norm, lightDirWorldSpace)), 0.003);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    FragColor = vec4((ambient+(1.0-shadow)*(diffuse+specular))*luminosity*intensity, 1.0f);
}