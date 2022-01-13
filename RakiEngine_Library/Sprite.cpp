#include "Sprite.h"
#include "NY_Camera.h"



void Sprite::CreateSprite(ID3D12Device *dev, XMFLOAT2 size, XMFLOAT2 anchor, UINT resourceID, bool adjustResourceFlag)
{
	HRESULT result;

    //���\�[�XID�ݒ�
    spdata.texNumber = resourceID;

    //�A���J�[�|�C���g�̃R�s�[
    spdata.anchorPoint = anchor;

	//���_�f�[�^
	SpriteVertex vertices[] = {
		{{0.0f,0.0f,0.0f},{0.0f,1.0f}},
		{{0.0f,0.0f,0.0f},{0.0f,0.0f}},
		{{0.0f,0.0f,0.0f},{1.0f,1.0f}},
		{{0.0f,0.0f,0.0f},{1.0f,0.0f}},
	};

    //�A���J�[�|�C���g�ɍ��킹���ݒ�
    //float left   = (0.0f - spdata.anchorPoint.x) * spdata.size.x;
    //float right  = (1.0f - spdata.anchorPoint.x) * spdata.size.x;
    //float top    = (0.0f - spdata.anchorPoint.y) * spdata.size.y;
    //float bottom = (1.0f - spdata.anchorPoint.y) * spdata.size.y;

    //vertices[0].pos = { left,bottom,0.0f };
    //vertices[1].pos = { left,   top,0.0f };
    //vertices[2].pos = { right,bottom,0.0f };
    //vertices[3].pos = { right,  top,0.0f };

	//���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(SpriteVertex) * _countof(vertices));

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
    result = dev->CreateCommittedResource(
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
    for (int i = 0; i < _countof(vertices); i++)
    {
        vertMap[i] = vertices[i];//���W���R�s�[
    }
    //�}�b�v������
    spdata.vertBuff->Unmap(0, nullptr);

    //���_�o�b�t�@�r���[����
    spdata.vbView.BufferLocation = spdata.vertBuff->GetGPUVirtualAddress();
    spdata.vbView.SizeInBytes = sizeof(vertices);
    spdata.vbView.StrideInBytes = sizeof(SpriteVertex);

    auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpConstBufferData) + 0xff) & ~0xff);
    //�萔�o�b�t�@����
    result = dev->CreateCommittedResource(
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
    if (adjustResourceFlag == true && SpriteManager::Get()->texbuff[resourceID])
    {
        //�e�N�X�`�����擾
        D3D12_RESOURCE_DESC resDesc = SpriteManager::Get()->texbuff[spdata.texNumber]->GetDesc();
        //���\�[�X�ɍ��킹�ăT�C�Y����
        spdata.size = { (float)resDesc.Width,(float)resDesc.Height };
        //�A���J�[�|�C���g�ɉ���������

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

    //���_�f�[�^
    SpriteVertex vertices[] = {
        {{0.0f,0.0f,0.0f},{0.0f,1.0f}},
        {{0.0f,0.0f,0.0f},{0.0f,0.0f}},
        {{0.0f,0.0f,0.0f},{1.0f,1.0f}},
        {{0.0f,0.0f,0.0f},{1.0f,0.0f}},
    };

    //�A���J�[�|�C���g�ɍ��킹���ݒ�
    float left   = (0.0f - spdata.anchorPoint.x) * spdata.size.x;
    float right  = (1.0f - spdata.anchorPoint.x) * spdata.size.x;
    float top    = (0.0f - spdata.anchorPoint.y) * spdata.size.y;
    float bottom = (1.0f - spdata.anchorPoint.y) * spdata.size.y;

    vertices[0].pos = { left ,bottom,0.0f };
    vertices[1].pos = { left ,   top,0.0f };
    vertices[2].pos = { right,bottom,0.0f };
    vertices[3].pos = { right,   top,0.0f };

    //���_�o�b�t�@�]��
    SpriteVertex *vertMap = nullptr;
    result = spdata.vertBuff->Map(0, nullptr, (void **)&vertMap);
    //�S���_�ɑ΂���
    memcpy(vertMap, vertices, sizeof(vertices));
    //�}�b�v������
    spdata.vertBuff->Unmap(0, nullptr);

}

void Sprite::UpdateSprite()
{
    spdata.matWorld = XMMatrixIdentity();

    spdata.matWorld *= XMMatrixRotationZ(XMConvertToRadians(spdata.rotation));

    spdata.matWorld *= XMMatrixTranslation(spdata.position.x, spdata.position.y, spdata.position.z);

    //�萔�o�b�t�@�]��
    SpConstBufferData *constMap = nullptr;
    HRESULT result = spdata.constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->mat = spdata.matWorld * SpriteManager::Get()->matProjection;
    spdata.constBuff->Unmap(0, nullptr);

    /*constMap->mat = XMMatrixIdentity();
    constMap->mat = spdata.matWorld * cam._matView * spmgr->matProjection;

    spdata.screenPos = { constMap->mat.r[3].m128_f32[0],constMap->mat.r[3].m128_f32[1] };*/
}

void Sprite::Draw(ID3D12GraphicsCommandList *cmd,ID3D12Device *dev)
{
    //���_�o�b�t�@�Z�b�g
    cmd->IASetVertexBuffers(0, 1, &spdata.vbView);
    //�萔�o�b�t�@�Z�b�g
    cmd->SetGraphicsRootConstantBufferView(0, spdata.constBuff->GetGPUVirtualAddress());
    //�V�F�[�_�[���\�[�X�r���[���Z�b�g
    cmd->SetGraphicsRootDescriptorTable(1, 
        CD3DX12_GPU_DESCRIPTOR_HANDLE(SpriteManager::Get()->descheap->GetGPUDescriptorHandleForHeapStart(),
        spdata.texNumber, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
    //�`��
    cmd->DrawInstanced(4, 1, 0, 0);

}