#pragma once

#include "../Scene.h"
#include "../../Text/Text.h"

enum PlayPhase
{
	PLAY_PHASE_START,
	PLAY_PHASE_LOOP,
	PLAY_PHASE_FIN,

	PLAY_PHASE_PAUSE,

	PLAY_PHASE_NUM,
};

enum TutorialPhase
{
	TUTORIAL_START,
	TUTORIAL_AVOID,
	TUTORIAL_MOVE,	//カメラが（（笑））
	TUTORIAL_ENEMY,
	TUTORIAL_ATTACK,
	TUTORIAL_ATTACK2,
	TUTORIAL_STRONG_ATTACK,
	TUTORIAL_BOOST_GO,
	TUTORIAL_BOOST_ATTACK,
	TUTORIAL_END,

	TUTORIAL_NUM,
};

enum TutorialState
{
	TUTORIAL_STATE_STOP,
	TUTORIAL_STATE_TEXT,
	TUTORIAL_STATE_ACTION,

	TUTORIAL_STATE_NUM,
};

//プレイシーンクラス
class CPlayScene : public CScene
{
private:
	PlayPhase	m_phase;
	PlayPhase	m_oldPhase;
	int			m_startcount;
	bool		m_isStart;

	CText		m_textInfo;
	int			m_textCount;
	bool		m_isText;
	int			m_TextBackHandle;

	int			m_tutorialPhase;
	TutorialState m_tutorialState;
	bool		m_isTutorial;
	bool		m_isCurrentTutorialEnd;
	int			m_playCount;
	int			m_stopCount;
	int			m_finCount;
	bool		m_finCountFlg;
	bool		m_checkTutorial[2];

	int			m_fontHandle;

public:

	CPlayScene();
	~CPlayScene();

	//オーバーライド
	void Init() override;
	void Load() override;
	void Step() override;
	void Draw() override;
	void Loop() override;
	void Fin()  override;

public:
	void ChangePhase(PlayPhase phase);

	void StepTutorial();

	void StepText();
	void DrawTextData();

	bool CheckTutorial();


	void StepStart();
	void StepLoop();
	void StepFin();
	void StepPause();
	void (CPlayScene::* PlayPhaseFunc[PLAY_PHASE_NUM])() =
	{
		&CPlayScene::StepStart,
		&CPlayScene::StepLoop,
		&CPlayScene::StepFin,
		&CPlayScene::StepPause,
	};

	void DrawStart();
	void DrawLoop();
	void DrawFin();
	void DrawPause();
	void (CPlayScene::* DrawPhaseFunc[PLAY_PHASE_NUM])() =
	{
		&CPlayScene::DrawStart,
		&CPlayScene::DrawLoop,
		&CPlayScene::DrawFin,
		&CPlayScene::DrawPause,
	};

	void GoLoop();

	//クリア判定
	static void JudgementClear();
};

