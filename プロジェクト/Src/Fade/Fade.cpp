#include "Fade.h"

#include "../Common.h"

CFadeManager* CFadeManager::m_fadeManager = nullptr;

void CFadeManager::DeleteManager()
{
	if (m_fadeManager)
	{
		_DELETE(m_fadeManager);
		m_fadeManager = nullptr;
	}
}



void CFadeManager::Request(float speed, bool isFadeIN)
{
	m_speed = speed;

	m_state = isFadeIN ? FADE_IN : FADE_OUT;
	m_count = isFadeIN ? 255.f : 0.f;

}

void CFadeManager::Update()
{
	switch (m_state)
	{
	case FADE_IN:
		m_count -= m_speed;
		if (m_count < 0.f)m_count = 0.f;
		break;

	case FADE_OUT:
		m_count += m_speed;
		if (m_count > 255.f)m_count = 255.f;
		break;
	}
}

void CFadeManager::Draw()
{
	if (m_state == FADE_NON) { return; }

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(m_count));
	DrawBox(0, 0, m_windowX, m_windowY, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool CFadeManager::IsEnd()
{
	bool is_ret = true;

	switch (m_state)
	{
	case FADE_IN:
		if (m_count > 0.f)is_ret = false;
		break;

	case FADE_OUT:
		if (m_count < 255.f)is_ret = false;
		break;
	}

	return is_ret;
}


CFadeManager::CFadeManager() : 
	m_count(),
	m_speed(),
	m_state(FADE_NON),
	m_windowX(WINDOW_WIDTH),
	m_windowY(WINDOW_HEIGHT)

{

}
CFadeManager::~CFadeManager()
{

}