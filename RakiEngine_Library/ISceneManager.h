#pragma once
#include "SceneBase.h"
#include "SceneChanger.h"
#include "Task.h"

#include <vector>

class ISceneManager : public Task
{
private:
	//SceneBase�̃|�C���^�^List�R���e�i
	static std::vector<SceneBase*> scenes;
	//���݂̃V�[���ԍ�
	static int sceneNum;
	//�ύX��V�[���ԍ�
	static int changeNum;

public:
	//�R���X�g���N�^�A�f�X�g���N�^
	ISceneManager();
	~ISceneManager();

	/// <summary>
	/// ���[�U�[���쐬�����V�[�����V�[���R���e�i�ɒǉ�
	/// </summary>
	/// <param name="scene">SceneBase���p�����č�����V�[���N���X�̃|�C���^�ϐ�</param>
	/// <returns>�i�[�����V�[���̏ꏊ�������ԍ�</returns>
	int AddScene(SceneBase *scene);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

	/// <summary>
	/// ���s�V�[����ύX
	/// </summary>
	/// <param name="nextSceneNum"></param>
	void ChangeScene(int nextSceneNum);

};

