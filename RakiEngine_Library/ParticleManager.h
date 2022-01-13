#pragma once

#include <windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>

#include "NY_Camera.h"
#include "RVector.h"

class ParticleManager
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// ���_�f�[�^
	struct PVertex {
		RVector3 pos;
		float scale;
	};

	//�萔�o�b�t�@�f�[�^�\����
	struct ConstBufferData {
		XMMATRIX mat;
		XMMATRIX matBillBoard;
	};

	// �p�[�e�B�N��1���N���X
	class Particle {
		// Microsoft::WRL::���ȗ�
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::���ȗ�
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;

	public:
		//���W
		RVector3 pos;
		//���x
		RVector3 vel;
		//�����x
		RVector3 acc;
		//�F
		RVector3 color;
		//�X�P�[�����O
		float scale;
		//��]
		float rot;
		//�����l
		XMFLOAT3 s_color = {};
		float s_scale = 1.0f;
		float s_rotation = 0.0f;
		// �ŏI�l
		XMFLOAT3 e_color = {};
		float e_scale = 0.0f;
		float e_rotation = 0.0f;
		// ���݃t���[��
		int nowFrame = 0;
		// �I���t���[��
		int endFrame = 0;
	};

private:
	// �ő吶�����_��
	static const int MAX_VERTEX = 65536;
	// �f�o�C�X
	ID3D12Device *dev;

public:
	/// <summary>
	/// �p�[�e�B�N���}�l�[�W���[����
	/// </summary>
	/// <param name="camera">NY_Camera�̃C���X�^���X</param>
	/// <returns>��������ParticleManager�̃C���X�^���X</returns>
	static ParticleManager *Create(NY_Camera *camera);

	/// <summary>
	/// �p�[�e�B�N���}�l�[�W���[������
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// �S�p�[�e�B�N���X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �p�[�e�B�N���`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �p�[�e�B�N���ǉ�
	/// </summary>
	void Add(int life, RVector3 pos, RVector3 vel, RVector3 acc, float startScale, float endScale);

private:


};

