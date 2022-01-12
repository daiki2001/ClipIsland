#pragma once
#include <Windows.h>

#include "imgui/imgui_impl_win32.h"

//imgui�֐��v���g�^�C�v�錾
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Raki_WinAPI
{
public:
	//�E�B���h�E�T�C�Y
	static int window_width; //�c��
	static int window_height;//����
	static wchar_t window_name[];//�E�B���h�E�̖��O
public:
	//---------------------�����o�֐�--------------------//

	//�ÓI�����o�֐�

	/// <summary>
	/// �E�B���h�E�v���V�[�W��
	/// </summary>
	/// <param name="hwnd">�E�B���h�E�n���h��</param>
	/// <param name="msg">���b�Z�[�W</param>
	/// <param name="wparam">���b�Z�[�W1</param>
	/// <param name="lparam">���b�Z�[�W2</param>
	/// <returns>�E�B���h�E�����̐���</returns>
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//�����o�֐�
	
	/// <summary>
	/// �Q�[���E�B���h�E�̐���
	/// </summary>
	void CreateGameWindow();

	/// <summary>
	/// �Q�[���E�B���h�E�̔j��
	/// </summary>
	void DeleteGameWindow();

	/// <summary>
	/// ���b�Z�[�W�����i�I�����ǂ����j
	/// </summary>
	bool ProcessMessage();

	/// <summary>
	/// �E�B���h�E�n���h���擾
	/// </summary>
	static HWND GetHWND() { return hwnd; }

	static WNDCLASSEX GetWNDCLASSEX() { return wndClass; }

	/// <summary>
	/// �E�B���h�E�N���X�̃C���X�^���X�擾
	/// </summary>
	HINSTANCE GetInstance() { return wndClass.hInstance; }

private:
	//�v���C�x�[�g�����o�ϐ�
	static HWND hwnd;  //�E�B���h�E�n���h��
	static WNDCLASSEX wndClass;//�E�B���h�E�N���X

};

