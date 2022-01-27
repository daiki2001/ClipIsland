#pragma once
#include "RVector.h"
#include "NY_Object3DMgr.h"
#include <DirectXMath.h>

class Warp
{
public:
	Warp();
	~Warp();

	// �`��p�I�u�W�F�N�g�̐���
	void CreateObj(RVector3 pos);
	// �`��
	void Draw();

	// ���[�v�Q�[�g���J��
	void WarpOpen(const RVector3 pos, const RVector3& forwordVec);

	// �I�u�W�F�N�g�̍��W�̎擾
	inline RVector3& GetObjectPos() { return object->position; }
	// �I�u�W�F�N�g�̍��W�̐ݒ�
	inline void SetObjectPos(const RVector3& pos) { object->position = pos; }
	// �I�u�W�F�N�g�̐F�̐ݒ�
	inline void SetWarpOpenColor(const DirectX::XMFLOAT4& color) { openColor = color; }
	// �I�u�W�F�N�g�̐F�̐ݒ�
	inline void SetWarpCloseColor(const DirectX::XMFLOAT4& color) { closeColor = color; }

public:
	RVector3 forwordVec; //���[�v���J���Ă������

	bool isOpen; //���[�v���J���Ă��邩�ǂ���

	int blockNumber; //�u���b�N�̔ԍ�
private:
	DirectX::XMFLOAT4 openColor;  //���[�v���J���Ă��鎞�̐F
	DirectX::XMFLOAT4 closeColor; //���[�v�������Ă��鎞�̐F

	NY_Model3D boxModel; //���[�v���ݒu�o���锠�̃��f��

	UINT warpGraph;       //���[�v�̃e�N�X�`��
	NY_Model3D warpModel; //���[�v�̃��f��

	Object3d* object;    //���[�v�̃I�u�W�F�N�g
};
