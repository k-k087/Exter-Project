#include "DxLib.h"
#include "Pad.h"
#include "../../Common.h"
#include "../../MyMath/MyMath.h"

CPad* CPad::m_padInfo = nullptr;

CPad* CPad::CreatePad()
{
	if(!m_padInfo)
		m_padInfo = _NEW(CPad);

	return m_padInfo;
}

//������
void CPad::Init()
{
	for (int index = 0; index < PAD_BUF_LEN; index++)
	{
		m_currentBuf[index] = '\0';
		m_preBuf[index] = '\0';
	}
	for (int index = 0; index < STICK_MAX_NUM; index++)
	{
		m_stickBuf[index] = 0.f;
	}
	for (int index = 0; index < TRIGGER_MAX_NUM; index++)
	{
		m_triggerBuf[index] = 0.f;
	}
}

//�X�e�b�v
void CPad::Step()
{
	unsigned char* cur_pad = m_currentBuf;
	unsigned char* pre_pad = m_preBuf;

	//�O�t���[���̃R���g���[���[�{�^�����X�V
	for (int index = 0; index < PAD_BUF_LEN; index++, cur_pad++, pre_pad++)
		*pre_pad = *cur_pad;
	XINPUT_STATE* pad = &m_pad;

	//���͏�Ԏ擾
	GetJoypadXInputState(DX_INPUT_PAD1, pad);

	//�{�^��
	unsigned char* pad_button = pad->Buttons;
	cur_pad = m_currentBuf;
	for (int index = 0; index < PAD_BUF_LEN; index++, cur_pad++, pad_button++)
		*cur_pad = *pad_button;

	//�g���K�[�ƃX�e�B�b�N�͐��K������
	m_stickBuf[LEFT_STICK_X] = (float)pad->ThumbLX / SHRT_MAX;
	m_stickBuf[LEFT_STICK_Y] = (float)pad->ThumbLY / SHRT_MAX;
	m_stickBuf[RIGHT_STICK_X] = (float)pad->ThumbRX / SHRT_MAX;
	m_stickBuf[RIGHT_STICK_Y] = (float)pad->ThumbRY / SHRT_MAX;
	m_triggerBuf[TRRIGER_LEFT] = pad->LeftTrigger / 255.f;
	m_triggerBuf[TRRIGER_RIGHT] = pad->RightTrigger / 255.f;

#if 1
	//�X�e�B�b�N�̓��͂������0.1�ȉ��Ȃ�0�ɂ���
	for (int stick_index = 0; stick_index < STICK_MAX_NUM; stick_index+=2)
	{
		if (m_stickBuf[stick_index] <= 0.1f && m_stickBuf[stick_index] >= -0.1f &&
			m_stickBuf[stick_index+1] <= 0.1f && m_stickBuf[stick_index+1] >= -0.1f)
			m_stickBuf[stick_index] = m_stickBuf[stick_index+1] = 0;
	}
#endif

}

//�O�t���ŉ�����ĂȂ� ���� ���t���ŉ�����Ă���
bool CPad::IsPush(int button_code)
{
	if (m_preBuf[button_code] == 0 && m_currentBuf[button_code] == 1)
		return true;

	return false;
}

//�O�t���ŉ�����Ă��� ���� ���t���ŉ�����Ă���
bool CPad::IsKeep(int button_code)
{
	if (m_preBuf[button_code] == 1 && m_currentBuf[button_code] == 1)
		return true;

	return false;
}

//�O�t���ŉ�����Ă��� ���� ���t���ŉ�����ĂȂ�
bool CPad::IsRelease(int button_code)
{
	if (m_preBuf[button_code] == 1 && m_currentBuf[button_code] == 0)
		return true;

	return false;
}

//�P���ɉ�����Ă��邩
bool CPad::IsDown(int button_code)
{
	if (m_currentBuf[button_code] == 1)
		return true;

	return false;
}

//�X�e�B�b�N�����͂���Ă��邩
bool CPad::IsLeftStick()
{
	if (m_stickBuf[LEFT_STICK_X] > 0 || m_stickBuf[LEFT_STICK_X] < 0 ||
		m_stickBuf[LEFT_STICK_Y] > 0 || m_stickBuf[LEFT_STICK_Y] < 0)
		return true;

	return false;
}
bool CPad::IsRightStick()
{
	if (m_stickBuf[RIGHT_STICK_X] > 0 || m_stickBuf[RIGHT_STICK_X] < 0 ||
		m_stickBuf[RIGHT_STICK_Y] > 0 || m_stickBuf[RIGHT_STICK_Y] < 0)
		return true;

	return false;
}