#include "Score.h"
#include "DxLib.h"
#include "../Input/InputManager.h"

#include "../Scene/SceneManager.h"

int CScore::m_score;
int CScore::m_memoryScore;
int CScore::m_handle[NUMBER_NUM];
int CScore::m_disp_score;

const int ADD_SCORE_SPD = 50;	//�X�R�A���Z����

const int DIGIT_NUMBER = 6;	//�\������

//�v���C���`��J�n�ʒu
const int DRAW_START_X = 1250;	
const int DRAW_START_Y = 20;

//	�N���A���`��J�n�ʒu
const int CLEAR_DRAW_START_X = 700;
const int CLEAR_DRAW_START_Y = 150;

//�R���X�g���N�^
CScore::CScore()
{

}
//�f�X�g���N�^
CScore::~CScore()
{

}

//	������
void CScore::Init() 
{
	m_score = 0;
	m_memoryScore = 0;
	m_disp_score = 0;
	for (int score_index = 0; score_index < NUMBER_NUM; ++score_index)
		m_handle[score_index] = -1;
}

//	�摜�ǂ݂���
void CScore::Load()
{
	LoadDivGraph("Data/Graph/Score/score.png", NUMBER_NUM, 11, 1, NUMBER_SIZE_X, NUMBER_SIZE_Y, m_handle);
}

//�X�V����
void CScore::Update()
{
	/*	�X�R�A��`��p�ɕϊ�	*/

	if (m_disp_score < m_score)
	{
		m_disp_score += ADD_SCORE_SPD;

		if (m_disp_score > m_score)
			m_disp_score = m_score;
	}
}

//���U���g�V�[���X�R�A�X�V����
bool CScore::ResultUpdate()
{
	bool is_end = false;

	if (m_memoryScore == 0)
	{
		//�X�R�A��ۑ��p�ϐ��ɑ��
		m_memoryScore = m_score;
		//m_score����x���Z�b�g
		CScore::ResetScore();
	}

	//�G���^�L�[����������ۂ̃X�R�A�\��
	if (CInputManager::IsPush(KEY_INPUT_RETURN, I_KEY))
	{
		m_score = m_memoryScore;
		is_end = true;
	}

	//���ۂ̃X�R�A�𒴂���܂�m_score�����Z���Ȃ���
	if (m_score < m_memoryScore)
	{
		CScore::AddScore(10);

		if (m_score >= m_memoryScore)
		{
			CScore::SetScore(m_score);
			is_end = true;
		}

	}
	//��������X�R�A�����ۂ̃X�R�A�̒l�ɒ����ăZ�b�g
	else
	{
		CScore::SetScore(m_score);
		is_end = true;
	}

	return is_end;
}

//�㏈��
void CScore::Fin()
{
	for (int index = 0; index < NUMBER_NUM; index++)
		DeleteGraph(m_handle[index]);
}

//	�X�R�A���Z
void CScore::AddScore(const int score)
{
	m_score += score;
}

//	�v���C���̃X�R�A�\��
void CScore::DrawScorePlay()
{
	//�`��p�X�R�A���ꎞ�I�Ɋi�[
	int temp_score = m_disp_score;

	for (int index = 0; index < DIGIT_NUMBER; index++)
	{
		//10�Ŋ������]����i�[
		int num = temp_score % 10;

		//�]��̐����̉摜��`��
		DrawRotaGraph(DRAW_START_X - (NUMBER_SIZE_X * index)-20, DRAW_START_Y,
			1.0, 0.0, m_handle[num], true);

		//�������炷
		temp_score /= 10;
	}

	DrawRotaGraph(DRAW_START_X, DRAW_START_Y, 1.0, 0.0, m_handle[10], true);
}

//	���U���g���̃X�R�A�\��
void CScore::DrawScoreResult()
{
	//�`��p�X�R�A���ꎞ�I�Ɋi�[
	int temp_score = m_score;

	for (int index = 0; index < DIGIT_NUMBER; index++)
	{
		//10�Ŋ������]����i�[
		int num = temp_score % 10;

		//�]��̐����̉摜��`��
		DrawRotaGraph(CLEAR_DRAW_START_X - (NUMBER_SIZE_X * 2 * index), CLEAR_DRAW_START_Y,
			2.0, 0.0, m_handle[num], true);
		

		//�������炷
		temp_score /= 10;
	}

	DrawRotaGraph(CLEAR_DRAW_START_X + 40, CLEAR_DRAW_START_Y, 2.0, 0.0, m_handle[10], true);
}

//�X�R�A�Z�b�g
void CScore::SetScore(int score)
{
	m_score = score;
}

//�X�R�A���Z�b�g
void CScore::ResetScore()
{
	m_score = 0;
}