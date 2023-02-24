#pragma once

#include "../Common.h"

// �t�F�[�h�N���X
class CFadeManager
{
public:
	enum FadeState
	{
		FADE_NON,		//�t�F�[�h�Ȃ�
		FADE_IN,		//�t�F�[�h�C��
		FADE_OUT,		//�t�F�[�h�A�E�g
	};

private:
	static CFadeManager*	m_fadeManager;

	float m_count;
	float m_speed;
	int m_windowX;
	int m_windowY;

	FadeState	m_state;

public:

	static CFadeManager* CreateManager()
	{
		if (!m_fadeManager)
		{
			m_fadeManager = _NEW(CFadeManager);
		}

		return m_fadeManager;
	}

	static CFadeManager* GetManager()
	{
		if (m_fadeManager)
		{
			return m_fadeManager;
		}

		return nullptr;
	}

	void DeleteManager();


	void Reset() { m_state = FADE_NON; }


	void Request(float speed, bool isFadeIN);

	void Update();

	void Draw();

	bool IsEnd();

	int	GetState() { return m_state; }

	void SetWindowSize(int x, int y) { m_windowX = x; m_windowY = y; }


private:
	CFadeManager();
	~CFadeManager();

	//�R�s�[�R���X�g���N�^�A������Z�q
	CFadeManager(const CFadeManager &other);
	CFadeManager& operator= (const CFadeManager &other);
};