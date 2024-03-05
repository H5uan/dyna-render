#version 460 core

out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;
uniform float roughness;

#define M_PI 3.1415926535897932384626433832795

// UE Isotropic Trowbridge-Reitz GGX
// [Walter et al. 2007, "Microfacet models for refraction through rough surfaces"]
float D_GGX(vec3 N, vec3 H, float roughness){
    float alpha = roughness * roughness;
    float alphaSqaure = alpha * alpha;

    float NdotH = max(dot(N, H), 0.0);
    float NdotHSquare = NdotH * NdotH;

    float nom = alphaSqaure;
    float denom = (NdotHSquare  * (alphaSqaure-1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

// Quasirandom sequences
float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10;// / 0x100000000
}

vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

// Biased importance sampling
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness*roughness;

    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    // from spherical coordinates to cartesian coordinates - halfway vector
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    // from tangent-space H vector to world-space sample vector
    vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);

    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

void main(){
    vec3 N = normalize(WorldPos);

    // make the simplyfying assumption that V equals R equals the normal
    // reflecting vector
    vec3 R = N;
    vec3 V = R;

    const uint SAMPLE_COUNT = 1024u;
    vec3 prefilteredColor = vec3(0.0);
    float totalWeight = 0.0;

    for (uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        // generates a sample vector that's biased towards the preferred alignment direction (importance sampling).
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if (NdotL > 0.0)// only not 90 degree have reflection
        {
            // sample from the environment's mip level based on roughness/pdf
            float D   = D_GGX(N, H, roughness);
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001;

            float resolution = 512.0;// resolution of source cubemap (per face)
            // solid angle
            float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
            float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            // Determine mipmap level according to roughness since roughness can affect sampling
            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

            prefilteredColor += textureLod(environmentMap, L, mipLevel).rgb * NdotL;
            totalWeight      += NdotL;// The closer the direction of the incident light is to the normal direction, the greater its contribution to the illumination should be.

        }
    }

    prefilteredColor = prefilteredColor / totalWeight;

    FragColor = vec4(prefilteredColor, 1.0);
}
