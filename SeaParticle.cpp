#include "SeaParticle.h"
#include <NY_random.h>

void SeaParticle::Init()
{
	//�J�E���g������
	spawnCount = 0;
}

void SeaParticle::Update()
{
	spawnCount++;
	//�������ԃJ�E���g
	if (spawnCount > SPAWN_DELAY) {
		//��������ݒ�
		int spawnNum = NY_random::intrand_sl(SPAWN_NUM_MAX, SPAWN_NUM_MIN);
		for (int i = 0; i < spawnNum; i++) {
			//�o���n�_�ݒ�
			RVector3 spawnPos(
				NY_random::floatrand_sl(640, -640),
				NY_random::floatrand_sl(360, -360),
				0
			);
			//����
			pManager->Add(120, spawnPos,
				RVector3(0, 0, 0), RVector3(0, 0, 0),
				0.5f, 5.0f,
				XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 0));
		}

		//�J�E���g�����Z�b�g
		spawnCount = 0;
	}

	//�S�p�[�e�B�N���X�V
	pManager->Update();
}

void SeaParticle::Draw()
{
	//�p�[�e�B�N���`��
	pManager->Draw(pTex);
}

