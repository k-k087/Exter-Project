#pragma once

enum UIKind
{
	UI_KIND_NONE,

	UI_KIND_TITLE,
	UI_KIND_PLAY,

	UI_KIND_NUM,
};

class CUI
{
protected:
	UIKind m_kind;

	CUI() :
		m_kind(UI_KIND_NONE)
	{}
public:
	~CUI()
	{}

	virtual void Init()		= 0;
	virtual void Load()		= 0;
	virtual void Step()		= 0;
	virtual void Update()	= 0;
	virtual void Draw()		= 0;
	virtual void Fin()		= 0;

	virtual bool StepStartPhase() = 0;
	virtual void DrawStartPhase() = 0;

	virtual int StepPause() = 0;
	virtual void DrawPause() = 0;


	virtual void SetBossHP(int hp) = 0;
};