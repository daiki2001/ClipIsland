#pragma once

#include <Sprite.h>
#include <RVector.h>

class SceneChangeDirection final
{
private:
	//���ރe�N�X�`��
	UINT tex1;
	UINT tex2;
	//���ރX�v���C�g
	Sprite press1, press2;
	//���݊J�n�_�A�I���_
	RVector3 ps1s, ps1e;
	RVector3 ps2s, ps2e;
	//�t���[���J�E���g�A���o�ő�t���[����
	int frameCount;
	const int DIR_FRAME = 60;
	//�ǂ���̉��o��
	bool isStart = false;
	bool isEnd = false;
	//�R���X�g���N�^�A�f�X�g���N�^
	SceneChangeDirection();
	~SceneChangeDirection();

public:
	//�Q�b�^
	static SceneChangeDirection *get() {
		static SceneChangeDirection ins;
		return &ins;
	}
	//���o�I���t���O
	bool isEndDir = false;

	//������
	void Init();
	//�J�n���o�J�n
	void Start_StartDir();
	//�I�����o�J�n
	void Start_EndDir();
	//�X�V
	void Update();
	//�`��
	void Draw();
};