#pragma once
#include "BlockData.h"
#include "NY_Object3DMgr.h"
#include "RVector.h"

namespace GameCommonData
{
class StageBlockModels final
{
private: // �V���O���g����
	StageBlockModels() {}
	~StageBlockModels() {}
public:
	StageBlockModels(const StageBlockModels&) = delete;
	StageBlockModels operator=(const StageBlockModels&) = delete;

public: // �萔
	static const RVector3 modelRot; //���f���̉�]�p�x
	static const float modelScale; //���f���̃X�P�[�����O�{��

public: // �ÓI�����o�֐�
	// StageBlockModels�̏�����
	static void StaticInitiizer(); //�Q�[�����ɏo�Ă��鋤�ʂ̔��̃��f��

public: // �ÓI�����o�ϐ�
	static NY_Model3D moveModel; //��������n�ʂ̃��f��
	static NY_Model3D stayModel; //�����Ȃ��n�ʂ̃��f��
	static NY_Model3D goalModel; //�S�[���u���b�N�̃��f��
	static NY_Model3D switchOnModel; //�N�������X�C�b�`�̃��f��
	static NY_Model3D switchOffModel; //�N�����ĂȂ��X�C�b�`�̃��f��
	static NY_Model3D doorModel; //�X�C�b�`�ŏ�����u���b�N�̃��f��

	static UINT debugBoxGraph;       //���`��p�̔��̃e�N�X�`��
	static NY_Model3D debugBoxModel; //���`��p�̔��̃��f��
};

// �X�N���[�����W�����[���h���W�ւ̕ϊ�
RVector3 ScreenToWorld(const XMFLOAT2& screen);

// XYZ���ɓ����l�̃X�P�[�����O��������
inline RVector3 ScaleXYZ(const float& scale) { return RVector3(scale, scale, scale); }
} //GameCommonData