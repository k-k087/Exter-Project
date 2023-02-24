#include "Text.h"

#include "../Common.h"
#include "../Sound/SoundManager.h"

static const int DEF_FONT_SIZE = 16;
static const int TEXT_LENGTH = 256;

CText::CText():
	m_handle(-1),
	m_currentCount(-1),
	m_waitTime(-1),
	m_time(-1),
	m_textAmount(TEXT_LENGTH),
	m_fontSize(DEF_FONT_SIZE),
	m_color(GetColor(255,255,255)),
	m_text(nullptr),
	m_dispBuf(new(char[m_textAmount])),
	m_isEnd(true)
{

}
CText::~CText()
{
	
	delete[] m_dispBuf;
	m_dispBuf = nullptr;
	
	if (m_handle != -1)
	{
		DeleteFontToHandle(m_handle);
		m_handle = -1;
	}
}


bool CText::Load(char* fontname, int size, int shick, int type)
{
	if (size == -1)m_fontSize = DEF_FONT_SIZE;
	else m_fontSize = size;

	m_handle = CreateFontToHandle(fontname, 40, shick, DX_FONTTYPE_ANTIALIASING);

	return m_handle != -1;
}

void CText::Request(const char* text, int wait)
{
	m_text = text;
	m_waitTime = m_time = wait;
	
	if (wait <= 0)
	{
		m_currentCount = static_cast<int>(strlen(text));
		m_isEnd = true;
	}
	else
	{
		m_currentCount = 0;
		m_isEnd = false;
	}
}

void CText::Update()
{
	if (!m_text) { return; }
	if (m_isEnd) { return; }

	--m_time;

	if (m_time == 0)
	{
		int count = GetCharBytes(DX_CHARCODEFORMAT_SHIFTJIS, &m_text[m_currentCount]);

		if (m_text[m_currentCount + count] == '\n')count++;

		CSoundManager::PlaySE(SE_KIND_SELECT1);

		m_currentCount += count;
		m_time = m_waitTime;
	}
}

void CText::Draw()
{
	if (!m_text) { return; }

	int count = 0;
	int col = 0;
	bool is_end = false;

	while (!is_end)
	{
		memset(m_dispBuf, 0, sizeof(char) * m_textAmount);

		int text_index = 0;

		for (; text_index < m_textAmount - 1; ++text_index)
		{
			if (m_text[count + text_index] == '\n' || count + text_index >= m_currentCount)
				break;
			else if (m_text[count + text_index] == '\0')
			{
				is_end = m_isEnd = true;
				break;
			}
		}

		memcpy(m_dispBuf, &m_text[count], text_index);
		m_dispBuf[text_index] = '\0';

		DrawFormatStringToHandle(50, 530 + col * (m_fontSize + 50) , m_color, m_handle,
			m_dispBuf);

		if (m_text[count + text_index] == '\n')
			count += text_index + 1;
		else
			count += text_index;


		if (m_currentCount <= count)
			is_end = true;
		else
			++col;
	}
}

void CText::Reset()
{
	m_text = nullptr;
}

bool CText::IsEnd()
{
	if (m_text == nullptr)return false;
	else return m_isEnd;
}

void CText::SetAllText()
{
	if (!m_text) { return; }

	m_currentCount = static_cast<int>(strlen(m_text));
	m_isEnd = true;
}