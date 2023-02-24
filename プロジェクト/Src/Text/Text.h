#pragma once

#include "../Common.h"

class CText
{
private:
	int m_handle;
	int m_currentCount;
	int m_waitTime;
	int m_time;
	int m_textAmount;		//\1行に表示できるテキスト量
	int m_fontSize;
	int m_color;

	const char* m_text;		//テキストのアドレス保存

	char* m_dispBuf;		//１行分の文字列をディスプレイに表示するためのバッファ

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
