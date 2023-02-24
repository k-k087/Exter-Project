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

//������
void CMouse::Init()
{
	for (int index = 0; index < MOUSE_BUF_LEN; index++)
	{
		m_currentBuf[index] = '\0';
		m_preBuf[index] = '\0';
	}

	SetMousePoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}

//�X�e�b�v
void CMouse::Step()
{
	//�O�t���[���̃}�E�X���ϐ��ɋL�^���Ă���
	char* cur_mouse = m_currentBuf;
	char* pre_mouse = m_preBuf;

	m_oldPosX = m_posX;
	m_oldPosY = m_posY;

	GetMousePoint(&m_posX, &m_posY);

	for (int index = 0; index < MOUSE_BUF_LEN; index++, cur_mouse++, pre_mouse++)
		* pre_mouse = *cur_mouse;

	//���݂̃}�E�X�����擾
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

//�������ꂽ
bool CMouse::IsPush(int mouse_code)
{
	char* cur_mouse = m_currentBuf;
	char* pre_mouse = m_preBuf;

	for (int index = 0; index < 3; index++, cur_mouse++, pre_mouse++) {
		if (mouse_code == 1) {
			//�O�t�����O�A���t�����P
			if (*pre_mouse == 0 && *cur_mouse == 1)
				return true;

			return false;
		}
		else
			mouse_code = mouse_code >> 1;
	}

	return false;
}

//�����������Ă���
bool CMouse::IsKeep(int mouse_code)
{
	char* cur_mouse = m_currentBuf;
	char* pre_mouse = m_preBuf;

	for (int index = 0; index < 3; index++, cur_mouse++, pre_mouse++) {
		if (mouse_code == 1) {
			//�O�t�����P�A���t�����P
			if (*pre_mouse == 1 && *cur_mouse == 1)
				return true;

			return false;
		}
		else
			mouse_code = mouse_code >> 1;
	}

	return false;
}

//���͂Ȃ��ꂽ
bool CMouse::IsRelease(int mouse_code)
{
	char* cur_mouse = m_currentBuf;
	char* pre_mouse = m_preBuf;

	for (int index = 0; index < 3; index++, cur_mouse++, pre_mouse++) {
		if (mouse_code == 1) {
			//�O�t�����P�A���t�����O
			if (*pre_mouse == 1 && *cur_mouse == 0)
				return true;

			return false;
		}
		else
			mouse_code = mouse_code >> 1;
	}

	return false;
}

//�P���ɉ����ꂽ��
bool CMouse::IsDown(int mouse_code)
{
	char* cur_mouse = m_currentBuf;
	char* pre_mouse = m_preBuf;

	for (int index = 0; index < 3; index++, cur_mouse++, pre_mouse++) {
		if (mouse_code == 1) {
			//���t�����P
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