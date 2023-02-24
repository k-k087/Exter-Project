#include "DxLib.h"

#include <list>
#include <cassert>

#include "SoundManager.h"

#include "../Common.h"
#include "../Debug/Debug.h"

using namespace std;

list<CSoundManager::SoundNode*> CSoundManager::m_SEList;	//SEリスト
list<CSoundManager::SoundNode*> CSoundManager::m_BGMList;	//BGMリスト

//SEファイルパス
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

//BGMファイルパス
static const char BGM_PATH[BGM_KIND_NUM][256] =
{
	"Data/Sound/BGM/BossBGM.mp3",
	"Data/Sound/BGM/PlayBGM.mp3",
	"Data/Sound/BGM/TitleBGM.mp3",
};

//初期化
void CSoundManager::Init()
{
	//すべて空にする
	m_SEList.clear();
	m_BGMList.clear();
}

//後処理
void CSoundManager::Fin()
{
	//delete処理
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
	
	//すべて空にする
	m_SEList.clear();
	m_BGMList.clear();
}

//効果音の読み込み
void CSoundManager::LoadSE(SEKind kind)
{
	SoundNode* se_node = _NEW(SoundNode);

	se_node->m_handle = LoadSoundMem(SE_PATH[kind]);
	se_node->m_kind = kind;

	//リストに追加
	m_SEList.push_back(se_node);
}
//効果音の再生
void CSoundManager::PlaySE(SEKind kind)
{
	//サーチ
	SoundNode* se_node = SearchSE(kind);

	//再生
	PlaySoundMem(se_node->m_handle, DX_PLAYTYPE_BACK, true);
	SetSEVolume(kind, 0.14f);
}
//効果音ループ
void CSoundManager::LoopSE(SEKind kind)
{
	//サーチ
	SoundNode* se_node = SearchSE(kind);

	//ループ再生
	PlaySoundMem(se_node->m_handle, DX_PLAYTYPE_LOOP, true);
	SetSEVolume(kind, 0.14f);
}
//効果音停止
void CSoundManager::StopSE(SEKind kind)
{
	//サーチ
	SoundNode* se_node = SearchSE(kind);

	//停止
	StopSoundMem(se_node->m_handle);
}

//BGM読み込み
void CSoundManager::LoadBGM(BGMKind kind)
{
	SoundNode* se_node = _NEW(SoundNode);

	se_node->m_handle = LoadSoundMem(BGM_PATH[kind]);
	se_node->m_kind = kind;

	//リストに追加
	m_BGMList.push_back(se_node);
}
//BGM再生
void CSoundManager::PlayBGM(BGMKind kind)
{
	//サーチ
	SoundNode* se_node = SearchBGM(kind);

	//再生
	PlaySoundMem(se_node->m_handle, DX_PLAYTYPE_LOOP, true);
	SetBGMVolume(kind, 0.2f);
}
//BGMの停止
void CSoundManager::StopBGM(BGMKind kind)
{
	//サーチ
	SoundNode* se_node = SearchBGM(kind);

	//停止
	StopSoundMem(se_node->m_handle);
}

//再生時間取得（ミリ秒）
int CSoundManager::CSoundManager::GetSETime(SEKind iID)
{
	//サーチ
	SoundNode* se_node = SearchSE(iID);

	return static_cast<int>(GetSoundCurrentTime(se_node->m_handle));
}
int CSoundManager::GetBGMTime(BGMKind iID)
{
	//サーチ
	SoundNode* se_node = SearchBGM(iID);

	return static_cast<int>(GetSoundCurrentTime(se_node->m_handle));
}

//総再生時間取得（ミリ秒）
int CSoundManager::GetSEAllTime(SEKind iID)
{
	//サーチ
	SoundNode* se_node = SearchSE(iID);

	return static_cast<int>(GetSoundTotalTime(se_node->m_handle));
}
int CSoundManager::GetBGMAllTime(BGMKind iID)
{
	//サーチ
	SoundNode* se_node = SearchBGM(iID);

	return static_cast<int>(GetSoundTotalTime(se_node->m_handle));
}

//ボリューム
void CSoundManager::SetSEVolume(SEKind iID, float Vol)
{
	//サーチ
	SoundNode* se_node = SearchSE(iID);
	
	if (Vol<0.f || Vol>1.f)
	{
		return;
	}
	ChangeVolumeSoundMem((int)(255.f*Vol), se_node->m_handle);
}
void CSoundManager::SetBGMVolume(BGMKind iID, float Vol)
{
	//サーチ
	SoundNode* se_node = SearchBGM(iID);
	
	if (Vol<0.f || Vol>1.f)
	{
		return;
	}
	ChangeVolumeSoundMem((int)(255.f*Vol), se_node->m_handle);
}

//サーチ関数
CSoundManager::SoundNode* CSoundManager::SearchSE(SEKind kind)
{
	for (const auto& node : m_SEList)
	{
		//見つかったら返す
		if (node->m_kind == kind)
			return node;
	}

	//ない場合読み込まれていない可能性が！！
#ifdef DEBUG_MODE
	
#endif
	return NULL;
}
CSoundManager::SoundNode* CSoundManager::SearchBGM(BGMKind kind)
{
	for (const auto& node : m_BGMList)
	{
		//見つかったら返す
		if (node->m_kind == kind)
			return node;
	}

	//ない場合読み込まれていない可能性が！！
#ifdef DEBUG_MODE

#endif
	return NULL;
}