#include "TexManager.h"
#include "Raki_DX12B.h"

TexManager::texture								TexManager::textureData[1024];
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	TexManager::texDsvHeap;
const int										TexManager::MAX_TEXNUM = 1024;

ID3D12Device *TexManager::dev;

void TexManager::InitTexManager()
{
    HRESULT result;
    //デスクリプタヒープ生成
    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NumDescriptors = MAX_TEXNUM;
    result = Raki_DX12B::Get()->GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&texDsvHeap));

    dev = Raki_DX12B::Get()->GetDevice();
}

UINT TexManager::LoadTexture(const char *filename)
{
    //ヌルチェック
    assert(filename != nullptr);

    HRESULT result;
    // 使用するテクスチャの番号を設定
    UINT useTexIndexNum = 0;
    for (int i = 0; i < MAX_TEXNUM; i++) {
        //空のテクスチャを発見
        if (textureData[i].texBuff == nullptr) {
            //番号設定
            useTexIndexNum = (UINT)i;
            textureData[i].texNumber = i;
            break;
        }
    }

    // const char => wchar_t
    size_t newsize = strlen(filename) + 1;
    //wchar_tに変換した文字列を格納する変数を作成
    wchar_t *FileName = new wchar_t[newsize];
    //wchar_tに変換
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, FileName, newsize, filename, _TRUNCATE);

    // WICテクスチャのロード
    result = LoadFromWICFile(FileName,
        WIC_FLAGS_NONE,
        &textureData[useTexIndexNum].metaData,
        textureData[useTexIndexNum].scratchImg);
    textureData[useTexIndexNum].img = textureData[useTexIndexNum].scratchImg.GetImage(0, 0, 0);

    // テクスチャバッファ生成
    D3D12_HEAP_PROPERTIES texHeapProp{};//テクスチャヒープ設定
    texHeapProp.Type                    = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty         = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference    = D3D12_MEMORY_POOL_L0;

    D3D12_RESOURCE_DESC texresDesc{};//リソース設定
    texresDesc.Dimension        = static_cast<D3D12_RESOURCE_DIMENSION>(textureData[useTexIndexNum].metaData.dimension);//2Dテクスチャ用
    texresDesc.Format           = textureData[useTexIndexNum].metaData.format;//RGBAフォーマット
    texresDesc.Width            = textureData[useTexIndexNum].metaData.width;//横
    texresDesc.Height           = (UINT)textureData[useTexIndexNum].metaData.height;//縦
    texresDesc.DepthOrArraySize = (UINT16)textureData[useTexIndexNum].metaData.arraySize;
    texresDesc.MipLevels        = (UINT16)textureData[useTexIndexNum].metaData.mipLevels;
    texresDesc.SampleDesc.Count = 1;

    result = dev->CreateCommittedResource(//GPUリソース生成
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &texresDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&textureData[useTexIndexNum].texBuff)
    );

    //テクスチャバッファへのデータ転送
    result = textureData[useTexIndexNum].texBuff->WriteToSubresource(
        0,
        nullptr,
        textureData[useTexIndexNum].img->pixels,
        (UINT)textureData[useTexIndexNum].img->rowPitch,
        (UINT)textureData[useTexIndexNum].img->slicePitch
    );

    //シェーダーリソースビュー設定
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureData[useTexIndexNum].metaData.format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    //ヒープにシェーダーリソースビュー作成
    D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = texDsvHeap.Get()->GetCPUDescriptorHandleForHeapStart();
    D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = texDsvHeap.Get()->GetGPUDescriptorHandleForHeapStart();
    dev->CreateShaderResourceView(textureData[useTexIndexNum].texBuff.Get(), &srvDesc,
        CD3DX12_CPU_DESCRIPTOR_HANDLE(texDsvHeap.Get()->GetCPUDescriptorHandleForHeapStart(), useTexIndexNum,
            dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

    return useTexIndexNum;
}

UINT TexManager::LoadTexture(std::string filename)
{
    UINT texNumber = LoadTexture(filename.c_str());
    return texNumber;
}

UINT TexManager::LoadDivTextureTest(const char *filename, const int numDivTex, int sizeX)
{
    //テクスチャ読み込み
    UINT useNo = LoadTexture(filename);

    //該当テクスチャのuvオフセットを用意

    XMFLOAT2 offset_temp;
    //分割数分のオフセットを用意（x方向）
    for (int i = 0; i < numDivTex; i++) {
        offset_temp.x = (float)sizeX / textureData[useNo].metaData.width;
        offset_temp.y = textureData[useNo].metaData.height / textureData[useNo].metaData.height;

        //オフセットをコンテナに格納
        textureData[useNo].uv_offsets.push_back(offset_temp);
    }

    return useNo;
}
