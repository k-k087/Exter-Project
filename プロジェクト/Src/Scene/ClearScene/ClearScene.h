#pragma once

#include "../Scene.h"
enum ClearPhase
{
	CLEAR_PHASE_SCORE_AND_NAME,
	CLEAR_PHASE_RANKING,

	CLEAR_PHASE_NUM,
};

//GameOverシーンクラス
class CClearScene : public CScene
{
private:
	int m_handle[2];
	int m_fontHandle[2];
	bool m_isEndScore;
	ClearPhase m_phase;
public:

	//オーバーライド
	void Init() override;
	void Load() override;
	void Step() override;
	void Draw() override;
	void Loop() override;
	void Fin()  override;

	CClearScene() :
		m_handle{-1},
		m_fontHandle{ -1 }
	{}

	void StepScore();
	void StepRinking();
	void (CClearScene::* StepPhaseFunc[CLEAR_PHASE_NUM])() =
	{
		&CClearScene::StepScore,
		&CClearScene::StepRinking,
	};

	void DrawScore();
	void DrawRinking();
	void (CClearScene::* DrawPhaseFunc[CLEAR_PHASE_NUM])() =
	{
		&CClearScene::DrawScore,
		&CClearScene::DrawRinking,
	};
};