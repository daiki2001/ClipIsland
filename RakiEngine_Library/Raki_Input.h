#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <wrl.h>

#include "RVector.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


#pragma region InputKeyCode
// �L�[�{�[�h�͂��Ȃ�����

// �}�E�X����
#define MOUSE_L			0x00		//�}�E�X��
#define MOUSE_R			0x01		//�}�E�X�E
#define MOUSE_CENTER	0x02		//�}�E�X����

//�Q�[���p�b�h

#pragma endregion InputKeyCode

using namespace Microsoft::WRL;
/// <summary>
/// �V���O���g���p�^�[���̗p�A�L�[���̓N���X
/// </summary>
class Input final
{
private:
	/// �f�o�C�X

	//���̓f�o�C�X
	static IDirectInput8       *dinput      ;
	//�L�[�{�[�h
	static IDirectInputDevice8 *devkeyBoard ;
	//�}�E�X
	static IDirectInputDevice8 *devMouse;


	/// �e����͏��

	//�L�[�{�[�h
	static BYTE keys[256];
	static BYTE oldkeys[256];
	//�}�E�X
	static DIMOUSESTATE mouseState;		//�}�E�X���͏��
	static DIMOUSESTATE oldMouseState;	//1F�O�̃}�E�X���͏��
	static POINT		pos;			//�}�E�X���W

	//�R���X�g���N�^�A�f�X�g���N�^���B��
	Input() {}
	~Input() {}

public:

	/// <summary>
	/// ���͌n������
	/// </summary>
	/// <param name="w">�E�B���h�E�N���X</param>
	/// <param name="hwnd">�E�B���h�E�n���h��</param>
	/// <returns>����</returns>
	bool Init(WNDCLASSEX w, HWND hwnd);

	//���͏��̎擾�J�n
	static void StartGetInputState();

	// ---------- �L�[���� ---------- //

	//�L�[�����͂���Ă��邩�H
	static bool isKey(int key);
	//�g���K�[����
	static bool isKeyTrigger(int key);
	//�����[�X����
	static bool isKeyReleased(int key);

	// ---------- �}�E�X���� ---------- //

	static bool isMouseClicking(int keyCode);

	static bool isMouseClickTrigger(int keyCode);

	static bool isMouseClicked(int keyCode);

	static XMFLOAT2 getMousePos();

	static XMFLOAT2 getMouseVelocity();

	//�C���X�^���X�擾
	static Input *Get();

	//�R�s�[�R���X�g���N�^�A������Z�q������
	Input(const Input &obj) = delete;
	Input &operator=(const Input &obj) = delete;
};

//�}�E�X���́i���b�v�֐��j
namespace mouse {



}

//�Q�[���p�b�h����
namespace pad {




}
