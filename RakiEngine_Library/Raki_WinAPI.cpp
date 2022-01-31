#include "Raki_WinAPI.h"

int Raki_WinAPI::window_width = 1280;
int Raki_WinAPI::window_height = 720;
wchar_t Raki_WinAPI::window_name[] = L"Clip Island";

HWND Raki_WinAPI::hwnd;
WNDCLASSEX Raki_WinAPI::wndClass;

LRESULT Raki_WinAPI::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // ���b�Z�[�W�ŕ���
    switch (msg) {
    case WM_DESTROY: // �E�B���h�E���j�����ꂽ
        PostQuitMessage(0); // OS�ɑ΂��āA�A�v���̏I����`����
        return 0;
    }
	//imgui�p�̏���
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);

    return DefWindowProc(hwnd, msg, wparam, lparam); // �W���̏������s��
}

void Raki_WinAPI::CreateGameWindow()
{
	// �E�B���h�E�N���X�̐ݒ�
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = (WNDPROC)WindowProcedure; // �E�B���h�E�v���V�[�W��
	wndClass.lpszClassName = window_name;            // �E�B���h�E�N���X��
	wndClass.hInstance = GetModuleHandle(nullptr);   // �E�B���h�E�n���h��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);  // �J�[�\���w��

	RegisterClassEx(&wndClass); // �E�B���h�E�N���X��OS�ɓo�^

	// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc = { 0, 0, window_width, window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y�␳

	// �E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(wndClass.lpszClassName, // �N���X��
		window_name,			// �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	// �^�C�g���o�[�Ƌ��E��������E�B���h�E
		CW_USEDEFAULT,			// �\��X���W�iOS�ɔC����j
		CW_USEDEFAULT,			// �\��Y���W�iOS�ɔC����j
		wrc.right - wrc.left,	// �E�B���h�E����
		wrc.bottom - wrc.top,	// �E�B���h�E�c��
		nullptr,				// �e�E�B���h�E�n���h��
		nullptr,				// ���j���[�n���h��
		wndClass.hInstance,			// �Ăяo���A�v���P�[�V�����n���h��
		nullptr);				// �I�v�V����

	// �E�B���h�E�\��
	ShowWindow(hwnd, SW_SHOW);
}

void Raki_WinAPI::DeleteGameWindow()
{
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
}

bool Raki_WinAPI::ProcessMessage()
{
	MSG msg{};	// ���b�Z�[�W

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // ���b�Z�[�W������H
	{
		TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg); // �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	if (msg.message == WM_QUIT) // �I�����b�Z�[�W�������烋�[�v�𔲂���
	{
		return true;
	}

	return false;
}
