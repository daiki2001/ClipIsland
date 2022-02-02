#include "SceneChangeDirection.h"

SceneChangeDirection::SceneChangeDirection() {
	//�e�N�X�`���ǂݍ���
	tex1 = TexManager::LoadTexture("Resources/SCClip1.png");
	tex2 = TexManager::LoadTexture("Resources/SCClip2.png");
	//�e�N�X�`������
	XMFLOAT2 size = { 640,720 };
	XMFLOAT2 anchor = { 0.5,0.5 };
	press1.CreateSprite(size, anchor, tex1, false, nullptr);
	press2.CreateSprite(size, anchor, tex2, false, nullptr);
	//���W����ʊO��
	press1.spdata.position = RVector3(-1280 / 2, 720 / 2, 0);
	press2.spdata.position = RVector3(1280 + 1280 / 2, 720 / 2, 0);
	press1.UpdateSprite();
	press2.UpdateSprite();
}

SceneChangeDirection::~SceneChangeDirection()
{
}

void SceneChangeDirection::Init()
{
	//���o����
	isEndDir = false;
	//�ǂ����̉��o��������
	frameCount = 0;
	isStart = false;
	isEnd = false;
}

void SceneChangeDirection::Start_StartDir()
{
	//�X�^�[�g���o
	isStart = true;
	//���o�J�n
	isEndDir = false;
	//�t���[���J�E���g������
	frameCount = 0;
	//�C�[�W���O�ڕW���W�ݒ�
	//�J�n�_
	ps1s = RVector3(1280 / 4, 720 / 2, 0);
	ps2s = RVector3(1280 / 4 * 3, 720 / 2, 0);
	//�I���_
	ps1e = RVector3(-1280 / 4, 720 / 2, 0);
	ps2e = RVector3(1280 + 1280 / 4, 720 / 2, 0);
}

void SceneChangeDirection::Start_EndDir()
{
	//�I�����o
	isEnd = true;
	//���o�J�n
	isEndDir = false;
	//�t���[���J�E���g������
	frameCount = 0;
	//�C�[�W���O�ڕW���W�ݒ�
	//�J�n�_
	ps1s = RVector3(-1280 / 4, 720 / 2, 0);
	ps2s = RVector3(1280 + 1280 / 4, 720 / 2, 0);
	//�I���_
	ps1e = RVector3(1280 / 4, 720 / 2, 0);
	ps2e = RVector3((1280 / 4) * 3, 720 / 2, 0);
}

void SceneChangeDirection::Update()
{
	//�C���N�������g
	if (frameCount < DIR_FRAME) {
		frameCount++;
	}
	float timeRate = float(frameCount) / float(DIR_FRAME);

	//�C�[�W���O�v�Z
	if (isStart) {
		press1.spdata.position = Rv3Ease::OutQuad(ps1s, ps1e, timeRate);
		press2.spdata.position = Rv3Ease::OutQuad(ps2s, ps2e, timeRate);
	}
	else if(isEnd){
		press1.spdata.position = Rv3Ease::InQuad(ps1s, ps1e, timeRate);
		press2.spdata.position = Rv3Ease::InQuad(ps2s, ps2e, timeRate);
	}
	press1.UpdateSprite();
	press2.UpdateSprite();

	//�t���[����������l
	if (frameCount > DIR_FRAME) {
		isEndDir = true;
	}
}

void SceneChangeDirection::Draw()
{
	//�`�揀��
	SpriteManager::Get()->SetCommonBeginDraw();
	//�`��
	press1.Draw();
	press2.Draw();
}
