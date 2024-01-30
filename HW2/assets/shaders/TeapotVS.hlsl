struct VS_INPUT{
    float pos: POSITION0
};

struct VS_OUTPUT{
    float4 pos: SV_POSITION;
float distance : TEXCOORD0;
};

cbuffer MatrixBuffer{
    matrix u_ViewProjection;
};

VS_OUTPUT main(VS_INPUT input){
    VS_OUTPUT output;
    output.pos = mul(u_ViewProjection, float4(0.05 * input.pos, 1.0f));
    output.distance = output.pos.z;
    return output;
}
