#pragma once

#include "../Scene.h"

//GameOver�V�[���N���X
class CGameOverScene : public CScene
{
private:
	int m_handle;
public:

	//�I�[�o�[���C�h
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


