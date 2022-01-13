#pragma once
#include <wrl.h>

#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui.h"

class ImguiMgr final
{
private:
	//imgui�p�f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeapForImgui;

	//�R���X�g���N�^�A�f�X�g���N�^�𖳌�
	ImguiMgr() {};
	~ImguiMgr() {};

public:
	//imguiMgr�̃Q�b�^
	static ImguiMgr *Get() {
		static ImguiMgr obj;
		return &obj;
	}

	/// <summary>
	/// imgui�̏������Aimgui�p�f�X�N���v�^�q�[�v�쐬
	/// </summary>
	/// <param name="dev">ID3D12Device�̃|�C���^</param>
	/// <returns>bool�^�Ő���</returns>
	bool InitImgui(ID3D12Device *dev, HWND hwnd);

	void StartDrawImgui(const char *windowTitle, int posX, int posY);

	void EndDrawImgui();

	void FinalizeImgui();
};

namespace myImgui {
	/// <summary>
	/// imgui�̏������Aimgui�p�f�X�N���v�^�q�[�v�쐬
	/// Raki_DX12B�̏����������Ŏ��s���邽�ߕs�v�H
	/// </summary>
	/// <param name="dev">ID3D12Device�̃|�C���^</param>
	/// <returns>bool�^�Ő���</returns>
	bool InitializeImGui(ID3D12Device *dev, HWND hwnd);

	/// <summary>
	/// Imgui�̃E�B���h�E���쐬�A�`��J�n�B�E�B���h�E�ɕ\��������e�͂��̊֐��̎��s��艺�ɏ�������
	/// </summary>
	/// <param name="windowTitle">�\������imgui�E�B���h�E�̃^�C�g��</param>
	/// <param name="posX">�����̉���</param>
	/// <param name="posY">�����̏c��</param>
	void StartDrawImGui(const char *windowTitle, int sizeX, int sizeY);

	/// <summary>
	/// Imgui�̃E�B���h�E�`��̏I���B�E�B���h�E�ɕ\��������e�͂��̊֐��̎��s����ɏ������ƁB
	/// </summary>
	void EndDrawImGui();

	/// <summary>
	/// imgui�̏I�������B�A�v���P�[�V�����̏I���^�C�~���O�Ŏ��s���邱��
	/// </summary>
	void FinalizeImGui();
}