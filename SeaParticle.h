#pragma once
#include <TexManager.h>
#include <ParticleManager.h>

#define SPAWN_DELAY		(20)
#define SPAWN_NUM_MAX	(16)
#define SPAWN_NUM_MIN	(8)

//�V���O���g���i�����́j
class SeaParticle final
{
private:
	//�G�~�b�^�[
	ParticleManager *pManager;
	//�p�[�e�B�N�����\�[�X
	UINT			pTex;
	//�p�[�e�B�N���������ԃJ�E���g
	int				spawnCount;

	//�R���X�g���N�^
	SeaParticle() {
		pTex = TexManager::LoadTexture("Resources/bubble.png");
		pManager = ParticleManager::Create();
	}
	//�f�X�g���N�^
	~SeaParticle() {
		delete pManager;
	}


public:
	//�Q�b�^
	static SeaParticle *Get() {
		static SeaParticle ins;
		return &ins;
	}

	//������
	void Init();

	//�X�V�i���t���[�������ƍX�V������j
	void Update();

	//�`��
	void Draw();

};

#define SEA_INIT (SeaParticle::Get()->Init())
#define SEA_UPDATE (SeaParticle::Get()->Update())
#define SEA_DRAW (SeaParticle::Get()->Draw())