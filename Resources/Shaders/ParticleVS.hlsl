#include "ParticleShaderHeader.hlsli"

VSOutput main( float4 pos : POSITION ,float scale : TEXCOORD,float4 color : COLOR)
{
    //�擾�������_�������̂܂ܕԋp
    VSOutput output;
    output.pos = pos;
    output.scale = scale;
    output.color = color;
    return output;
}