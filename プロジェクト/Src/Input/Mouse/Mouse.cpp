#include "DxLib.h"
#include "Mouse.h"

#include "../../Common.h"
#include "../../MyMath/MyMath.h"


CMouse* CMouse::m_MouseInfo = nullptr;

CMouse* CMouse::CreateMouse()
{
	if (!m_MouseInfo)
		m_MouseInfo = _NEW(CMouse);

	return m_MouseInfo;
}

//初期化
void CMouse::Init()
{
	for (int index = 0; index < MOUSE_BUF_LEN; index++)
	{
		m_currentBuf[index] = '\0';
		m_preBuf[index] = '\0';
	}

	SetMousePoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}

//ステップ
void CMouse::Step()
{
	//前フレームのマウス情報変数に記録しておく
	char* cur_mouse = m_currentBuf;
	char* pre_mouse = m_preBuf;

	m_oldPosX = m_posX;
	m_oldPosY = m_posY;

	GetMousePoint(&m_posX, &m_posY);

	for (int index = 0; index < MOUSE_BUF_LEN; index++, cur_mouse++, pre_mouse++)
		* pre_mouse = *cur_mouse;

	//現在のマウス情報を取得
	cur_mouse = m_currentBuf;
	unsigned int mouse_start_input = MOUSE_INPUT_LEFT;
	unsigned int mouse_end_input = MOUSE_INPUT_MIDDLE;
	for (unsigned int mouse_input_type = mouse_start_input; mouse_input_type <= mouse_end_input;
		mouse_input_type = mouse_input_type << 1, cur_mouse++)
	{
		if ((GetMouseInput() & mouse_input_type) != 0)
			* cur_mouse = 1;
		else
			*cur_mouse = 0;
	}
}

VECTOR CMouse::GetDistance()
{
	VECTOR distance = { static_cast<float>(m_posX - WINDOW_WIDTH / 2) , static_cast<float>(m_posY - WINDOW_HEIGHT / 2) ,0.f };

	return distance;
}

//今押された
bool CMouse::IsPush(int mouse_code)
{
	char* cur_mouse = m_currentBuf;
	char* pre_mouse = m_preBuf;

	for (int index = 0; index < 3; index++, cur_mouse++, pre_mouse++) {
		if (mouse_code == 1) {
			//前フレ＝０、現フレ＝１
			if (*pre_mouse == 0 && *cur_mouse == 1)
				return true;

			return false;
		}
		else
			mouse_code = mouse_code >> 1;
	}

	return false;
}

//押し続けられている
bool CMouse::IsKeep(int mouse_code)
{
	char* cur_mouse = m_currentBuf;
	char* pre_mouse = m_preBuf;

	for (int index = 0; index < 3; index++, cur_mouse++, pre_mouse++) {
		if (mouse_code == 1) {
			//前フレ＝１、現フレ＝１
			if (*pre_mouse == 1 && *cur_mouse == 1)
				return true;

			return false;
		}
		else
			mouse_code = mouse_code >> 1;
	}

	return false;
}

//今はなされた
bool CMouse::IsRelease(int mouse_code)
{
	char* cur_mouse = m_currentBuf;
	char* pre_mouse = m_preBuf;

	for (int index = 0; index < 3; index++, cur_mouse++, pre_mouse++) {
		if (mouse_code == 1) {
			//前フレ＝１、現フレ＝０
			if (*pre_mouse == 1 && *cur_mouse == 0)
				return true;

			return false;
		}
		else
			mouse_code = mouse_code >> 1;
	}

	return false;
}

//単純に押されたか
bool CMouse::IsDown(int mouse_code)
{
	char* cur_mouse = m_currentBuf;
	char* pre_mouse = m_preBuf;

	for (int index = 0; index < 3; index++, cur_mouse++, pre_mouse++) {
		if (mouse_code == 1) {
			//現フレ＝１
			if (*cur_mouse == 1)
				return true;

			return false;
		}
		else
			mouse_code = mouse_code >> 1;
	}

	return false;
}

bool CMouse::IsMoveMousePos()
{
	if (m_posX == m_oldPosX &&
		m_posY == m_oldPosY)
	{
		return false;
	}

	return true;
}