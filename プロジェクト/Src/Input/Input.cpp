/*
#include <limits.h>
#include <cassert>
#include "DxLib.h"
#include "Input.h"
#include "KeyBoard/KeyBoard.h"
#include "Mouse/Mouse.h"
#include "Pad/Pad.h"
#include "../MyMath/MyMath.h"

typedef bool (*IFunc[INPUT_KIND_NUM])(int);

static const IFunc IsPushFunc =
{
	IsKeyPush,
	IsMouseButtonPush,
	IsPadButtonPush,
};

static const IFunc IsKeepFunc =
{
	IsKeyKeep,
	IsMouseButtonKeep,
	IsPadButtonKeep,
};

static const IFunc IsReleaseFunc =
{
	IsKeyRelease,
	IsMouseButtonRelease,
	IsPadButtonRelease,
};

static const IFunc IsDownFunc =
{
	IsKeyDown,
	IsMouseButtonDown,
	IsPadButtonDown,
};

//���͐��䏉����
void InitInput()
{
	//�L�[��񏉊���
	InitKey();

	//�}�E�X��񏉊���
	InitMouse();

	//�p�b�h��񏉊���
	InitPad();
}

//���͐���X�e�b�v
//����Step��葁���Ă�
void StepInput()
{
	//�L�[�X�e�b�v
	StepKey();

	//�}�E�X�X�e�b�v
	StepMouse();
	
	//�p�b�h�X�e�b�v
	StepPad();
}

//�������ꂽ
bool IsPush(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);
	if ((*IsPushFunc[kind])(code))
		return true;

	return false;
}
//�����Ă��邩
bool IsKeep(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);
	if ((*IsKeepFunc[kind])(code))
		return true;
	
	return false;
}
//�����ꂽ
bool IsRelease(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);
	if ((*IsReleaseFunc[kind])(code))
		return true;

	return false;
}
//�P���ɉ�����Ă��邩
bool IsDown(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);
	if ((*IsDownFunc[kind])(code))
		return true;

	return false;
}
*/