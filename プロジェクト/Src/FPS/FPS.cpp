#include "FPS.h"
#include "DxLib.h"
#include "../Common.h"

int CFrameRate::currentTime;	//���݂̎���
int CFrameRate::lastFrameTime;	//�O��̃t���[�����s���̎���
int CFrameRate::count;			//�t���[���J�E���g�p
int CFrameRate::calcFpsTime;	//FPS���v�Z��������
float CFrameRate::fps;			//�v������FPS�i�\���p�j

//FPS������
void CFrameRate::Init()
{
	currentTime = 0;
	lastFrameTime = 0;
	count = 0;
	calcFpsTime = 0;
	fps = 0.f;
}

//FPS�X�e�b�v
void CFrameRate::Step()
{
	//���݂̎��Ԃ��擾
	currentTime = GetNowCount();

	//�ŏ��̃��[�v�Ȃ�
	//���݂̎��Ԃ��AFPS�̌v�Z���������Ԃɐݒ�
	if (calcFpsTime == 0.0f)
	{
		calcFpsTime = currentTime;
		fps = (float)FRAME_RATE;
	}
}

//�t���[�����[�g���X�V
void CFrameRate::Update()
{
	lastFrameTime = currentTime;
	count++;
}

//1/60�b�o�߂��Ă��邩�̔���
bool CFrameRate::IsPassageFrameTime()
{
	if (currentTime - lastFrameTime >= FRAME_TIME)
		return true;

	else return false;
}

//FPS�v�Z
void CFrameRate::Calc()
{
	//�O���FPS���v�Z�������Ԃ���̌o�ߎ��Ԃ����߂�
	int difTime = currentTime - calcFpsTime;

	//�O���FPS���v�Z�������Ԃ���
	//�P�b�ȏ�o�߂��Ă�����FPS���v�Z����
	if (difTime > 1000)
	{
		//�t���[���񐔂��~���b�ɍ��킹��
		//�����܂ŏo�������̂�float�ɃL���X�g
		float frameCount = (float)(count * 1000.0f);

		//FPS�����߂�
		//���z�̐��l�� 60000 / 1000 �� 60 �ƂȂ�
		fps = frameCount / difTime;

		//�t���[�����[�g�J�E���g���N���A
		count = 0;

		//FPS���v�Z�������Ԃ��X�V
		calcFpsTime = currentTime;
	}
}

//FPS�\���i�f�o�b�O�p�j
void CFrameRate::Draw()
{
	unsigned int color = GetColor(255, 30, 30);
	DrawFormatString((int)(WINDOW_WIDTH - 100), (int)(WINDOW_HEIGHT - 20), color, "FPS[%.2f]", fps);
}

