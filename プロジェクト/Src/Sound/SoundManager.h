#pragma once

#include <list>

using namespace std;

//���ʉ����
enum SEKind
{
	SE_KIND_NONE = -1,

	SE_KIND_ATTACK1,
	SE_KIND_ATTACK2,
	SE_KIND_ATTACK3,
	SE_KIND_ATTACK4,
	SE_KIND_BEAM1,
	SE_KIND_DAMAGE1,
	SE_KIND_DAMAGE2,
	SE_KIND_FANFARE,
	SE_KIND_HIT1,
	SE_KIND_HIT2,
	SE_KIND_HIT3,
	SE_KIND_THUNDER,
	SE_KIND_KO,
	SE_KIND_SELECT1,
	SE_KIND_SELECT2,
	SE_KIND_SHOOT1,
	SE_KIND_SWORD1,
	SE_KIND_SWORD_SUPER,
	SE_KIND_WARNING,

	SE_KIND_NUM,
};

//BGM���
enum BGMKind
{
	BGM_KIND_NONE = -1,

	BGM_KIND_BOSS,
	BGM_KIND_PLAY,
	BGM_KIND_TITLE,

	BGM_KIND_NUM,
};

class CSoundManager
{
public:
	//�T�E���h���\����
	struct SoundNode
	{
		int m_handle;
		int m_kind;

		SoundNode():
			m_handle(-1),
			m_kind(-1)
		{}
	};
public:
	static list<SoundNode*> m_SEList;	//SE���X�g
	static list<SoundNode*> m_BGMList;	//BGM���X�g

public:
	//������
	static void Init();

	//�㏈��
	static void Fin();

	//���ʉ��̓ǂݍ���
	static void LoadSE(SEKind kind);
	//���ʉ��̍Đ�
	static void PlaySE(SEKind kind);
	//���ʉ����[�v
	static void LoopSE(SEKind kind);
	//���ʉ���~
	static void StopSE(SEKind kind);

	//BGM�ǂݍ���
	static void LoadBGM(BGMKind kind);
	//BGM�Đ�
	static void PlayBGM(BGMKind kind);
	//BGM�̒�~
	static void StopBGM(BGMKind kind);

	//�Đ����Ԏ擾�i�~���b�j
	static int GetSETime(SEKind iID);
	static int GetBGMTime(BGMKind iID);

	//���Đ����Ԏ擾�i�~���b�j
	static int GetSEAllTime(SEKind iID);
	static int GetBGMAllTime(BGMKind iID);

	//�{�����[��
	static void SetSEVolume(SEKind iID, float Vol);
	static void SetBGMVolume(BGMKind iID, float Vol);

	//�T�[�`�֐�
	static SoundNode* SearchSE(SEKind kind);
	static SoundNode* SearchBGM(BGMKind kind);

	//�T�[�`handle
};