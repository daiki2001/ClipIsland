#pragma once
#include "RVector.h"
#include "NY_Object3DMgr.h"
#include <DirectXMath.h>

class Warp
{
public:
	Warp();
	Warp(const Warp& other);
	~Warp();

	// �`��p�I�u�W�F�N�g�̐���
	void CreateObj(const RVector3& pos);
	// �`��
	void Draw();
	// �I�u�W�F�N�g�̍폜
	void DeleteObject();

	// ���[�v�Q�[�g���J��
	void WarpOpen(const RVector3& forwordVec);

	// �I�u�W�F�N�g�̍��W�̎擾
	inline RVector3& GetObjectPos() { return object->position; }
	// �I�u�W�F�N�g�̍��W�̐ݒ�
	inline void SetObjectPos(const RVector3& pos) { object->position = pos; }
	// �I�u�W�F�N�g�̐F�̐ݒ�
	inline void SetWarpOpenColor(const DirectX::XMFLOAT4& color) { openColor = color; object->color = openColor; }
	// �I�u�W�F�N�g�̐F�̐ݒ�
	inline void SetWarpCloseColor(const DirectX::XMFLOAT4& color) { closeColor = color; object->color = closeColor; }

public:
	RVector3 forwordVec; //���[�v���J���Ă������
	bool isOpen; //���[�v���J���Ă��邩�ǂ���

	int blockNumber;     //�u���b�N�̔ԍ�
	int twinBlockNumber; //�΂ɂȂ�u���b�N�̔ԍ�
	size_t gateNumber;   //���Ԗڂ̃Q�[�g���ǂ���
private:
	DirectX::XMFLOAT4 openColor;  //���[�v���J���Ă��鎞�̐F
	DirectX::XMFLOAT4 closeColor; //���[�v�������Ă��鎞�̐F

	static UINT warpGraph;       //���[�v�̃e�N�X�`��
	static NY_Model3D warpModel; //���[�v�̃��f��

	Object3d* object; //���[�v�̃I�u�W�F�N�g
};
