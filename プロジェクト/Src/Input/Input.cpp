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

//入力制御初期化
void InitInput()
{
	//キー情報初期化
	InitKey();

	//マウス情報初期化
	InitMouse();

	//パッド情報初期化
	InitPad();
}

//入力制御ステップ
//他のStepより早く呼ぶ
void StepInput()
{
	//キーステップ
	StepKey();

	//マウスステップ
	StepMouse();
	
	//パッドステップ
	StepPad();
}

//今押された
bool IsPush(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);
	if ((*IsPushFunc[kind])(code))
		return true;

	return false;
}
//押しているか
bool IsKeep(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);
	if ((*IsKeepFunc[kind])(code))
		return true;
	
	return false;
}
//離された
bool IsRelease(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);
	if ((*IsReleaseFunc[kind])(code))
		return true;

	return false;
}
//単純に押されているか
bool IsDown(int code, InputKind kind)
{
	assert(INPUT_KIND_NONE < kind && kind < INPUT_KIND_NUM);
	if ((*IsDownFunc[kind])(code))
		return true;

	return false;
}
*/