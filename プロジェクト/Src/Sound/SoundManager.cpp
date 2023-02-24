#include "DxLib.h"

#include <list>
#include <cassert>

#include "SoundManager.h"

#include "../Common.h"
#include "../Debug/Debug.h"

using namespace std;

list<CSoundManager::SoundNode*> CSoundManager::m_SEList;	//SE���X�g
list<CSoundManager::SoundNode*> CSoundManager::m_BGMList;	//BGM���X�g

//SE�t�@�C���p�X
static const char SE_PATH[SE_KIND_NUM][256] =
{
	"Data/Sound/SE/Attack1.mp3",
	"Data/Sound/SE/Attack2.mp3",
	"Data/Sound/SE/Attack3.mp3",
	"Data/Sound/SE/Attack4.mp3",
	"Data/Sound/SE/Beam1.mp3",
	"Data/Sound/SE/Damage1.mp3",
	"Data/Sound/SE/Damage2.mp3",
	"Data/Sound/SE/Fanfare.mp3",
	"Data/Sound/SE/Hit1.mp3",
	"Data/Sound/SE/Hit2.mp3",
	"Data/Sound/SE/Hit3.mp3",
	"Data/Sound/SE/Kaminari.mp3",
	"Data/Sound/SE/KO.mp3",
	"Data/Sound/SE/Select1.mp3",
	"Data/Sound/SE/Select2.mp3",
	"Data/Sound/SE/Shoot1.mp3",
	"Data/Sound/SE/Sword1.mp3",
	"Data/Sound/SE/SwordSlashSuper.mp3",
	"Data/Sound/SE/Warning.mp3",
};

//BGM�t�@�C���p�X
static const char BGM_PATH[BGM_KIND_NUM][256] =
{
	"Data/Sound/BGM/BossBGM.mp3",
	"Data/Sound/BGM/PlayBGM.mp3",
	"Data/Sound/BGM/TitleBGM.mp3",
};

//������
void CSoundManager::Init()
{
	//���ׂċ�ɂ���
	m_SEList.clear();
	m_BGMList.clear();
}

//�㏈��
void CSoundManager::Fin()
{
	//delete����
	for (const auto& node : m_SEList) { DeleteSoundMem(node->m_handle); }
	for (const auto& node : m_BGMList) { DeleteSoundMem(node->m_handle); }

	for (auto& node = m_SEList.begin(); node != m_SEList.end();)
	{
		auto& current_node = node++;

		_DELETE(*current_node);
	}
	for (auto node = m_BGMList.begin(); node != m_BGMList.end();)
	{
		auto current_node = node++;

		_DELETE(*current_node);
	}
	
	//���ׂċ�ɂ���
	m_SEList.clear();
	m_BGMList.clear();
}

//���ʉ��̓ǂݍ���
void CSoundManager::LoadSE(SEKind kind)
{
	SoundNode* se_node = _NEW(SoundNode);

	se_node->m_handle = LoadSoundMem(SE_PATH[kind]);
	se_node->m_kind = kind;

	//���X�g�ɒǉ�
	m_SEList.push_back(se_node);
}
//���ʉ��̍Đ�
void CSoundManager::PlaySE(SEKind kind)
{
	//�T�[�`
	SoundNode* se_node = SearchSE(kind);

	//�Đ�
	PlaySoundMem(se_node->m_handle, DX_PLAYTYPE_BACK, true);
	SetSEVolume(kind, 0.14f);
}
//���ʉ����[�v
void CSoundManager::LoopSE(SEKind kind)
{
	//�T�[�`
	SoundNode* se_node = SearchSE(kind);

	//���[�v�Đ�
	PlaySoundMem(se_node->m_handle, DX_PLAYTYPE_LOOP, true);
	SetSEVolume(kind, 0.14f);
}
//���ʉ���~
void CSoundManager::StopSE(SEKind kind)
{
	//�T�[�`
	SoundNode* se_node = SearchSE(kind);

	//��~
	StopSoundMem(se_node->m_handle);
}

//BGM�ǂݍ���
void CSoundManager::LoadBGM(BGMKind kind)
{
	SoundNode* se_node = _NEW(SoundNode);

	se_node->m_handle = LoadSoundMem(BGM_PATH[kind]);
	se_node->m_kind = kind;

	//���X�g�ɒǉ�
	m_BGMList.push_back(se_node);
}
//BGM�Đ�
void CSoundManager::PlayBGM(BGMKind kind)
{
	//�T�[�`
	SoundNode* se_node = SearchBGM(kind);

	//�Đ�
	PlaySoundMem(se_node->m_handle, DX_PLAYTYPE_LOOP, true);
	SetBGMVolume(kind, 0.2f);
}
//BGM�̒�~
void CSoundManager::StopBGM(BGMKind kind)
{
	//�T�[�`
	SoundNode* se_node = SearchBGM(kind);

	//��~
	StopSoundMem(se_node->m_handle);
}

//�Đ����Ԏ擾�i�~���b�j
int CSoundManager::CSoundManager::GetSETime(SEKind iID)
{
	//�T�[�`
	SoundNode* se_node = SearchSE(iID);

	return static_cast<int>(GetSoundCurrentTime(se_node->m_handle));
}
int CSoundManager::GetBGMTime(BGMKind iID)
{
	//�T�[�`
	SoundNode* se_node = SearchBGM(iID);

	return static_cast<int>(GetSoundCurrentTime(se_node->m_handle));
}

//���Đ����Ԏ擾�i�~���b�j
int CSoundManager::GetSEAllTime(SEKind iID)
{
	//�T�[�`
	SoundNode* se_node = SearchSE(iID);

	return static_cast<int>(GetSoundTotalTime(se_node->m_handle));
}
int CSoundManager::GetBGMAllTime(BGMKind iID)
{
	//�T�[�`
	SoundNode* se_node = SearchBGM(iID);

	return static_cast<int>(GetSoundTotalTime(se_node->m_handle));
}

//�{�����[��
void CSoundManager::SetSEVolume(SEKind iID, float Vol)
{
	//�T�[�`
	SoundNode* se_node = SearchSE(iID);
	
	if (Vol<0.f || Vol>1.f)
	{
		return;
	}
	ChangeVolumeSoundMem((int)(255.f*Vol), se_node->m_handle);
}
void CSoundManager::SetBGMVolume(BGMKind iID, float Vol)
{
	//�T�[�`
	SoundNode* se_node = SearchBGM(iID);
	
	if (Vol<0.f || Vol>1.f)
	{
		return;
	}
	ChangeVolumeSoundMem((int)(255.f*Vol), se_node->m_handle);
}

//�T�[�`�֐�
CSoundManager::SoundNode* CSoundManager::SearchSE(SEKind kind)
{
	for (const auto& node : m_SEList)
	{
		//����������Ԃ�
		if (node->m_kind == kind)
			return node;
	}

	//�Ȃ��ꍇ�ǂݍ��܂�Ă��Ȃ��\�����I�I
#ifdef DEBUG_MODE
	
#endif
	return NULL;
}
CSoundManager::SoundNode* CSoundManager::SearchBGM(BGMKind kind)
{
	for (const auto& node : m_BGMList)
	{
		//����������Ԃ�
		if (node->m_kind == kind)
			return node;
	}

	//�Ȃ��ꍇ�ǂݍ��܂�Ă��Ȃ��\�����I�I
#ifdef DEBUG_MODE

#endif
	return NULL;
}