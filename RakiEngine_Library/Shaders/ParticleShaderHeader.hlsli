cbuffer cbuff0 : register(b0)
{
    matrix mat;                 //ビュープロジェクション行列
    matrix matBill;             //ビルボード行列
};

struct VSOutput
{
    float4 pos   : POSITION;    //頂点座標
    float scale  : TEXCOORD;    //スケーリング
};

struct GSOutput
{
    float4 svpos : SV_POSITION; //システム頂点
    float2 uv    : TEXCOORD;    //uv値
};