#pragma once

#include "../Scene.h"

//GameOverシーンクラス
class CGameOverScene : public CScene
{
private:
	int m_handle;
public:

	//オーバーライド
	void Init() override;
	void Load() override;
	void Step() override;
	void Draw() override;
	void Loop() override;
	void Fin()  override;

	CGameOverScene():
		m_handle(-1)
	{}
};


