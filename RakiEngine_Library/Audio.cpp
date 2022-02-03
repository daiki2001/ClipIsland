#include "Audio.h"

ComPtr<IXAudio2>        Audio::xAudio2;
IXAudio2MasteringVoice *Audio::masterVoice;
float                   Audio::volume;

void Audio::Init()
{
    //xAudio�G���W���̃C���X�^���X�쐬
    HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    //�}�X�^�[�{�C�X�쐬
    result = xAudio2->CreateMasteringVoice(&masterVoice);
    //�{�����[��������(50%)
    volume = 0.5f;
}

SoundData Audio::LoadSound_wav(const char *filename)
{
    //�t�@�C�����̓X�g���[���̃C���X�^���X
    std::ifstream file;
    //.wav�t�@�C�����o�C�i�����[�h�ŊJ��
    file.open(filename, std::ios_base::binary);
    //�I�[�v�����s�����o
    assert(file.is_open());

    //RIFT�w�b�_�[�ǂݍ���
    RiffHeader riff;
    file.read((char *)&riff, sizeof(riff));
    //�t�@�C����RIFT���`�F�b�N
    if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
        assert(0);
    }
    //�^�C�v��WAVE���`�F�b�N
    if (strncmp(riff.type, "WAVE", 4) != 0) {
        assert(0);
    }

    //�t�H�[�}�b�g�`�����N�̓ǂݍ���
    FormatChunk format = {};
    //�`�����N�w�b�_�[�̊m�F
    file.read((char *)&format, sizeof(ChunkHeader));
    //JUNK�`�����N���o��
    if (strncmp(format.chunk.id, "JUNK", 4) == 0) {
        //�ǂݎ����W�����N�`�����N�̏I���܂Ői�߂�
        file.seekg(format.chunk.size, std::ios_base::cur);
        //�ēǂݍ���
        file.read((char *)&format, sizeof(ChunkHeader));
    }
    if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
        assert(0);
    }
    //�`�����N�{�̂̓ǂݍ���
    assert(format.chunk.size <= sizeof(format.fmt));
    file.read((char *)&format.fmt, format.chunk.size);

    //Data�`�����N�̓ǂݍ���
    ChunkHeader data;
    file.read((char *)&data, sizeof(data));
    //JUNK�`�����N���o��
    if (strncmp(data.id, "JUNK", 4) == 0) {
        //�ǂݎ����W�����N�`�����N�̏I���܂Ői�߂�
        file.seekg(data.size, std::ios_base::cur);
        //�ēǂݍ���
        file.read((char *)&data, sizeof(data));
    }
    if (strncmp(data.id, "data", 4) != 0) {
        assert(0);
    }
    //data�`�����N�̃f�[�^�����ǂݍ���
    char *pBuffer = new char[data.size];
    file.read(pBuffer, data.size);
    //wav�����
    file.close();

    //�ԋp����f�[�^
    SoundData soundData = {};
    soundData.wfex       = format.fmt;
    soundData.pBuffer    = reinterpret_cast<BYTE*>(pBuffer);
    soundData.bufferSize = data.size;
    HRESULT result;
    //�g�`�t�H�[�}�b�g�����ƂɃ\�[�X�̍쐬
    result = xAudio2->CreateSourceVoice(&soundData.source, &soundData.wfex);
    assert(SUCCEEDED(result));

    //�Đ�����g�`�f�[�^�̐ݒ�
    soundData.buf.pAudioData = soundData.pBuffer;
    soundData.buf.AudioBytes = soundData.bufferSize;
    soundData.buf.Flags = XAUDIO2_END_OF_STREAM;

    return soundData;
}

void Audio::UnloadSound(SoundData *data)
{
    //���������
    delete[] data->pBuffer;

    data->pBuffer = 0;
    data->bufferSize = 0;
    data->wfex = {};
}

void Audio::SetPlayRoopmode(SoundData &soundData,int roopCount)
{
    if (roopCount < 0 || roopCount>256) { return; }

    soundData.buf.LoopCount = roopCount;
    if (roopCount == 0) {//���[�v�񐔂�0�̎�
        soundData.buf.LoopBegin = 0;
        soundData.buf.LoopLength = 0;
    }
}

void Audio::PlayLoadedSound(const SoundData &soundData)
{
    HRESULT result;

    result = soundData.source->Stop();
    result = soundData.source->FlushSourceBuffers();
    result = soundData.source->SetVolume(volume);
    //�g�`�f�[�^�Đ�
    result = soundData.source->SubmitSourceBuffer(&soundData.buf);
    result = soundData.source->Start();
}

void Audio::PauseLoadedSound(const SoundData &soundData)
{
    HRESULT result;

    //�g�`�f�[�^��~
    result = soundData.source->SubmitSourceBuffer(&soundData.buf);
    result = soundData.source->Stop();
}

void Audio::StopLoadedSound(SoundData &soundData)
{
    HRESULT result;

    //�g�`�f�[�^��~
    result = soundData.source->Stop();
    result = soundData.source->FlushSourceBuffers();
    result = soundData.source->SubmitSourceBuffer(&soundData.buf);
}
