#pragma once

#include <NY_Camera.h>
#include <ParticleManager.h>

//�X�e�[�W�����߂�Ƃ��A�X�e�[�W�����܂����Ƃ��ɏo�Ă���p�[�e�B�N��
//�߂�ǂ������A�V���O���g����
class StageMoveParticle final
{
private:
	//�p�[�e�B�N���}�l�[�W���[�i�G�~�b�^�j
	ParticleManager *pManager;
	//�e�N�X�`���ԍ�
	UINT ptex;

	//�R���X�g���N�^�A�f�X�g���N�^
	StageMoveParticle();
	~StageMoveParticle();

public:
	//�R�s�[�A�������


	//�Q�b�^
	static StageMoveParticle *Get() {
		static StageMoveParticle ins;
		return &ins;
	}

	//������
	void Init(NY_Camera *cam);

	//�X�V
	void Update();

	/// <summary>
	/// �p�[�e�B�N������
	/// <para>��������u���b�N�������p�[�e�B�N���́A���t���[���Ăяo��</para>
	/// <para>�����������Ƃɔ��������́A������������ɌĂяo��</para>
	/// </summary>
	/// <param name="spawnPos">�o�����S�_</param>
	/// <param name="max">�o���͈́A���̕���</param>
	/// <param name="min">�o���͈́A���̕���</param>
	void SpawnMoveStandbyParticle(RVector3 spawnPos, RVector3 max, RVector3 min, float s_scale = 0.1f, float e_scale = 0.01f);

	//�`��
	void Draw();

	//�I��
	void Finalize();

};

