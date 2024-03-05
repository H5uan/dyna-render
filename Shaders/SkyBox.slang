struct Uniforms{
    float4x4 modelViewProjection;
    SamplerCube skyBox;
};

struct VertexAttrib{
    float3 position : POSITION;
};

struct VSOutput{
    vector<float,3> texCoords;
};

struct PSOutput{
    float4 fragColor;
};


// Vertex Shader

struct VertexStageOutput{
    VSOutput vsOutput : VSOutput;
    float4 sv_position : SV_Position;
};

[shader("vertex")]
VertexStageOutput vertexMain(
    VertexAttrib vertexAttrib, uniform Uniforms uniforms)
{
    VertexStageOutput output;
    output.vsOutput.texCoords = vertexAttrib.position;

    float4 position = mul(uniforms.modelViewProjection, vector<float, 4>(vertexAttrib.position,1.0f));
    output.sv_position = position.xyww;

    return output;
}

// Fragment Shader

[shader("fragment")]
float4 fragmentMain(VSOutput vsOutput, uniform Uniforms uniforms): SV_Target{
    return uniforms.skyBox.Sample(vsOutput.texCoords);
}