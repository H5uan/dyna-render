#version 460 core

out vec4 FragColor;
out int color2;

struct VertexOut{
    vec3 WorldPos;
    vec3 Normal;
    vec2 TexCoord;
};

layout (location = 0) in VertexOutput Input;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

// Point lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

// Camera
uniform vec3 camPos;

// HDR tonemapping
// uniform float exposure;

// Directional light
uniform vec3 lightDir;
uniform float dirLightIntensity;
// uniform LightSpaceMatrices

// Shadow (CSM) TODO

// ----------------------------------------- Utility ----------------------------------------------
#define POW_CLAMP 0.000001

#define M_PI 3.1415926535897932384626433832795

float ClampedPow(float X, float Y) {
    return pow(max(abs(X), POW_CLAMP), Y);
}

vec2 ClampedPow(vec2 X, float Y) {
    return pow(max(abs(X), POW_CLAMP), vec2(Y));
}

vec3 ClampedPow(vec3 X, float y) {
    return pow(max(abs(X), POW_CLAMP), vec3(Y));
}

vec4 ClampedPow(vec4 X, float y) {
    return pow(max(abs(X), POW_CLAMP), vec4(Y));
}

float PhongShadingPow(float X, float Y){
    return ClampedPow(X, Y);
}

// ----------------------------------------- PBR Function -----------------------------------------
vec3 GetNormalFromMap(){
    vec3 tangentNormal = texture(normalMap, Input.TexCoord).xyz * 1.0;// Remap [0,1] to [-1,1]

    vec3 Q1 = dFdx(Input.WorldPos);
    vec3 Q2 = dFdy(Input.WorldPos);
    vec2 st1 = dFdx(Input.TexCoord);
    vec2 st2 = dFdy(Input.TexCoord);

    vec3 N   = normalize(Input.Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

// ------ BRDF Function ------

// -- Normal Distribution --

// UE Normalized Blinn-Phong
// [Blinn 1977, "Models of light reflection for computer synthesized pictures"]
float D_Blinn(vec3 N, vec3 H, float roughness){
    float alpha = roughness * roughness;
    float alphaSqaure = alpha * alpha;

    float NdotH = max(dot(N, H), 0.0);

    float power = 2 / alphaSqaure -2;
    return (n + 2) / 2 * M_PI * PhongShadingPow(NdotH, n);
}

// UE Beckmann
// [Beckmann 1963, "The scattering of electromagnetic waves from rough surfaces"]
float D_Beckmann(vec3 N, vec3 H, float roughness){
    float alpha = roughness * roughness;
    float alphaSqaure = alpha * alpha;

    float NdotH = max(dot(N, H), 0.0);
    float NdotHSquare = NdotH * NdotH;

    return exp((NdotHSquare - 1) / (alphaSqaure * NdotHSquare) / (M_PI * alphaSqaure * NdotHSquare * NdotHSquare));
}

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

// UE Anisotropic Trowbridge-Reitz GGX
// [Burley 2012, "Physically-Based Shading at Disney"]
// TODO


// -- Geometry Distribution --

float Vis_Implicit(){
    return 0.25;
}

float G_Schlick(float NdotV, float roughness){
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float G_Smith(vec3 N, vec3 V, vec3 L, float roughness){
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// -- Fresnel Function

vec3 F_None (vec3 SpecularColor){
    return SpecularColor;
}

vec3 F_Schlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
vec3 F_Roughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// ------ Diffuse ------
vec3 Diffuse_Lambert(vec3 Ks, vec3 albedo, float metallic)
{
    vec3 Kd = (vec3(1.0f, 1.0f, 1.0f) - Ks) * (1 - metallic);
    return (Kd * albedo / PI);
}

// ------ Specular ------
vec3 Specular_CookTorrance(vec3 n, vec3 l, vec3 v, float roughness, float metalness, vec3 f0, out vec3 kS){
    vec3 h = normalize(v + l);

    float D = DistributionGGX(n, h, roughness);
    float G = GeometrySmith(n, v, l, roughness);
    vec3 F  = F_Schlick(max(dot(h, v), 0.0), f0);

    // kS is equal to Fresnel
    kS = F;

    float NdotV = max(dot(n, v), 0.0f);
    float NdotL = max(dot(n, l), 0.0f);
    return (D * G * F) / (4.0 * max(NdotV * NdotL, 0.01f));
}

void main(){
    vec3 albedo = pow(texture(albedoMap, Input.TexCoord).rgb, vec3(2.2));
    float metallic = texture(metallicMap, Input.TexCoord).r;
    float roughness = texture(roughnessMap, Input.TexCoord).r;
    float ao = texture(aoMap, Input.TexCoord).r;

    // input lighting data
    vec3 N = getNormalFromMap();
    vec3 V = normalize(camPos - Input.WorldPos);
    vec3 R = reflect(-V, N);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = mix(vec3(F0_NON_METAL), albedo, metallic);

    // Point Light PBR
    for (int i = 0; i < 4; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - Input.WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - Input.WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        vec3 kS;
        vec3 specularBRDF = CookTorrance(N, L, V, roughness, metallic, F0, kS);

        // kS is equal to Fresnel
        vec3 diffuseBRDF = LambertDiffuse(kS, albedo, metallic);

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (diffuseBRDF + specularBRDF) * radiance * NdotL;// note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }

    // Directional Light PBR
    {
        vec3 L = lightDir;

        vec3 kS;
        vec3 specularBRDF = CookTorrance(N, L, V, roughness, metallic, F0, kS);

        // kS is equal to Fresnel
        vec3 diffuseBRDF = LambertDiffuse(kS, albedo, metallic);

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        vec3 radiance = vec3(10.0) * dirLightIntensity;

        Lo += (diffuseBRDF + specularBRDF) * radiance * NdotL;
    }

    // ambient lighting (IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;

    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    // calculate shadow
    // float shadow = ShadowCalculation(Input.WorldPos);
    // vec3 color = ambient + (1.0 - shadow) * Lo;

    // HDR tonemapping
    color = vec3(1.0) - exp(-color * exposure);
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}


