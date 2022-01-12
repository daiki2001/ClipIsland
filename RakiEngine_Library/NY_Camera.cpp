#include "NY_Camera.h"

NY_Camera::NY_Camera(XMFLOAT3 eye_, XMFLOAT3 target_, XMFLOAT3 up_)
{
	//�e�l�̊i�[
	_eye = eye_;
	_target = target_;
	_up = up_;

	//�r���[�s��i�[
	_matView = XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_target), XMLoadFloat3(&_up));

	//���[�J�����W����
	_camRocal = XMMatrixIdentity();

	_camRocal.r[0].m128_f32[0] = _eye.x;
	_camRocal.r[0].m128_f32[1] = _eye.y;
	_camRocal.r[0].m128_f32[2] = _eye.z;
	_camRocal.r[0].m128_f32[3] = 1;

	_camRocal.r[1].m128_f32[0] = _up.x - _eye.x;
	_camRocal.r[1].m128_f32[1] = _up.y - _eye.y;
	_camRocal.r[1].m128_f32[2] = _up.z - _eye.z;
	_camRocal.r[1].m128_f32[3] = 0;

	_camRocal.r[2].m128_f32[0] = _target.x;
	_camRocal.r[2].m128_f32[1] = _target.y;
	_camRocal.r[2].m128_f32[2] = _target.z;
	_camRocal.r[2].m128_f32[3] = 1;

	_camRocal.r[3].m128_f32[3] = 0;
}

void NY_Camera::UpdateFollowing(XMMATRIX followTarget)//�����͒Ǐ]�������I�u�W�F�N�g�̃��[���h�ϊ��s��
{
	//�󂯎�郏�[���h�ϊ��s���������ΒǏ]�������I�u�W�F�N�g���ł邩�H
	//�J�����̃��[���h���W�s��łł���H
	_position.z = -30;
	_position.y = 10;

	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�s��̌v�Z

	matScale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(_rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(_rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(_rotation.y));

	matTrans = XMMatrixTranslation(_position.x, _position.y, _position.z);

	_camRocal = XMMatrixIdentity();
	_camRocal *= matScale;//���[���h�s��ɃX�P�[�����O�𔽉f
	_camRocal *= matRot;
	_camRocal *= matTrans;

	_matView = XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_target), XMLoadFloat3(&_up));

	_matView = _matView * _camRocal * followTarget;
	
	_matView = XMMatrixInverse(nullptr, _matView);
	

}

void NY_Camera::UpdateFollowingOnlyZ(XMFLOAT3 followTargetPos, XMFLOAT3 followRange, float XfollowState, float YfollowState)//Z�ȊO�g��Ȃ��Ȃ�A�I�u�W�F�N�g��Z�������ׂ��ł́H
{

	// �Ǐ]�ʒu�̎w��
	_position.x = followRange.x;
	_position.y = followRange.y;
	_position.z = followRange.z;

	// �J�����̃��[���h�s��ϊ��p
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�s��̌v�Z
	matScale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);

	matRot =  XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(_rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(_rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(_rotation.y));

	matTrans = XMMatrixTranslation(_position.x, _position.y, _position.z);

	// ���[���h�ϊ�
	_camRocal = XMMatrixIdentity();
	_camRocal *= matScale;//���[���h�s��ɃX�P�[�����O�𔽉f
	_camRocal *= matRot;
	_camRocal *= matTrans;

	// �Ǐ]�I�u�W�F�N�g�̐��l���H?�Ǐ]�������I�u�W�F�N�g��Z�ȊO��0�ɂ����s��𐶐�
	XMMATRIX objScale, objRot, objTrans, objProcessed;

	objScale = XMMatrixScaling(1, 1, 1);

	objRot = XMMatrixIdentity();
	objRot *= XMMatrixRotationZ(XMConvertToRadians(0));
	objRot *= XMMatrixRotationX(XMConvertToRadians(0));
	objRot *= XMMatrixRotationY(XMConvertToRadians(0));

	//�Ǐ]�x��p�����v�Z���A���̎�@���ƃJ�����̃p�����[�^��p�������������s���Y������������?���߁A���P���K�v
	objTrans = XMMatrixTranslation(followTargetPos.x * XfollowState, followTargetPos.y * YfollowState, followTargetPos.z);

	//�Ǐ]�p�ɉ��H�������[���h�s�񐶐�
	objProcessed = XMMatrixIdentity();
	objProcessed *= objScale;
	objProcessed *= objRot;
	objProcessed *= objTrans;

	//�r���[�s�񐶐�
	_matView = XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_target), XMLoadFloat3(&_up));
	//�Ǐ]�ŕϊ��s��
	_matView = _matView * _camRocal * objProcessed;
	//�r���[�p�ɋt�s��ɕϊ�
	_matView = XMMatrixInverse(nullptr, _matView);
}

void NY_Camera::UpdateFollowingOnlyZVer2(XMFLOAT3 followTarget, XMFLOAT3 followRange, float followRad)
{
	//�ǔ��^�[�Q�b�g�̃Z�b�g
	_target = followTarget;
	//����]��Ԃ̃I�t�Z�b�g�x�N�g��
	XMVECTOR offVec = { 0.0f,0.0f,followRange.z,0.0f };
	//Y����followRad���̉�]�s����o��
	XMMATRIX rot = XMMatrixIdentity();
	rot *= XMMatrixRotationY(XMConvertToRadians(followRad));
	//offset��rot�ŉ�]�������x�N�g�����o��
	XMVECTOR rotVec = XMVector3TransformNormal(offVec, rot);
	//target��rotVec�����Z���Aeye���o���B
	_eye.x = _target.x + rotVec.m128_f32[0];
	_eye.y = _target.y + rotVec.m128_f32[1];
	_eye.z = _target.z + rotVec.m128_f32[2];

	//�r���[����
	_matView = XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_target), XMLoadFloat3(&_up));

}

void NY_Camera::UpdateFollowingOnlyZWorldMat(XMMATRIX targetWorld)
{
	//�Ǐ]����^�[�Q�b�g�̃��[���h�ƍ���
	XMMATRIX cam_after = _camRocal * targetWorld;

	//�r���[����
	_matView = XMMatrixLookAtLH(
		XMVECTOR{ cam_after.r[0].m128_f32[0],cam_after.r[0].m128_f32[1], cam_after.r[0].m128_f32[2], },
		XMVECTOR{ cam_after.r[2].m128_f32[0],cam_after.r[2].m128_f32[1], cam_after.r[2].m128_f32[2], },
		XMVECTOR{ cam_after.r[1].m128_f32[0],cam_after.r[1].m128_f32[1], cam_after.r[1].m128_f32[2], }
	);

	//���_�A�����_�A������x�N�g���̍X�V
	_eye = { cam_after.r[0].m128_f32[0],cam_after.r[0].m128_f32[1], cam_after.r[0].m128_f32[2] };
	_up = { cam_after.r[1].m128_f32[0],cam_after.r[1].m128_f32[1], cam_after.r[1].m128_f32[2] };
	_target = { cam_after.r[2].m128_f32[0], cam_after.r[2].m128_f32[1], cam_after.r[2].m128_f32[2] };

}

void NY_Camera::UpdateFollowingViewAndWorld(XMMATRIX target, XMFLOAT3 range)
{
	// �Ǐ]�ʒu�̎w��
	_position.x = range.x;
	_position.y = range.y;
	_position.z = range.z;

	// �J�����̃��[���h�s��ϊ��p
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�s��̌v�Z
	//matScale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(_rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(_rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(_rotation.y));

	matTrans = XMMatrixTranslation(_position.x, _position.y, _position.z);

	//���[���h���W�v�Z
	_camWorld = matRot * matTrans;

	//�Ǐ]����^�[�Q�b�g�̃��[���h�ƍ���
	XMMATRIX cam_after = _camRocal * target;

	//�r���[����
	_matView = XMMatrixLookAtLH(
		XMVECTOR{ cam_after.r[0].m128_f32[0],cam_after.r[0].m128_f32[1], cam_after.r[0].m128_f32[2], },
		XMVECTOR{ cam_after.r[2].m128_f32[0],cam_after.r[2].m128_f32[1], cam_after.r[2].m128_f32[2], },
		XMVECTOR{ cam_after.r[1].m128_f32[0],cam_after.r[1].m128_f32[1], cam_after.r[1].m128_f32[2], }
	);

	//���_�A�����_�A������x�N�g���̍X�V
	_eye = { cam_after.r[0].m128_f32[0],cam_after.r[0].m128_f32[1], cam_after.r[0].m128_f32[2] };
	_up = { cam_after.r[1].m128_f32[0],cam_after.r[1].m128_f32[1], cam_after.r[1].m128_f32[2] };
	_target = { cam_after.r[2].m128_f32[0], cam_after.r[2].m128_f32[1], cam_after.r[2].m128_f32[2] };

	//�r���[�̍ŏI�X�V
	_matView *= _camWorld;

}

void NY_Camera::UpdateFollowingViewAndWorld(XMMATRIX target)
{
	// �J�����̃��[���h�s��ϊ��p
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�s��̌v�Z
	//matScale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(_rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(_rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(_rotation.y));

	matTrans = XMMatrixTranslation(_position.x, _position.y, _position.z);

	//���[���h���W�v�Z
	_camWorld = matRot * matTrans;

	//�Ǐ]����^�[�Q�b�g�̃��[���h�ƍ���
	XMMATRIX cam_after = _camRocal * target;

	//�r���[����
	_matView = XMMatrixLookAtLH(
		XMVECTOR{ cam_after.r[0].m128_f32[0],cam_after.r[0].m128_f32[1], cam_after.r[0].m128_f32[2], },
		XMVECTOR{ cam_after.r[2].m128_f32[0],cam_after.r[2].m128_f32[1], cam_after.r[2].m128_f32[2], },
		XMVECTOR{ cam_after.r[1].m128_f32[0],cam_after.r[1].m128_f32[1], cam_after.r[1].m128_f32[2], }
	);

	//���_�A�����_�A������x�N�g���̍X�V
	_eye = { cam_after.r[0].m128_f32[0],cam_after.r[0].m128_f32[1], cam_after.r[0].m128_f32[2] };
	_up = { cam_after.r[1].m128_f32[0],cam_after.r[1].m128_f32[1], cam_after.r[1].m128_f32[2] };
	_target = { cam_after.r[2].m128_f32[0], cam_after.r[2].m128_f32[1], cam_after.r[2].m128_f32[2] };

	//�r���[�̍ŏI�X�V
	_matView *= _camWorld;
}
