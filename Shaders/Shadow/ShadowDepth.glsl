#type vertex

#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 Position;
    vec2 TexCoords;
} vs_out;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;


void main()
{
    vs_out.Position = aPos;
    vs_out.TexCoords = aTexCoords;
}

#type tess_control
#version 460 core

layout(vertices = 3) out;

in VS_OUT {
    vec3 Position;
    vec2 TexCoords;
} tcs_in[];

out TCS_OUT {
    vec3 Position;
    vec2 TexCoords;
} tcs_out[];

uniform int outerLevel;
uniform int innerLevel;

void main()
{
    tcs_out[gl_InvocationID].Position = tcs_in[gl_InvocationID].Position;
    tcs_out[gl_InvocationID].TexCoords = tcs_in[gl_InvocationID].TexCoords;
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
    vec2 TexCoords;
} tes_in[];

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

uniform sampler2D displacementMap;
uniform float displacementScale;

void main(){
    vec3 pos = gl_TessCoord.x * tes_in[0].Position + gl_TessCoord.y * tes_in[1].Position + gl_TessCoord.z * tes_in[2].Position;
    vec2 texCoords = gl_TessCoord.x * tes_in[0].TexCoords + gl_TessCoord.y * tes_in[1].TexCoords + gl_TessCoord.z * tes_in[2].TexCoords;

    float displacement = texture(displacementMap, texCoords).r;
    vec3 displacedPosition = pos + vec3(0.0, displacement * displacementScale, 0.0);

    vec4 worldPosition = u_Model * vec4(displacedPosition, 1.0);
    gl_Position = u_ViewProjection * vec4(worldPosition.xyz, 1.0);
}

#type fragment

#version 460 core

void main()
{
    // Only need z buffer
}