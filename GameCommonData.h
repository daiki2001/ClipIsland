#pragma once
#include "BlockData.h"
#include "NY_Object3DMgr.h"
#include "RVector.h"

namespace GameCommonData
{
class CommonData final
{
private: // �V���O���g����
	CommonData() {}
	~CommonData() {}
public:
	CommonData(const CommonData&) = delete;
	CommonData operator=(const CommonData&) = delete;

public: // �ÓI�����o�֐�
	// CommonData�̒��ɐ錾�����ϐ��̏�����
	static void StaticInitiizer(); //�Q�[�����ɏo�Ă��鋤�ʂ̔��̃��f��

public: // �ÓI�����o�ϐ�
	static UINT boxGraph;       //�Q�[�����ɏo�Ă��鋤�ʂ̔��̉摜
	static NY_Model3D boxModel; //�Q�[�����ɏo�Ă��鋤�ʂ̔��̃��f��
};

RVector3 ScreenToWorld(const XMFLOAT2& screen);
} //GameCommonData