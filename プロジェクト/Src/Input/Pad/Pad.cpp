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

//初期化
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

//ステップ
void CPad::Step()
{
	unsigned char* cur_pad = m_currentBuf;
	unsigned char* pre_pad = m_preBuf;

	//前フレームのコントローラーボタン情報更新
	for (int index = 0; index < PAD_BUF_LEN; index++, cur_pad++, pre_pad++)
		*pre_pad = *cur_pad;
	XINPUT_STATE* pad = &m_pad;

	//入力状態取得
	GetJoypadXInputState(DX_INPUT_PAD1, pad);

	//ボタン
	unsigned char* pad_button = pad->Buttons;
	cur_pad = m_currentBuf;
	for (int index = 0; index < PAD_BUF_LEN; index++, cur_pad++, pad_button++)
		*cur_pad = *pad_button;

	//トリガーとスティックは正規化する
	m_stickBuf[LEFT_STICK_X] = (float)pad->ThumbLX / SHRT_MAX;
	m_stickBuf[LEFT_STICK_Y] = (float)pad->ThumbLY / SHRT_MAX;
	m_stickBuf[RIGHT_STICK_X] = (float)pad->ThumbRX / SHRT_MAX;
	m_stickBuf[RIGHT_STICK_Y] = (float)pad->ThumbRY / SHRT_MAX;
	m_triggerBuf[TRRIGER_LEFT] = pad->LeftTrigger / 255.f;
	m_triggerBuf[TRRIGER_RIGHT] = pad->RightTrigger / 255.f;

#if 1
	//スティックの入力した情報が0.1以下なら0にする
	for (int stick_index = 0; stick_index < STICK_MAX_NUM; stick_index+=2)
	{
		if (m_stickBuf[stick_index] <= 0.1f && m_stickBuf[stick_index] >= -0.1f &&
			m_stickBuf[stick_index+1] <= 0.1f && m_stickBuf[stick_index+1] >= -0.1f)
			m_stickBuf[stick_index] = m_stickBuf[stick_index+1] = 0;
	}
#endif

}

//前フレで押されてない かつ 現フレで押されている
bool CPad::IsPush(int button_code)
{
	if (m_preBuf[button_code] == 0 && m_currentBuf[button_code] == 1)
		return true;

	return false;
}

//前フレで押されている かつ 現フレで押されている
bool CPad::IsKeep(int button_code)
{
	if (m_preBuf[button_code] == 1 && m_currentBuf[button_code] == 1)
		return true;

	return false;
}

//前フレで押されている かつ 現フレで押されてない
bool CPad::IsRelease(int button_code)
{
	if (m_preBuf[button_code] == 1 && m_currentBuf[button_code] == 0)
		return true;

	return false;
}

//単純に押されているか
bool CPad::IsDown(int button_code)
{
	if (m_currentBuf[button_code] == 1)
		return true;

	return false;
}

//スティックが入力されているか
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