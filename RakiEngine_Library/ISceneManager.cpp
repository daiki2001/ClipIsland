#include "ISceneManager.h"

#include <assert.h>

std::vector<SceneBase *> ISceneManager::scenes;
int ISceneManager::sceneNum = 0;
int ISceneManager::changeNum = 0;

ISceneManager::ISceneManager()
{
	
}

ISceneManager::~ISceneManager()
{
	//�S�V�[���̏I��
	Finalize();
}

int ISceneManager::AddScene(SceneBase *scene)
{
	//�����̃V�[����list�Ɋi�[
	scenes.push_back(scene);

	//��{�����ɒǉ����邽�߁A�����̈ʒu���擾���ĕԋp
	return scenes.size() - 1;
}

void ISceneManager::Initialize()
{
	//�V�[�����i�[����Ă��Ȃ��ꍇ�̓G���[
	assert(scenes.size() == 0);
	//������s����V�[���͐擪�̃V�[��
	changeNum = 0;
	sceneNum = 0;
	//�V�[��������
	scenes[sceneNum]->Initialize();
}

void ISceneManager::Update()
{
	//�V�[���ύX�����m
	if (changeNum != sceneNum) {
		//���݃V�[���̏I�����������s
		scenes[sceneNum]->Finalize();
		//���V�[���̏��������������s
		scenes[changeNum]->Initialize();
		//���s�V�[���ԍ���ύX
		sceneNum = changeNum;
	}
	//�V�[���̍X�V���������s
	scenes[sceneNum]->Update();
}

void ISceneManager::Draw()
{
	//���s�V�[���̕`�揈�������s
	scenes[sceneNum]->Draw();
}

void ISceneManager::Finalize()
{
	//�S�V�[���̏I�����������s
	for (int i = 0; i < scenes.size(); i++) {
		scenes[i]->Finalize();
	}
}

void ISceneManager::ChangeScene(int nextSceneNum)
{
	//�ύX��V�[���ԍ���ݒ�
	changeNum = nextSceneNum;
}
