#ifndef __FPS_H__
#define __FPS_H__

class CFrameRate
{
public:
	static int currentTime;		//���݂̎���
	static int lastFrameTime;	//�O��̃t���[�����s���̎���
	static int count;			//�t���[���J�E���g�p
	static int calcFpsTime;		//FPS���v�Z��������
	static float fps;			//�v������FPS�i�\���p�j

	//FPS������
	static void Init();

	//FPS�X�e�b�v
	static void Step();

	//�t���[�����[�g���X�V
	static void Update();

	//1/60�b�o�߂��Ă��邩�̔���
	static bool IsPassageFrameTime();

	//FPS�v�Z
	static void Calc();

	//FPS�\���i�f�o�b�O�p�j
	static void Draw();
};



#endif
