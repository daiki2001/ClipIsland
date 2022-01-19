#include "NY_Object3DMgr.h"
#include "Raki_DX12B.h"
#include "TexManager.h"

ID3D12Device *NY_Object3DManager::dev = nullptr;

bool NY_Object3DManager::CreateObject3DManager(ID3D12Device *dev, int window_w, int window_h)
{
    //�E�B���h�E�T�C�Y�ݒ�
    window_width = window_w;
    window_height = window_h;
    //�r���[�|�[�g�s�񏉊���
    matViewport.r[0].m128_f32[0] = window_width / 2;
    matViewport.r[1].m128_f32[1] = -window_height / 2;
    matViewport.r[3].m128_f32[0] = window_width / 2;
    matViewport.r[3].m128_f32[1] = window_height / 2;
    //Object3D�p�p�C�v���C������
    object3dPipelineSet = Create3DPipelineState(dev);
    //�ˉe�s�񏉊���
    matProjection = XMMatrixPerspectiveFovLH(
        XMConvertToRadians(60.0f),
        (float)window_width / (float)window_height,
        0.1f, 1000.0f
    );

    //�f�o�C�X�̃|�C���^���i�[
    this->dev = dev;

    return true;
}


HRESULT NY_Object3DManager::QuickDrawShadersCompile()
{
    HRESULT result;//���U���g
    ComPtr<ID3DBlob> errorBlob = nullptr; //�G���[�I�u�W�F�N�g

    //�W���W�I���g���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        L"QuickDrawBoxGeometry.hlsl", //�V�F�[�_�[�t�@�C����
        nullptr,//�V�F�[�_�[�}�N���I�u�W�F�N�g�i����͎g��Ȃ��j
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�I�u�W�F�N�g�i�C���N���[�h�\�ɂ���j
        "main", "gs_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�ݒ�
        0,
        &qd_BoxGS, &errorBlob
    );
    //�V�F�[�_�[�̃G���[���e��\��
    if (FAILED(result))
    {
        //errorBlob����G���[���e��string�^�ɃR�s�[
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //�G���[���e���o�̓E�C���h�E�ɕ\��
        OutputDebugStringA(errstr.c_str());
        exit(1);

        return result;
    }


    //�����܂ŃV�F�[�_�[�̃R���p�C����Ƃ����s
    //��肪�Ȃ���΁A������S_OK���A��
    return result;
}


Pipeline3D NY_Object3DManager::Create3DPipelineState(ID3D12Device *dev)
{
    HRESULT result;

    //-----�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��-----//
#pragma region LoadShader
    ComPtr<ID3DBlob> errorBlob = nullptr; //�G���[�I�u�W�F�N�g

    //���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        L"./RakiEngine_Library/Shaders/OBJVertexShader.hlsl", //�V�F�[�_�[�t�@�C����
        nullptr,//�V�F�[�_�[�}�N���I�u�W�F�N�g�i����͎g��Ȃ��j
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�I�u�W�F�N�g�i�C���N���[�h�\�ɂ���j
        "main", "vs_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�ݒ�
        0,
        &obj_VS, &errorBlob
    );
    //�V�F�[�_�[�̃G���[���e��\��
    if (FAILED(result))
    {
        //errorBlob����G���[���e��string�^�ɃR�s�[
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //�G���[���e���o�̓E�C���h�E�ɕ\��
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    //�W���W�I���g���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        L"./RakiEngine_Library/Shaders/OBJGeometryShader.hlsl", //�V�F�[�_�[�t�@�C����
        nullptr,//�V�F�[�_�[�}�N���I�u�W�F�N�g�i����͎g��Ȃ��j
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�I�u�W�F�N�g�i�C���N���[�h�\�ɂ���j
        "main", "gs_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�ݒ�
        0,
        &obj_GS, &errorBlob
    );
    //�V�F�[�_�[�̃G���[���e��\��
    if (FAILED(result))
    {
        //errorBlob����G���[���e��string�^�ɃR�s�[
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //�G���[���e���o�̓E�C���h�E�ɕ\��
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    //�s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        L"./RakiEngine_Library/Shaders/OBJPixelShader.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &obj_PS, &errorBlob
    );
    //�V�F�[�_�[�̃G���[���e��\��
    if (FAILED(result))
    {
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        //�G���[���e���o�̓E�C���h�E�ɕ\��
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }
#pragma endregion LoadShader

    //-----���_���C�A�E�g-----//
    D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
        {//xyz���W
            "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {//�@��
            "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
        {//uv���W
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
    };

    //-----�O���t�B�b�N�X�p�C�v���C���ݒ�-----//
#pragma region GraphicsPipeline
    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
    //���_�V�F�[�_�[�A�W�I���g���V�F�[�_�[�A�s�N�Z���V�F�[�_�[���p�C�v���C���ɐݒ�
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(obj_VS.Get());
    gpipeline.GS = CD3DX12_SHADER_BYTECODE(obj_GS.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(obj_PS.Get());

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
    gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

    //�u�����h�X�e�[�g�̐ݒ�
    D3D12_RENDER_TARGET_BLEND_DESC &blenddesc = gpipeline.BlendState.RenderTarget[0];//blenddesc�������������RenderTarget[0]�����������
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//�W���ݒ�

    //�u�����h�X�e�[�g�̋��ʐݒ�
    blenddesc.BlendEnable = true;//�u�����h�L��
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z����
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g�p
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g�p

    //�����ݒ�(�e���ڂ����������邱�ƂŐݒ�\)
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̒l��100%�g�p
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//�f�X�g�̒l��100%�g�p

    //�f�v�X�X�e���V���X�e�[�g�ݒ�
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; //�[�x�l�t�H�[�}�b�g

    //���_���C�A�E�g�̐ݒ�
    gpipeline.InputLayout.pInputElementDescs = inputLayout;
    gpipeline.InputLayout.NumElements = _countof(inputLayout);

    //�}�`�̌`����O�p�`�ɐݒ�
    gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    //���̑�
    gpipeline.NumRenderTargets = 1;//�`��Ώۂ�1��
    gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0~255�w���RGBA
    gpipeline.SampleDesc.Count = 1;//1px�ɂ�1��T���v�����O

    CD3DX12_DESCRIPTOR_RANGE descRangeSRV{};
    descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    //���[�g�p�����[�^�[�̐ݒ�
    //�萔�p
    CD3DX12_ROOT_PARAMETER rootparams[3] = {};
    rootparams[0].InitAsConstantBufferView(0);//�萔�o�b�t�@�p
    rootparams[1].InitAsConstantBufferView(1);
    //�e�N�X�`���p
    rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);//�W��


    //�e�N�X�`���T���v���[�ݒ�
    D3D12_STATIC_SAMPLER_DESC samplerDesc{};

    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    Pipeline3D pipelineset;

    //���[�g�V�O�l�`���̐���
    D3D12_ROOT_SIGNATURE_DESC rootsignatureDesc{};
    rootsignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootsignatureDesc.pParameters = rootparams;//���[�g�p�����[�^�[�̐擪�A�h���X
    rootsignatureDesc.NumParameters = _countof(rootparams);//���[�g�p�����[�^��
    rootsignatureDesc.pStaticSamplers = &samplerDesc;
    rootsignatureDesc.NumStaticSamplers = 1;
    ComPtr<ID3DBlob> rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootsignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineset.rootsignature));

    //�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
    gpipeline.pRootSignature = pipelineset.rootsignature.Get();

    //�p�C�v���C���X�e�[�g
    result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineset.pipelinestate));
#pragma endregion GraphicsPipeline

    //�f�X�N���v�^�q�[�v����
    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NumDescriptors = 256;
    result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descheap));

    return pipelineset;
}


void NY_Object3DManager::LoadObject3DTexture(UINT &texNumber, string filename, ID3D12Device *dev)
{
    //�Q�ƈ����o�b�t�@�����łɃe�N�X�`���̓ǂݍ��ݍρi�d�����[�h�h�~�j
    //if (texbuff[texNumber] != nullptr) {
    //    return;
    //}

    for (int i = 0; i < _countof(texbuff); i++) {
        if (texbuff[i] == nullptr) {
            //���U���g
            HRESULT result;

            string pathname = filename;
            //���j�R�[�h������ɕϊ�
            wchar_t wfilename[128];
            int iBufferSize = MultiByteToWideChar(CP_ACP, 0, pathname.c_str(), -1, wfilename, _countof(wfilename));

            //WIC�e�N�X�`���̃��[�h
            TexMetadata metadata{};
            ScratchImage scratchImg{};
            //�ǂݍ���
            result = LoadFromWICFile(wfilename,
                WIC_FLAGS_NONE,
                &metadata, scratchImg
            );
            const Image *img = scratchImg.GetImage(0, 0, 0);

            // �e�N�X�`���o�b�t�@����
            D3D12_HEAP_PROPERTIES texHeapProp{};//�e�N�X�`���q�[�v�ݒ�
            texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
            texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
            texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

            D3D12_RESOURCE_DESC texresDesc{};//���\�[�X�ݒ�
            texresDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);//2D�e�N�X�`���p
            texresDesc.Format = metadata.format;//RGBA�t�H�[�}�b�g
            texresDesc.Width = metadata.width;//��
            texresDesc.Height = (UINT)metadata.height;//�c
            texresDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
            texresDesc.MipLevels = (UINT16)metadata.mipLevels;
            texresDesc.SampleDesc.Count = 1;

            result = dev->CreateCommittedResource(//GPU���\�[�X����
                &texHeapProp,
                D3D12_HEAP_FLAG_NONE,
                &texresDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&texbuff[i])
            );

            //�e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
            result = texbuff[i]->WriteToSubresource(
                0,
                nullptr,
                img->pixels,
                (UINT)img->rowPitch,
                (UINT)img->slicePitch
            );

            //�V�F�[�_�[���\�[�X�r���[�ݒ�
            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
            srvDesc.Format = metadata.format;
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = 1;

            //�q�[�v�ɃV�F�[�_�[���\�[�X�r���[�쐬
            D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = descheap.Get()->GetCPUDescriptorHandleForHeapStart();
            D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = descheap.Get()->GetGPUDescriptorHandleForHeapStart();
            
            dev->CreateShaderResourceView(texbuff[i].Get(), &srvDesc,
                 CD3DX12_CPU_DESCRIPTOR_HANDLE(descheap.Get()->GetCPUDescriptorHandleForHeapStart(), i,
                 dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
            
            texNumber = i;//�o�b�t�@�ԍ��ԋp
            break;//���[�v���甲����
        }
    }

}


void NY_Object3DManager::SetCamera(NY_Camera *cam)
{
    this->cam = cam;
}


Object3d *NY_Object3DManager::CreateObject3d(NY_Model3D *modelData)
{
    //Object3d�̃f�[�^��V���ɍ쐬
    Object3d *newobj = new Object3d;

    //���������I�u�W�F�N�g�̃f�[�^��������
    newobj->InitObject3D(dev);

    //���f���f�[�^���Z�b�g
    newobj->SetLoadedModelData(modelData);

    //�����������I�u�W�F�N�g���R���e�i�Ɋi�[
    objects.push_back(newobj);

    //���������I�u�W�F�N�g��ԋp
    return newobj;
}


void NY_Object3DManager::DeleteObject3d(Object3d *obj)
{

    for (int i = 0; i < objects.size(); i++) {
        //�����I�u�W�F�N�g�Ɠ����I�u�W�F�N�g�����o
        if (obj == objects[i]) {
            objects.erase(objects.begin() + i);
            break;
        }
    }

    //���������
    objects.shrink_to_fit();

    //�I�u�W�F�N�g�̏���
    delete obj;
    obj = nullptr;

}

void NY_Object3DManager::UpdateAllObjects()
{
    //���ׂẴI�u�W�F�N�g���X�V����
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->UpdateObject3D(cam);
    }
}


void NY_Object3DManager::SetCommonBeginDrawObject3D(ID3D12GraphicsCommandList *cmd)
{
    //�p�C�v���C���X�e�[�g���Z�b�g
    cmd->SetPipelineState(object3dPipelineSet.pipelinestate.Get());
    //���[�g�V�O�l�`�����Z�b�g
    cmd->SetGraphicsRootSignature(object3dPipelineSet.rootsignature.Get());
    //�v���~�e�B�u�`��ݒ�
    cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //�f�X�N���v�^�q�[�v�ݒ�
    ID3D12DescriptorHeap *ppHeaps[] = { TexManager::texDsvHeap.Get() };
    cmd->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

Object3d *CreateObject3d(NY_Model3D *modelData, RVector3 pos)
{
    //�ԋp�p�̃|�C���^�ϐ���Object3D���쐬
    Object3d *result = NY_Object3DManager::Get()->CreateObject3d(modelData);

    //�ʒu��ݒ�
    result->position = pos;

    //��������ԋp
    return result;
}


void DrawObject3d(Object3d *obj)
{
    //�`�揀��
    NY_Object3DManager::Get()->SetCommonBeginDrawObject3D(Raki_DX12B::Get()->GetGCommandList());
    //�I�u�W�F�N�g�`��
    obj->DrawModel3D(Raki_DX12B::Get()->GetGCommandList(), Raki_DX12B::Get()->GetDevice());
}
