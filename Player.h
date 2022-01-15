#pragma once
#include "RVector.h"
#include "NY_Object3DMgr.h"

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
private:
	NY_Model3D model; //�v���C���[�̃��f��
	Object3d* object; //�v���C���[�̃I�u�W�F�N�g

};
