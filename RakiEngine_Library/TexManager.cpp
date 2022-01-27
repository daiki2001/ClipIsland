#include "TexManager.h"
#include "Raki_DX12B.h"

TexManager::texture								TexManager::textureData[1024];
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	TexManager::texDsvHeap;
const int										TexManager::MAX_TEXNUM = 1024;

ID3D12Device *TexManager::dev;

void TexManager::InitTexManager()
{
    HRESULT result;
    //�f�X�N���v�^�q�[�v����
    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NumDescriptors = MAX_TEXNUM;
    result = Raki_DX12B::Get()->GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&texDsvHeap));

    dev = Raki_DX12B::Get()->GetDevice();
}

UINT TexManager::LoadTexture(const char *filename)
{
    //�k���`�F�b�N
    assert(filename != nullptr);

    HRESULT result;
    // �g�p����e�N�X�`���̔ԍ���ݒ�
    UINT useTexIndexNum = 0;
    for (int i = 0; i < MAX_TEXNUM; i++) {
        //��̃e�N�X�`���𔭌�
        if (textureData[i].texBuff == nullptr) {
            //�ԍ��ݒ�
            useTexIndexNum = (UINT)i;
            textureData[i].texNumber = i;
            break;
        }
    }

    // const char => wchar_t
    size_t newsize = strlen(filename) + 1;
    //wchar_t�ɕϊ�������������i�[����ϐ����쐬
    wchar_t *FileName = new wchar_t[newsize];
    //wchar_t�ɕϊ�
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, FileName, newsize, filename, _TRUNCATE);

    // WIC�e�N�X�`���̃��[�h
    result = LoadFromWICFile(FileName,
        WIC_FLAGS_NONE,
        &textureData[useTexIndexNum].metaData,
        textureData[useTexIndexNum].scratchImg);
    textureData[useTexIndexNum].img = textureData[useTexIndexNum].scratchImg.GetImage(0, 0, 0);

    // �e�N�X�`���o�b�t�@����
    D3D12_HEAP_PROPERTIES texHeapProp{};//�e�N�X�`���q�[�v�ݒ�
    texHeapProp.Type                    = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty         = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference    = D3D12_MEMORY_POOL_L0;

    D3D12_RESOURCE_DESC texresDesc{};//���\�[�X�ݒ�
    texresDesc.Dimension        = static_cast<D3D12_RESOURCE_DIMENSION>(textureData[useTexIndexNum].metaData.dimension);//2D�e�N�X�`���p
    texresDesc.Format           = textureData[useTexIndexNum].metaData.format;//RGBA�t�H�[�}�b�g
    texresDesc.Width            = textureData[useTexIndexNum].metaData.width;//��
    texresDesc.Height           = (UINT)textureData[useTexIndexNum].metaData.height;//�c
    texresDesc.DepthOrArraySize = (UINT16)textureData[useTexIndexNum].metaData.arraySize;
    texresDesc.MipLevels        = (UINT16)textureData[useTexIndexNum].metaData.mipLevels;
    texresDesc.SampleDesc.Count = 1;

    result = dev->CreateCommittedResource(//GPU���\�[�X����
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &texresDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&textureData[useTexIndexNum].texBuff)
    );

    //�e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
    result = textureData[useTexIndexNum].texBuff->WriteToSubresource(
        0,
        nullptr,
        textureData[useTexIndexNum].img->pixels,
        (UINT)textureData[useTexIndexNum].img->rowPitch,
        (UINT)textureData[useTexIndexNum].img->slicePitch
    );

    //�V�F�[�_�[���\�[�X�r���[�ݒ�
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = textureData[useTexIndexNum].metaData.format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    //�q�[�v�ɃV�F�[�_�[���\�[�X�r���[�쐬
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
    //�e�N�X�`���ǂݍ���
    UINT useNo = LoadTexture(filename);

    //�Y���e�N�X�`����uv�I�t�Z�b�g��p��

    XMFLOAT2 offset_temp;
    //���������̃I�t�Z�b�g��p�Ӂix�����j
    for (int i = 0; i < numDivTex; i++) {
        offset_temp.x = (float)sizeX / textureData[useNo].metaData.width;
        offset_temp.y = textureData[useNo].metaData.height / textureData[useNo].metaData.height;

        //�I�t�Z�b�g���R���e�i�Ɋi�[
        textureData[useNo].uv_offsets.push_back(offset_temp);
    }

    return useNo;
}
