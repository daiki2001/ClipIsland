#include "StageMoveParticle.h"

#include <TexManager.h>
#include <NY_random.h>

StageMoveParticle::StageMoveParticle()
{
	//�e�N�X�`���ǂݍ���
	ptex = TexManager::LoadTexture("Resources/blockParticleTex.png");
}

StageMoveParticle::~StageMoveParticle()
{
	//pManager��������Ă��Ȃ��ꍇ
	if (pManager != nullptr) {
		delete pManager;//����
		pManager = nullptr;
	}
}

void StageMoveParticle::Init(NY_Camera *cam)
{
	//�J��������荞�݁A�G�~�b�^�[����
	pManager = ParticleManager::Create(cam);
}

void StageMoveParticle::Update()
{
	//�p�[�e�B�N���X�V
	pManager->Update();
}

void StageMoveParticle::SpawnMoveStandbyParticle(RVector3 spawnPos, RVector3 max, RVector3 min, float s_scale, float e_scale)
{
	//�����Ő����͈͂�����
	float maxX = spawnPos.x + max.x,
		maxY = spawnPos.y + max.y,
		maxZ = spawnPos.z + max.z,
		minX = spawnPos.x + min.x,
		minY = spawnPos.y + min.y,
		minZ = spawnPos.z + min.z;

	NY_random::floatrand_sl(1, -1);

	RVector3 spawnPosition(NY_random::floatrand_sl(maxX, minX),
		NY_random::floatrand_sl(maxY, minY),
		NY_random::floatrand_sl(maxZ, minZ));

	//���x�A�����x��ݒ�
	RVector3 velocity(NY_random::floatrand_sl(1, -1), 0, 1);
	RVector3 accel(NY_random::floatrand_sl(0.2, -0.2), 0, -0.1);

	//����
	pManager->Add(30, spawnPosition, velocity, accel, s_scale, e_scale);

}

void StageMoveParticle::Draw()
{
	//�`��
	pManager->Draw(ptex);
}

void StageMoveParticle::Finalize()
{
	//�G�~�b�^�[������
	delete pManager;
	pManager = nullptr;
}
