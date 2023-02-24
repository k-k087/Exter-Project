#pragma once

#include "list"

#include "../UI.h"

using namespace std;

class CTitleUI : public CUI
{
private:
	int m_titleHandle;

	CTitleUI() :
		m_titleHandle(-1)
	{}

	~CTitleUI();

public:
	void Init();
	void Load();
	void Step();
	void Update();
	void Draw();
	void Fin();

	static CTitleUI* CreateTitleUI();

	bool StepStartPhase();
	void DrawStartPhase();

	int StepPause();
	void DrawPause();
	void SetBossHP(int hp);
};