#include "SpriteManager.h"
#include "TexManager.h"

void SpriteManager::CreateSpriteManager(ID3D12Device *dev, ID3D12GraphicsCommandList *cmd, int window_w, int window_h)
{
    //�E�B���h�E�T�C�Y�ݒ�
    window_width = window_width;
    window_height = window_height;
    //�r���[�|�[�g�s�񏉊���
    matViewport.r[0].m128_f32[0] = window_width / 2;
    matViewport.r[1].m128_f32[1] = -window_height / 2;
    matViewport.r[3].m128_f32[0] = window_width / 2;
    matViewport.r[3].m128_f32[1] = window_height / 2;
    //�f�o�C�X�ƃR�}���h���X�g�̃|�C���^���i�[
    this->dev = dev;
    this->cmd = cmd;
    //�p�C�v���C������
    CreateSpritePipeline();
    matProjection = XMMatrixOrthographicOffCenterLH(0.0f, (float)this->window_width, (float)this->window_height, 0.0f, 0.0f, 1.0f);
}

void SpriteManager::CreateSpritePipeline()
{
    HRESULT result;

    //-----�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��-----//
#pragma region LoadShader
    ComPtr<ID3DBlob> vsBlob = nullptr; //���_�V�F�[�_�[�I�u�W�F�N�g
    ComPtr<ID3DBlob> psBlob = nullptr; //�s�N�Z���V�F�[�_�[�I�u�W�F�N�g
    ID3DBlob *errorBlob = nullptr; //�G���[�I�u�W�F�N�g

    //���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
    result = D3DCompileFromFile(
        L"./RakiEngine_Library/Shaders/SpriteVS.hlsl", //�V�F�[�_�[�t�@�C����
        nullptr,//�V�F�[�_�[�}�N���I�u�W�F�N�g�i����͎g��Ȃ��j
        D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�I�u�W�F�N�g�i�C���N���[�h�\�ɂ���j
        "main", "vs_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�ݒ�
        0,
        &vsBlob, &errorBlob
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
        L"./RakiEngine_Library/Shaders/SpritePS.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &psBlob, &errorBlob
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
        {//uv���W
            "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
        },
    };

    //-----�O���t�B�b�N�X�p�C�v���C���̃Z�b�g-----//

    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
    //���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[���p�C�v���C���ɐݒ�
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

    //�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
    gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�
    gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;


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
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);//��x�\���ݒ�
    gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;//��ɏ㏑��
    gpipeline.DepthStencilState.DepthEnable = false;//�[�x�e�X�g����
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

    CD3DX12_ROOT_PARAMETER rootparam[2];
    rootparam[0].InitAsConstantBufferView(0);
    rootparam[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

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

    //���[�g�V�O�l�`���̐���
    D3D12_ROOT_SIGNATURE_DESC rootsignatureDesc{};
    rootsignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootsignatureDesc.pParameters = rootparam;//���[�g�p�����[�^�[�̐擪�A�h���X
    rootsignatureDesc.NumParameters = _countof(rootparam);//���[�g�p�����[�^��
    rootsignatureDesc.pStaticSamplers = &samplerDesc;
    rootsignatureDesc.NumStaticSamplers = 1;

    ComPtr<ID3DBlob> rootSigBlob = nullptr;
    result = D3D12SerializeRootSignature(&rootsignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));

    //�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
    gpipeline.pRootSignature = rootsignature.Get();

    //�p�C�v���C���X�e�[�g
    result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

    //�f�X�N���v�^�q�[�v����
    //D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
    //descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    //descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    //descHeapDesc.NumDescriptors = MAX_TEX_NUM;
    //result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descheap));
}

void SpriteManager::SetCommonBeginDraw()
{
    //�p�C�v���C���X�e�[�g���Z�b�g
    cmd->SetPipelineState(pipelinestate.Get());
    //���[�g�V�O�l�`�����Z�b�g
    cmd->SetGraphicsRootSignature(rootsignature.Get());
    //�v���~�e�B�u�`��ݒ�
    cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    //�f�X�N���v�^�q�[�v�ݒ�
    ID3D12DescriptorHeap *ppHeaps[] = { TexManager::texDsvHeap.Get() };
    cmd->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}
