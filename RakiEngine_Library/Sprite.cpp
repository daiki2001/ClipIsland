#include "Sprite.h"
#include "NY_Camera.h"

#include "TexManager.h"

void Sprite::CreateSprite(XMFLOAT2 size, XMFLOAT2 anchor, UINT resourceID, bool adjustResourceFlag, uvAnimData *animData)
{
	HRESULT result;

    if (animData != nullptr) {
        this->animData = animData;
        //���_�f�[�^
        SpriteVertex vertices[] = {
            {{0.0f,0.0f,0.0f},this->animData->GetOffset().offsetLT},
            {{0.0f,0.0f,0.0f},this->animData->GetOffset().offsetRT},
            {{0.0f,0.0f,0.0f},this->animData->GetOffset().offsetLB},
            {{0.0f,0.0f,0.0f},this->animData->GetOffset().offsetRB},
        };

        spdata.vertices[0] = vertices[0];
        spdata.vertices[1] = vertices[1];
        spdata.vertices[2] = vertices[2];
        spdata.vertices[3] = vertices[3];
    }
    else {
        //���_�f�[�^
        SpriteVertex vertices[] = {
            {{0.0f,0.0f,0.0f},{0.0f,0.0f}},
            {{0.0f,0.0f,0.0f},{1.0f,0.0f}},
            {{0.0f,0.0f,0.0f},{0.0f,1.0f}},
            {{0.0f,0.0f,0.0f},{1.0f,1.0f}},
        };

        spdata.vertices[0] = vertices[0];
        spdata.vertices[1] = vertices[1];
        spdata.vertices[2] = vertices[2];
        spdata.vertices[3] = vertices[3];
    }

    //���\�[�XID�ݒ�
    spdata.texNumber = resourceID;

    //�A���J�[�|�C���g�̃R�s�[
    spdata.anchorPoint = anchor;



	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(SpriteVertex) * _countof(spdata.vertices));

	//���_�o�b�t�@����
    D3D12_HEAP_PROPERTIES heapprop{}; //�q�[�v�ݒ�
    heapprop.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p
    D3D12_RESOURCE_DESC resdesc{}; //���\�[�X�ݒ�
    resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resdesc.Width = sizeVB; //���_�f�[�^�S�̂̃T�C�Y
    resdesc.Height = 1;
    resdesc.DepthOrArraySize = 1;
    resdesc.MipLevels = 1;
    resdesc.SampleDesc.Count = 1;
    resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    //���_�o�b�t�@�̐���
    result = SpriteManager::Get()->dev->CreateCommittedResource(
        &heapprop, //�q�[�v�ݒ�
        D3D12_HEAP_FLAG_NONE,
        &resdesc, //���\�[�X�ݒ�
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&spdata.vertBuff));

    //-----���_�o�b�t�@�ւ̃f�[�^�]��-----//
    SpriteVertex *vertMap = nullptr;
    result = spdata.vertBuff->Map(0, nullptr, (void **)&vertMap);
    //�S���_�ɑ΂���
    for (int i = 0; i < _countof(spdata.vertices); i++)
    {
        vertMap[i] = spdata.vertices[i];//���W���R�s�[
    }
    //�}�b�v������
    spdata.vertBuff->Unmap(0, nullptr);

    //���_�o�b�t�@�r���[����
    spdata.vbView.BufferLocation = spdata.vertBuff->GetGPUVirtualAddress();
    spdata.vbView.SizeInBytes = sizeof(spdata.vertices);
    spdata.vbView.StrideInBytes = sizeof(SpriteVertex);

    auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpConstBufferData) + 0xff) & ~0xff);
    //�萔�o�b�t�@����
    result = SpriteManager::Get()->dev->CreateCommittedResource(
        &HEAP_PROP,
        D3D12_HEAP_FLAG_NONE,
        &RESDESC,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(&spdata.constBuff)
    );

    //�萔�o�b�t�@�f�[�^�]��
    SpConstBufferData *constMap = nullptr;
    result = spdata.constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->color = XMFLOAT4(1, 1, 1, 1);//�F�w��
    //���s���e�s��
    constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, SpriteManager::Get()->window_width, SpriteManager::Get()->window_height, 0.0f, 0.0f, 1.0f);
    spdata.constBuff->Unmap(0, nullptr);

    //���\�[�X�ɍ��킹�Ē�������ꍇ
    if (adjustResourceFlag == true)
    {
        //�e�N�X�`�����擾
        D3D12_RESOURCE_DESC resDesc = TexManager::textureData[resourceID].texBuff->GetDesc();
        //���\�[�X�ɍ��킹�ăT�C�Y����
        spdata.size = { (float)resDesc.Width,(float)resDesc.Height };
    }
    else//���Ȃ��ꍇ
    {
        spdata.size = size;//�����̃T�C�Y�ɐݒ�
    }

    ResizeSprite(spdata.size);

}

void Sprite::ResizeSprite(XMFLOAT2 newsize)
{
    HRESULT result;

    spdata.size = newsize;

    //�A���J�[�|�C���g�ɍ��킹���ݒ�
    float left   = (0.0f - spdata.anchorPoint.x) * spdata.size.x;
    float right  = (1.0f - spdata.anchorPoint.x) * spdata.size.x;
    float top    = (0.0f - spdata.anchorPoint.y) * spdata.size.y;
    float bottom = (1.0f - spdata.anchorPoint.y) * spdata.size.y;

    spdata.vertices[0].pos = { left  ,   top,0.0f };
    spdata.vertices[1].pos = { right ,   top,0.0f };
    spdata.vertices[2].pos = { left  ,bottom,0.0f };
    spdata.vertices[3].pos = { right ,bottom,0.0f };

    //���_�o�b�t�@�]��
    SpriteVertex *vertMap = nullptr;
    result = spdata.vertBuff->Map(0, nullptr, (void **)&vertMap);
    //�S���_�ɑ΂���
    memcpy(vertMap, spdata.vertices, sizeof(spdata.vertices));
    //�}�b�v������
    spdata.vertBuff->Unmap(0, nullptr);

}

void Sprite::UpdateSprite()
{
    //�A�j���[�V�����X�V
    if (animData != nullptr) {
        spdata.vertices[0].uv = animData->GetOffset().offsetLB; //����
        spdata.vertices[1].uv = animData->GetOffset().offsetRB; //����
        spdata.vertices[2].uv = animData->GetOffset().offsetLT; //����
        spdata.vertices[3].uv = animData->GetOffset().offsetRT; //����
        //���_�o�b�t�@�f�[�^�]��
        SpriteVertex *vertMap = nullptr;
        auto result = spdata.vertBuff->Map(0, nullptr, (void **)&vertMap);
        //�S���_�ɑ΂���
        memcpy(vertMap, spdata.vertices, sizeof(spdata.vertices));
        //�}�b�v������
        spdata.vertBuff->Unmap(0, nullptr);
    }


    spdata.matWorld = XMMatrixIdentity();

    spdata.matWorld *= XMMatrixRotationZ(XMConvertToRadians(spdata.rotation));

    spdata.matWorld *= XMMatrixTranslation(spdata.position.x, spdata.position.y, spdata.position.z);

    //�萔�o�b�t�@�]��
    SpConstBufferData *constMap = nullptr;
    HRESULT result = spdata.constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->mat = spdata.matWorld * SpriteManager::Get()->matProjection;
    spdata.constBuff->Unmap(0, nullptr);

}

void Sprite::Draw()
{
    //���_�o�b�t�@�Z�b�g
    SpriteManager::Get()->cmd->IASetVertexBuffers(0, 1, &spdata.vbView);
    //�萔�o�b�t�@�Z�b�g
    SpriteManager::Get()->cmd->SetGraphicsRootConstantBufferView(0, spdata.constBuff->GetGPUVirtualAddress());
    //�V�F�[�_�[���\�[�X�r���[���Z�b�g
    SpriteManager::Get()->cmd->SetGraphicsRootDescriptorTable(1,
        CD3DX12_GPU_DESCRIPTOR_HANDLE(TexManager::texDsvHeap->GetGPUDescriptorHandleForHeapStart(),
        spdata.texNumber, SpriteManager::Get()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    //�`��
    SpriteManager::Get()->cmd->DrawInstanced(4, 1, 0, 0);

}
