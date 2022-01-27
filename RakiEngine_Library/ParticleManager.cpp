#include <d3dcompiler.h>

#include "ParticleManager.h"
#include "Raki_DX12B.h"
#include "TexManager.h"

#pragma comment(lib,"d3dcompiler.lib")

const int ParticleManager::MAX_VERTEX;

const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4 &lhs, const DirectX::XMFLOAT4 &rhs) {
	XMFLOAT4 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	result.w = lhs.w + rhs.w;
	return result;
}

static void operator+=(DirectX::XMFLOAT4 &lhs, const DirectX::XMFLOAT4 &rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	lhs.w += rhs.w;
}

const DirectX::XMFLOAT4 operator-(DirectX::XMFLOAT4 &lhs, const DirectX::XMFLOAT4 &rhs) {
	XMFLOAT4 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	result.w = lhs.w - rhs.w;
	return result;
}

const DirectX::XMFLOAT4 operator/(const DirectX::XMFLOAT4 &lhs, const float a) {
	XMFLOAT4 result;
	result.x = lhs.x / a;
	result.y = lhs.y / a;
	result.z = lhs.z / a;
	result.w = lhs.w / a;
	return result;
}

const DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4 &lhs, const float a) {
	XMFLOAT4 result;
	result.x = lhs.x * a;
	result.y = lhs.y * a;
	result.z = lhs.z * a;
	result.w = lhs.w * a;
	return result;
}


ParticleManager *ParticleManager::Create() {

	//�p�[�e�B�N���}�l�[�W���[����
	ParticleManager *pm = new ParticleManager(
		Raki_DX12B::Get()->GetDevice(),
		Raki_DX12B::Get()->GetGCommandList()
	);

	//�����������̂�������
	pm->Initialize();

	return pm;
}

void ParticleManager::Initialize() {
	//null�`�F�b�N

	HRESULT result;

	//�p�C�v���C��������
	InitializeGraphicsPipeline();

	//�p�[�e�B�N���p���f���쐬
	CreateModel();

	//�萔�o�b�t�@����
	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resdesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);
	result = dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);
	if (FAILED(result)) {
		assert(0);
	}
}

void ParticleManager::Update() {

	//�������؂ꂽ�p�[�e�B�N�����폜
	grains.remove_if([](Particle &p) {return p.nowFrame >= p.endFrame; });

	//�S�p�[�e�B�N���X�V
	for (std::forward_list<Particle>::iterator itr = grains.begin();
		itr != grains.end(); itr++) {

		//�t���[�����J�E���g
		itr->nowFrame++;

		//�i�s�x��0~1�̊�����
		float rate = (float)itr->nowFrame / itr->endFrame;

		//���x���Z
		itr->vel = itr->vel + itr->acc;

		//���x�ɂ��ړ�
		itr->pos += itr->vel;

		//�F���`���
		itr->color = itr->s_color + (itr->e_color - itr->s_color) * rate;

		//�X�P�[�����O�̐��`���
		itr->scale = itr->s_scale + (itr->e_scale - itr->s_scale) * rate;

		//��]���`���
		itr->rot = itr->s_rotation + (itr->e_rotation - itr->s_rotation) / rate;
	}

	//���_�o�b�t�@�f�[�^�]��
	int vcount = 0;
	PVertex *vertMap = nullptr;
	result = vertbuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		// �p�[�e�B�N���̏���1�����f
		for (std::forward_list<Particle>::iterator it = grains.begin();
			it != grains.end();
			it++) {
			// ���W
			vertMap->pos = it->pos;
			// �X�P�[��
			vertMap->scale = it->scale;
			//�F
			vertMap->color = it->color;
			// ���̒��_��
			vertMap++;
			if (++vcount >= MAX_VERTEX) {
				break;
			}
		}
		vertbuff->Unmap(0, nullptr);
	}

	//�萔�o�b�t�@�f�[�^�]��
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	if (result == S_OK) {
		//�r���[�v���W�F�N�V�����s��
		constMap->mat = camera->GetMatrixViewProjection();
		//�S�����r���{�[�h
		constMap->matBillBoard = camera->GetMatrixBillBoardAll();
		//�F
		constBuff->Unmap(0, nullptr);
	}

}

void ParticleManager::Draw(UINT drawTexNum)
{
	UINT drawNum = (UINT)std::distance(grains.begin(), grains.end());
	if (drawNum > MAX_VERTEX) {
		drawNum = MAX_VERTEX;
	}

	// �p�[�e�B�N����1���Ȃ��ꍇ
	if (drawNum == 0) {
		return;
	}

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmd->SetPipelineState(pipeline.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmd->SetGraphicsRootSignature(rootsig.Get());
	// �v���~�e�B�u�`���ݒ�
	cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// ���_�o�b�t�@�̐ݒ�
	cmd->IASetVertexBuffers(0, 1, &vbview);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap *ppHeaps[] = { TexManager::texDsvHeap.Get() };
	cmd->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmd->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmd->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(TexManager::texDsvHeap.Get()->GetGPUDescriptorHandleForHeapStart(),
			drawTexNum, Raki_DX12B::Get()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	// �`��R�}���h
	cmd->DrawInstanced(drawNum, 1, 0, 0);
}

void ParticleManager::Add(int life, RVector3 pos, RVector3 vel, RVector3 acc, 
	float startScale, float endScale, XMFLOAT4 s_color, XMFLOAT4 e_color)
{
	//�v�f�ǉ�
	grains.emplace_front();
	//�ǉ������v�f�̎Q��
	Particle &p = grains.front();
	p.pos = pos;			//�����ʒu
	p.vel = vel;			//���x
	p.acc = acc;			//�����x
	p.s_scale = startScale; //�J�n���̃X�P�[���T�C�Y
	p.e_scale = endScale;	//�I�����̃X�P�[���T�C�Y
	p.endFrame = life;		//��������
	p.s_color = s_color;
	p.e_color = e_color;
}



void ParticleManager::InitializeGraphicsPipeline() {

	result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob;	// �W�I���g���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleVS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticlePS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleGS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "gs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �X�P�[��
			"TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	// �f�v�X�̏������݂��֎~
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	// ���Z�u�����f�B���O
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̒l��100%�g�p
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//�f�X�g�̒l��100%�g�p
	//// ���Z�u�����f�B���O
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//blenddesc.DestBlend = D3D12_BLEND_ONE;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�_�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsig));
	if (FAILED(result)) {
		assert(0);
	}

	gpipeline.pRootSignature = rootsig.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = Raki_DX12B::Get()->GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipeline));

	if (FAILED(result)) {
		assert(0);
	}
}

void ParticleManager::CreateModel() {
	HRESULT result = S_FALSE;

	// ���_�o�b�t�@����
	auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto res = CD3DX12_RESOURCE_DESC::Buffer(sizeof(PVertex) * MAX_VERTEX);
	result = dev->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&res,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertbuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	//// ���_�o�b�t�@�r���[�̍쐬
	vbview.BufferLocation = vertbuff->GetGPUVirtualAddress();
	vbview.SizeInBytes = sizeof(PVertex) * MAX_VERTEX;
	vbview.StrideInBytes = sizeof(PVertex);


}