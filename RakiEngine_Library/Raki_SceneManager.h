#pragma once
#include "ISceneManager.h"

class Raki_SceneManager final
{
private:
	//�V�[���Ǘ��N���X
	static ISceneManager *mgr;

	Raki_SceneManager(){}
	~Raki_SceneManager() {
		delete mgr;
	}

public:


};

