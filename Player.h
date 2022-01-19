#pragma once
#include "RVector.h"
#include "NY_Object3DMgr.h"
#include "Collision.h"

class Player
{
public:
	Player();
	~Player();

	// �X�V
	void Update();
	// �`��
	void Draw();

public:
	RVector3 position;   //�v���C���[�̍��W
	RVector3 forwardVec; //�i�s����
	RVector3 playerRot;	 //�v���C���[�̉�]�p

	Collision playerCollision;

	const RVector3 offsetPos = RVector3(0, 0, -20);	 //�`�悸���␳���邽�߂ɉ��Z����
	Object3d *object; //�v���C���[�̃I�u�W�F�N�g
private:
	NY_Model3D model; //�v���C���[�̃��f��
	

};
