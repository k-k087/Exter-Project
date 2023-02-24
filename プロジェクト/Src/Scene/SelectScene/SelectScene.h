#pragma once

#include "../Scene.h"

enum StageKind
{
	STAGE_KIND_NONE = -1,

	STAGE_KIND_TUTORIAL,

	STAGE_KIND_1,
	STAGE_KIND_2,
	STAGE_KIND_3,

	STAGE_KIND_EX,

	STAGE_KIND_NUM,
};

//タイトルシーンクラス
class CSelectScene : public CScene
{
public:
	enum SelectKind
	{
		SELECT_KIND_NONE = -1,

		SELECT_KIND_TUTORIAL,
		SELECT_KIND_STAGE_1,
		SELECT_KIND_STAGE_2,
		SELECT_KIND_STAGE_3,
		SELECT_KIND_STAGE_EX,
		SELECT_KIND_BACK_TITLE,

		SELECT_KIND_NUM,
	};

	struct EffectInfo
	{
		VECTOR pos;
		int index;
		int count;
		float scale;
		float rot;

		bool isUse;
	};

	static const int EFFECT_NUM = 10;

private:
	int m_handle;
	int m_selectHandle[SELECT_KIND_NUM];
	int m_sceneCount;

	int m_cur;

	bool m_isSE;

	StageKind m_stageKind;
public:
	CSelectScene() :
		m_selectHandle{ -1 },
		m_stageKind(STAGE_KIND_NONE),
		m_handle(-1),
		m_sceneCount(0)
	{
	}


	//オーバーライド
	void Init() override;
	void Load() override;
	void Step() override;
	void Draw() override;
	void Loop() override;
	void Fin()  override;
};

