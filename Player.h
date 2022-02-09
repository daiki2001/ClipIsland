#pragma once
#include "RVector.h"
#include "NY_Object3DMgr.h"
#include "Collision.h"
#include "Audio.h"

#include "ReverseAndResetDir.h"

class Player
{
public:
	Player();
	~Player();

	// �X�V
	void Update();
	// �`��
	void Draw();

	// �����߂�����
	void PushBack();

	// ���Z�b�g
	void Reset();

public:
	RVector3 position;   //�v���C���[�̍��W
	RVector3 playerOldPos;   //�v���C���[�̑O���W
	RVector3 forwardVec; //�i�s����
	RVector3 playerRot;	 //�v���C���[�̉�]�p
	bool goalFlag;
	bool moveFlag;
	RVector3 startPos;
	RVector3 endPos;

	float timeRate;

	RVector3 Tposition;
	bool playerFlag;
	bool easeFlag;	//�����Ă���true
	bool stepbackFlag;	//�����Ă���true

	Collision playerCollision;

	const RVector3 offsetPos = RVector3(0, 0, -20); //�`�悸���␳���邽�߂ɉ��Z����
	Object3d *object; //�v���C���[�̃I�u�W�F�N�g

	SoundData walkSE;
private:
	NY_Model3D model; //�v���C���[�̃��f��
};