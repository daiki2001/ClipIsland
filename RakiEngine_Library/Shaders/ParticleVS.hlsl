#include "ParticleShaderHeader.hlsli"

VSOutput main( float4 pos : POSITION ,float scale : TEXCOORD)
{
    //æ“¾‚µ‚½’¸“_î•ñ‚ğ‚»‚Ì‚Ü‚Ü•Ô‹p
    VSOutput output;
    output.pos = pos;
    output.scale = scale;
    return output;
}