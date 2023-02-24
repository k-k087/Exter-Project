#include "DxLib.h"
#include "KeyBoard.h"

//������
void CKeyBoard::Init()
{
	for (int index = 0; index < KEY_BUF_LEN; index++)
	{
		m_currentBuf[index] = '\0';
		m_preBuf[index] = '\0';
	}
}

//�X�e�b�v
void CKeyBoard::Step()
{
	//�O�t���[���̃L�[���ϐ��ɋL�^���Ă���
	char* cur_key = m_currentBuf;
	char* pre_key = m_preBuf;

	for (int index = 0; index < KEY_BUF_LEN; index++, cur_key++, pre_key++)
		* pre_key = *cur_key;

	//���݂̃L�[�����擾
	GetHitKeyStateAll(m_currentBuf);
}

//�������ꂽ
bool CKeyBoard::IsPush(int key_code)
{
	//�O�t���ŉ�����ĂȂ� ���� ���t���ŉ�����Ă���
	if (m_preBuf[key_code] == 0 && m_currentBuf[key_code] == 1)
		return true;

	//������ĂȂ��̂� false
	return false;
}
//�����ꑱ���Ă���
bool CKeyBoard::IsKeep(int key_code)
{
	//�O�t���ł�����Ă��� ���� ���t���ŉ�����Ă���
	if (m_preBuf[key_code] == 1 && m_currentBuf[key_code] == 1)
		return true;

	return false;
}
//�����ꂽ
bool CKeyBoard::IsRelease(int key_code)
{
	//�O�t���ŉ�����Ă��� ���� ���t���ŉ�����Ă��Ȃ�
	if (m_preBuf[key_code] == 1 && m_currentBuf[key_code] == 0)
		return true;

	return false;
}
//�P���ɉ�����Ă��邩
bool CKeyBoard::IsDown(int key_code)
{
	//���t���ŉ�����Ă���i�O�t���̏�Ԃ͊֌W�Ȃ��j
	if (m_currentBuf[key_code] == 1)
		return true;

	return false;
}