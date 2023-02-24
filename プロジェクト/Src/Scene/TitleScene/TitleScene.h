#pragma once

#include "../Scene.h"

enum TitlePathKind
{
	TITLE_PATH_BACK1,
	TITLE_PATH_BACK2,
	TITLE_PATH_LEN,
	TITLE_PATH_SWORD,
	TITLE_PATH_SWORD_BACK,
	TITLE_PATH_PLAY_R,
	TITLE_PATH_PLAY_D,
	TITLE_PATH_SHUTDOWN_R,
	TITLE_PATH_SHUTDOWN_D,
	TITLE_PATH_CUR,

	TITLE_PATH_NUM,
};


//タイトルシーンクラス
class CTitleScene : public CScene
{
public:

private:
	int m_handle[TITLE_PATH_NUM];
	int m_sceneCount;
	float m_handleAlpha;
	float m_handleAlpha2;
	int m_alphaFlg;
	float m_handleRectX;
	bool m_isSE;

	int m_cur;

public:
	CTitleScene():
		m_handle{ -1 },
		m_sceneCount( ),
		m_handleAlpha( ),
		m_alphaFlg( ), 
		m_handleRectX( ),
		m_handleAlpha2( )
	{
	}


	//オーバーライド
	void Init() override;
	void Load() override;
	void Step() override;
	void Draw() override;
	void Loop() override;
	void Fin()  override;

private:
};

