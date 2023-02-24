#pragma once

#include <list>

using namespace std;

//効果音種類
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

//BGM種類
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
	//サウンド情報構造体
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
	static list<SoundNode*> m_SEList;	//SEリスト
	static list<SoundNode*> m_BGMList;	//BGMリスト

public:
	//初期化
	static void Init();

	//後処理
	static void Fin();

	//効果音の読み込み
	static void LoadSE(SEKind kind);
	//効果音の再生
	static void PlaySE(SEKind kind);
	//効果音ループ
	static void LoopSE(SEKind kind);
	//効果音停止
	static void StopSE(SEKind kind);

	//BGM読み込み
	static void LoadBGM(BGMKind kind);
	//BGM再生
	static void PlayBGM(BGMKind kind);
	//BGMの停止
	static void StopBGM(BGMKind kind);

	//再生時間取得（ミリ秒）
	static int GetSETime(SEKind iID);
	static int GetBGMTime(BGMKind iID);

	//総再生時間取得（ミリ秒）
	static int GetSEAllTime(SEKind iID);
	static int GetBGMAllTime(BGMKind iID);

	//ボリューム
	static void SetSEVolume(SEKind iID, float Vol);
	static void SetBGMVolume(BGMKind iID, float Vol);

	//サーチ関数
	static SoundNode* SearchSE(SEKind kind);
	static SoundNode* SearchBGM(BGMKind kind);

	//サーチhandle
};