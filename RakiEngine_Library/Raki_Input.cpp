#include "Raki_Input.h"
#include "Raki_WinAPI.h"

BYTE Input::keys[256]    = {};
BYTE Input::oldkeys[256] = {};
DIMOUSESTATE Input::mouseState;
DIMOUSESTATE Input::oldMouseState;
POINT        Input::pos;

IDirectInput8       *Input::dinput      = nullptr;
IDirectInputDevice8 *Input::devkeyBoard = nullptr;
IDirectInputDevice8 *Input::devMouse	= nullptr;

bool Input::Init(WNDCLASSEX w, HWND hwnd)
{
	HRESULT result;
	//������
	result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);
	if (result != S_OK) { return false; }

	//�L�[�{�[�h�f�o�C�X����
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyBoard, NULL);
	if (result != S_OK) { return false; }
	//�}�E�X�f�o�C�X����
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	if (result != S_OK) { return false; }

	//���̓f�[�^�`���Z�b�g
	result = devkeyBoard->SetDataFormat(&c_dfDIKeyboard);//�W��
	if (result != S_OK) { return false; }
	result = devMouse->SetDataFormat(&c_dfDIMouse);
	if (result != S_OK) { return false; }

	//�r�����䃌�x���Z�b�g
	result = devkeyBoard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (result != S_OK) { return false; }
	result = devMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (result != S_OK) { return false; }

	return true;
}

void Input::StartGetInputState()
{
	//�O�t���[���̓��͏����擾
	for (int i = 0; i < 256; i++) {
		oldkeys[i] = keys[i];
	}
	//�L�[���͏�Ԏ擾�J�n
	devkeyBoard->Acquire();
	//�S�L�[�̓��͏�Ԃ��擾
	devkeyBoard->GetDeviceState(sizeof(keys), keys);

	//�}�E�X�̑O�t���[���̓��͏�Ԃ��擾
	oldMouseState = mouseState;
	//�}�E�X���͏�Ԏ擾�J�n
	devMouse->Acquire();
	//�}�E�X�̓��͏�Ԃ��擾
	devMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
}

bool Input::isKey(int key)
{
	return keys[key];
}

bool Input::isKeyTrigger(int key)
{
	return keys[key] && !oldkeys[key];
}

bool Input::isKeyReleased(int key)
{
	return oldkeys[key] && !keys[key];
}

bool Input::isMouseClicking(int keyCode)
{
	//�}�E�X��������Ă���
	return mouseState.rgbButtons[keyCode] & (0x80);
}

bool Input::isMouseClickTrigger(int keyCode)
{
	BYTE onValue = 0x80;
	//�O�t���[���͉�����Ă��炸�A���t���[���ŉ�����Ă���
	return (mouseState.rgbButtons[keyCode] == onValue) & (oldMouseState.rgbButtons[keyCode] != onValue);
}

bool Input::isMouseClicked(int keyCode)
{
	BYTE onValue = 0x80;
	//�O�t���[���ŉ�����Ă���A���t���[���ŉ�����Ă��Ȃ�
	return (mouseState.rgbButtons[keyCode] != onValue) & (oldMouseState.rgbButtons[keyCode] == onValue);
}

XMFLOAT2 Input::getMousePos()
{
	//�f�X�N�g�b�v���W���擾
	GetCursorPos(&pos);
	//�N���C�A���g�̈���W�ϊ�
	ScreenToClient(Raki_WinAPI::GetHWND(), &pos);
	//�߂�l�Ɋi�[
	XMFLOAT2 result(pos.x, pos.y);
	//�ԋp
	return result;
}

XMFLOAT2 Input::getMouseVelocity()
{
	XMFLOAT2 result;
	result.x = mouseState.lX;
	result.y = mouseState.lY;

	return result;
}

Input *Input::Get()
{
	static Input instance;
	return &instance;
}
