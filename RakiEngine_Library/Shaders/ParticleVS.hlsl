#include "ParticleShaderHeader.hlsli"

VSOutput main( float4 pos : POSITION ,float scale : TEXCOORD)
{
    //�擾�������_�������̂܂ܕԋp
    VSOutput output;
    output.pos = pos;
    output.scale = scale;
    return output;
}