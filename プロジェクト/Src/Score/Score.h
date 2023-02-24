#pragma once 

const int NUMBER_NUM = 11;	//�����̐�

const int NUMBER_SIZE_X = 20;

const int NUMBER_SIZE_Y = 32;


class CScore
{
private:
	static int m_score;	//�X�R�A
	static int m_memoryScore;	//���ۂ̃X�R�A�ۑ��p�ϐ�
	static int m_handle[NUMBER_NUM];	//�����摜�n���h��
	static int m_disp_score;

public:
	CScore();
	~CScore();

	//	������
	static void Init();

	//	�摜�ǂ݂���
	static void Load();

	//	�X�V
	static void Update();

	//	�㏈��
	static void Fin();

	//	�X�R�A���Z
	static void AddScore(const int score);

	//	���U���g���̃X�R�A�X�V����
	static bool ResultUpdate();
	
	//	�v���C���̃X�R�A�\��
	static void DrawScorePlay();

	//	���U���g���̃X�R�A�\��
	static void DrawScoreResult();

	//�X�R�A�Z�b�g
	static void SetScore(int score);

	static int GetScore() { return m_score; }

	//�X�R�A���Z�b�g
	static void ResetScore();
};