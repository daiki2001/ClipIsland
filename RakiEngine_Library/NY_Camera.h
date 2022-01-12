#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class NY_Camera
{
public:
	//�r���[�ϊ����
	XMFLOAT3 _eye;//���_
	XMFLOAT3 _target;//�����_
	XMFLOAT3 _up;//��x�N�g��
	//�r���[�s��
	XMMATRIX _matView;

	//�A�t�B���ϊ����
	XMFLOAT3 _scale{ 1,1,1 };
	XMFLOAT3 _rotation{ 0,0,0 };
	XMFLOAT3 _position{ 0,0,0 };
	//�J�������[�J��
	XMMATRIX _camRocal;
	//�J�������[���h
	XMMATRIX _camWorld;

	//�Ǐ]�I�u�W�F�N�g�̃��[���h�s��A�h���X
	//NY_Object3D *_matFollowObject;

public:
	//�R���X�g���N�^
	NY_Camera(XMFLOAT3 eye_, XMFLOAT3 target_, XMFLOAT3 up_);
	NY_Camera() {};

	//�J�������Ǐ]����I�u�W�F�N�g�̐ݒ�
	//void SetFollowingObject(NY_Object3D *followObjWorld) { _matFollowObject = followObjWorld; }

	//�J�����̍X�V(�S�����A�S�p�x�Ǐ]�A�����͗v���P)
	void UpdateFollowing(XMMATRIX followTarget);


	//�J�����X�V�֐��Q�i����z����,XY���ǂꂭ�炢�Ǐ]���邩�̎w����\�j 
	
	//Z�����W�̂ݒǏ]�B�ǐՋ����w��\�BXY�����ւ̔��Ǐ]���ł���i�����͋����j
	void UpdateFollowingOnlyZ(XMFLOAT3 followTargetPos,XMFLOAT3 followRange,float XfollowPower = 0, float YfollowPower = 0);
	//�����ɍ��킹���Ǐ]�J�����B
	void UpdateFollowingOnlyZVer2(XMFLOAT3 followTarget,XMFLOAT3 followRange,float followRad);
	//����ɉ��ς������������J�����B�Ⴂ�́A���[���h�ϊ����s���A�r���[�s����Ē�`���邱��
	void UpdateFollowingOnlyZWorldMat(XMMATRIX targetWorld);
	/*
	����܂ł̏����̖��_
	�EWorld�ϊ��o�[�W�����́A�r���[�s��̍\���v�f���͕̂ς��Ȃ����߁A�r���[�\���v�f��p���������i�r���{�[�h�j���o���Ȃ�
	�E�r���[�ϊ��o�[�W�����́A�r���[�\���v�f�ɂ�鏈���͉\�����A���[���h���W�n�ł̊Ǘ����ł��Ȃ��̂ŁA�J�����A���O���̕ύX���s���R
	�ł͂��������킹���ꍇ�ǂ��Ȃ邩�H
	*/
	void UpdateFollowingViewAndWorld(XMMATRIX target, XMFLOAT3 range);
	void UpdateFollowingViewAndWorld(XMMATRIX target);

	void UpdateViewMat() {
		_matView = XMMatrixIdentity();
		_matView = XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_target), XMLoadFloat3(&_up));
	}
};

