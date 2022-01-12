#pragma once

#include <xaudio2.h>
#include <fstream>
#include <assert.h>
#include <wrl.h>

#pragma comment(lib,"xaudio2.lib")

using namespace Microsoft::WRL;

//�����f�[�^�Ǘ��\����
struct ChunkHeader {
	char id[4];   //�`�����NiD
	int32_t size; //�`�����N�T�C�Y
};

//RIFF�w�b�_�`�����N
struct RiffHeader {
	ChunkHeader chunk;
	char type[4];
};

//FMT�`�����N
struct FormatChunk {
	ChunkHeader  chunk; //"fmt "
	WAVEFORMATEX fmt;   //�g�`�t�H�[�}�b�g
};

//�����f�[�^
struct SoundData {
	//�g�`�t�H�[�}�b�g
	WAVEFORMATEX wfex;
	//�o�b�t�@�擪�A�h���X
	BYTE *pBuffer;
	//�o�b�t�@�T�C�Y
	unsigned int bufferSize;
	//�I�[�f�B�I�|�C���^
	IXAudio2SourceVoice *source;
	//�o�b�t�@
	XAUDIO2_BUFFER buf{};
};


class Audio
{
public:

	static ComPtr<IXAudio2>        xAudio2;
	static IXAudio2MasteringVoice *masterVoice;
	static float volume;

public:

	Audio() {
		//xAudio�G���W���̃C���X�^���X�쐬
		HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
		//�}�X�^�[�{�C�X�쐬
		result = xAudio2->CreateMasteringVoice(&masterVoice);
		//�{�����[��������(50%)
		volume = 0.5f;
	}

	//������
	static void Init();

	//�T�E���h�f�[�^�̓ǂݍ���
	static SoundData LoadSound_wav(const char *filename);
	//�T�E���h�f�[�^�̃A�����[�h
	static void UnloadSound(SoundData *data);

	//���[�v�̐ݒ�(0~254�Ń��[�v�񐔂��w��B255�̏ꍇ�������[�v�B����ȊO�͖���)
	static void SetPlayRoopmode(SoundData &soundData,int roopCount);
	//�Đ�
	static void PlayLoadedSound(const SoundData &soundData);
	//�ꎞ��~
	static void PauseLoadedSound(const SoundData &soundData);
	//��~
	static void StopLoadedSound(SoundData &soundData);

};

