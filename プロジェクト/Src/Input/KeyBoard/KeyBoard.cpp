#include "DxLib.h"
#include "KeyBoard.h"

//初期化
void CKeyBoard::Init()
{
	for (int index = 0; index < KEY_BUF_LEN; index++)
	{
		m_currentBuf[index] = '\0';
		m_preBuf[index] = '\0';
	}
}

//ステップ
void CKeyBoard::Step()
{
	//前フレームのキー情報変数に記録しておく
	char* cur_key = m_currentBuf;
	char* pre_key = m_preBuf;

	for (int index = 0; index < KEY_BUF_LEN; index++, cur_key++, pre_key++)
		* pre_key = *cur_key;

	//現在のキー情報を取得
	GetHitKeyStateAll(m_currentBuf);
}

//今押された
bool CKeyBoard::IsPush(int key_code)
{
	//前フレで押されてない かつ 現フレで押されている
	if (m_preBuf[key_code] == 0 && m_currentBuf[key_code] == 1)
		return true;

	//押されてないので false
	return false;
}
//押され続けている
bool CKeyBoard::IsKeep(int key_code)
{
	//前フレでおされている かつ 現フレで押されている
	if (m_preBuf[key_code] == 1 && m_currentBuf[key_code] == 1)
		return true;

	return false;
}
//離された
bool CKeyBoard::IsRelease(int key_code)
{
	//前フレで押されていた かつ 現フレで押されていない
	if (m_preBuf[key_code] == 1 && m_currentBuf[key_code] == 0)
		return true;

	return false;
}
//単純に押されているか
bool CKeyBoard::IsDown(int key_code)
{
	//現フレで押されている（前フレの状態は関係なし）
	if (m_currentBuf[key_code] == 1)
		return true;

	return false;
}