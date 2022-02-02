#pragma once
#include "SpriteManager.h"
#include "NY_Camera.h"
#include "TexManager.h"

class Sprite
{
public:
	//�X�v���C�g�ꖇ�̏��
	SpriteData spdata;
	//�A�j���[�V�����f�[�^
	uvAnimData *animData;

public:
	//�X�v���C�g�����i���̐������ɋN���ł����H�j(�f�o�C�X�A�X�v���C�g�T�C�Y�A���\�[�X�ԍ��A�A���J�[�|�C���g�A�X�v���C�g�}�l�[�W���[�|�C���^)
	Sprite(XMFLOAT2 size,UINT resourceID, bool adjustResourceFlag = false,XMFLOAT2 anchor = { 0.0f,0.0f })
	{
		//�X�v���C�g����
		CreateSprite(size, anchor, resourceID, adjustResourceFlag, nullptr);
	};
	//�����Ȃ��i�ʂŏ������j
	Sprite() {};

	//�X�v���C�g������(�f�o�C�X�A�X�v���C�g�T�C�Y�A�A���J�[�|�C���g�A�g�p���郊�\�[�X�ԍ��A���\�[�X�����t���O)
	void CreateSprite(XMFLOAT2 size, XMFLOAT2 anchor, UINT resourceID, bool adjustResourceFlag, uvAnimData *animData);

	//�T�C�Y�ύX
	void ResizeSprite(XMFLOAT2 newsize);

	//�X�v���C�g�X�V
	void UpdateSprite();

	//�X�v���C�g�`��
	void Draw();

};

