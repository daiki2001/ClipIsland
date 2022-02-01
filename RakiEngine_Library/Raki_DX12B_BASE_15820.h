#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <dxgi1_6.h>
#include <cstdlib>

#include "Raki_WinAPI.h"
#include "Raki_Input.h"

class Raki_DX12B
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	Raki_DX12B() {};
	~Raki_DX12B() {
	
	};

public:
	//--------------------�����o�֐�--------------------//

	//�R�s�[�R���X�g���N�^�A������Z�q������
	Raki_DX12B &operator=(const Raki_DX12B &obj) = delete;
	Raki_DX12B(const Raki_DX12B &obj) = delete;

	static Raki_DX12B *Get() {
		static Raki_DX12B dx12b;
		return &dx12b;
	}

	/// <summary>
	/// DirectX12�̈ꊇ������
	/// </summary>
	void Initialize(Raki_WinAPI *win);

	/// <summary>
	/// �`��J�n
	/// </summary>
	void StartDraw();

	/// <summary>
	/// �`��I��
	/// </summary>
	void EndDraw();

	/// <summary>
	/// �����_�[�^�[�Q�b�g�̃N���A
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// �[�x�o�b�t�@�[�̃N���A
	/// </summary>
	void ClearDepthBuffer();

	/// <summary>
	/// ID3D12GraphicsCommandList�̃Q�b�^
	/// </summary>
	/// <returns>ID3D12GraphicsCommandList�̃|�C���^</returns>
	ID3D12GraphicsCommandList *GetGCommandList() { return commandList.Get(); }

	/// <summary>
	/// ID3D12Device�̃Q�b�^
	/// </summary>
	/// <returns></returns>
	ID3D12Device *GetDevice() { return device.Get(); }

	/// <summary>
	/// imgui�p�f�X�N���v�^�q�[�v�̃Q�b�^
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12DescriptorHeap> GetDsvHeapForImgui() { return dsvHeapForImgui; }


	void ManualRelease() {
		dxgiFactory.ReleaseAndGetAddressOf();
		device.ReleaseAndGetAddressOf();
		commandAllocator.ReleaseAndGetAddressOf();
		commandList.ReleaseAndGetAddressOf();
		commandQueue.ReleaseAndGetAddressOf();
		swapchain.ReleaseAndGetAddressOf();
		depthBuffer.ReleaseAndGetAddressOf();
		rtvHeaps.ReleaseAndGetAddressOf();
		dsvHeap.ReleaseAndGetAddressOf();
		fence.ReleaseAndGetAddressOf();
	}

private:
	//DirectX12�v�f�̃����o�ϐ�

	// �E�B���h�E�Y�A�v���P�[�V�����Ǘ�
	Raki_WinAPI *winApp = nullptr;

	// Direct3D�֘A
	ComPtr<IDXGIFactory6>				dxgiFactory;
	ComPtr<ID3D12Device>				device;
	ComPtr<ID3D12GraphicsCommandList>	commandList;
	ComPtr<ID3D12CommandAllocator>		commandAllocator;
	ComPtr<ID3D12CommandQueue>			commandQueue;
	ComPtr<IDXGISwapChain4>				swapchain;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource>				depthBuffer;
	ComPtr<ID3D12DescriptorHeap>		rtvHeaps;
	ComPtr<ID3D12DescriptorHeap>		dsvHeap;
	ComPtr<ID3D12Fence>					fence;
	UINT64								fenceVal = 0;

	//imgui�֘A
	ComPtr<ID3D12DescriptorHeap>		dsvHeapForImgui;

private:
	//DirectX12�̏������֐��Q

	/// <summary>
	/// DXGI�f�o�C�X������
	/// </summary>
	/// <returns>����</returns>
	bool InitDXGIDevice();

	/// <summary>
	/// �R�}���h�֘A�̐���
	/// </summary>
	/// <returns>����</returns>
	bool CreateCommand();

	/// <summary>
	/// �X���b�v�`�F�[������
	/// </summary>
	/// <returns>����</returns>
	bool CreateSwapChain();

	/// <summary>
	/// �����_�[�^�[�Q�b�g�r���[����
	/// </summary>
	/// <returns>����</returns>
	bool CreateRenderTargetView();

	/// <summary>
	/// �[�x�o�b�t�@�[����
	/// </summary>
	/// <returns>����</returns>
	bool CreateDepthBuffer();

	/// <summary>
	/// �t�F���X����
	/// </summary>
	/// <returns>����</returns>
	bool CreateFence();

	/// <summary>
	/// ���͌n������
	/// </summary>
	bool InitInput(Raki_WinAPI *win);

	/// <summary>
	/// imgui�p�f�X�N���v�^�q�[�v
	/// </summary>
	/// <returns></returns>
	bool CreateDsvHeapForIngui();

};

