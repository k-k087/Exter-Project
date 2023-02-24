#pragma once

#include "../Common.h"

class CText
{
private:
	int m_handle;
	int m_currentCount;
	int m_waitTime;
	int m_time;
	int m_textAmount;		//\1�s�ɕ\���ł���e�L�X�g��
	int m_fontSize;
	int m_color;

	const char* m_text;		//�e�L�X�g�̃A�h���X�ۑ�

	char* m_dispBuf;		//�P�s���̕�������f�B�X�v���C�ɕ\�����邽�߂̃o�b�t�@

	bool m_isEnd;

public:

	CText();
	~CText();


	bool Load(char* fontname = nullptr, int size = -1, int shick = -1, int type = -1);

	void Request(const char* text, int wait);

	void Update();

	void Draw();

	void Reset();

	bool IsEnd();

	void SetAllText();

	void SetAmount(const int amount)
	{
		if (!m_dispBuf)delete[] m_dispBuf;
		m_textAmount = amount;
		m_dispBuf = new char[m_textAmount];
	}

	void SetColor(int r, int g, int b)
	{
		m_color = GetColor(r, g, b);
	}

	
};
