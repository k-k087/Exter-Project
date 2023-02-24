#pragma once

#include "../Common.h"

//現在のシーン
enum SceneID
{
	SCENE_ID_NONE = -1,

	SCENE_ID_TITLE,		//タイトル
	SCENE_ID_SELECT,	//セレクト画面
	SCENE_ID_PLAY,		//プレイ
	SCENE_ID_GAMEOVER,	//ゲームオーバー
	SCENE_ID_CLEAR,		//クリア

	SCENE_ID_NUM,
};

//現在の処理
enum SceneFuncID
{
	SCENE_FUNC_ID_NONE = -1,

	SCENE_FUNC_ID_INIT,	//初期化
	SCENE_FUNC_ID_LOAD,	//ロード
	SCENE_FUNC_ID_LOOP,	//ループ
	SCENE_FUNC_ID_FIN,	//後処理

	SCENE_FUNC_ID_NUM,
};

//シーンクラス
class CScene
{
public:
	struct EffectInfo
	{
		VECTOR pos;
		int index;
		int count;
		float scale;
		float rot;
		float alpha;

		bool isUse;
	};

	static const int EFFECT_NUM = 10;


	//初期化
	virtual void Init() = 0;
	//ロード
	virtual void Load() = 0;
	//通常処理
	virtual void Step() = 0;
	//描画
	virtual void Draw() = 0;
	//後処理
	virtual void Fin() = 0;
	//ループ処理
	virtual void Loop()
	{
		Step();
		Draw();
	}

protected:
	EffectInfo m_effectInfo[EFFECT_NUM];
	int m_effectHandle[4];
	int m_curHandle;
	bool m_isCur;
	int	m_effectCount;

public:
	CScene():
		m_effectInfo{},
		m_effectHandle{-1},
		m_curHandle(-1),
		m_isCur(),
		m_effectCount()
	{};
	virtual ~CScene() = default;

public:
	void InitEffect();
	void LoadEffect();
	void FinEffect();

	void OccurEffect(float mouse_x, float mouse_y);
	void StepEffect();
	void DrawEffect();
};