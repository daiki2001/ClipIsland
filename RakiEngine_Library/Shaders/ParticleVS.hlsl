#include "ParticleShaderHeader.hlsli"

VSOutput main( float4 pos : POSITION ,float scale : TEXCOORD)
{
    //取得した頂点情報をそのまま返却
    VSOutput output;
    output.pos = pos;
    output.scale = scale;
    return output;
}