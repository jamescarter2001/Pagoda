/* vertex attributes go here to input to the vertex shader */
struct vs_in {
    float3 position_local : POS;
    float4 color_local : COL;
};

/* outputs from vertex shader go here. can be interpolated to pixel shader */
struct vs_out {
    float4 position_clip : SV_POSITION;  // required output of VS
    float4 color : COLOR;
};

cbuffer mvpMatrix : register(b0)
{
    matrix g_mvpMat;
};

cbuffer transformMatrix : register(b1)
{
    matrix g_transMat;
};

vs_out vs_main(vs_in input)
{
    vs_out output = (vs_out)0;  // zero the memory first
    float4 pos = float4(input.position_local, 1.0);
    
    float4 transformed = mul(g_transMat, pos);
    output.position_clip = mul(g_mvpMat, transformed);
    output.color = input.color_local;
    return output;
}

float4 ps_main(vs_out input) : SV_TARGET {
    return input.color;  // must return an RGBA colour
}