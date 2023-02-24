#ifndef __PAD_H__
#define __PAD_H__

#include "DxLib.h"

#include "../Input.h"

enum StickKind
{
	LEFT_STICK_X,		//左スティックのX
	LEFT_STICK_Y,		//左スティックのY
	RIGHT_STICK_X,		//右スティックのX
	RIGHT_STICK_Y,		//右スティックのY

	STICK_MAX_NUM,
};

enum TriggerKind
{
	TRRIGER_LEFT,		//トリガーの左
	TRRIGER_RIGHT,		//トリガーの右

	TRIGGER_MAX_NUM,
};

class CPad : public CInput
{
private:
	static CPad* m_padInfo;

public:
	static const int PAD_BUF_LEN = 16;

private:
	XINPUT_STATE m_pad;							//パッド情報を受け取る構造体
	unsigned char m_currentBuf[PAD_BUF_LEN];	//現在のフレームのパッドのボタン情報
	unsigned char m_preBuf[PAD_BUF_LEN];		//前フレームのパッドのボタン情報
	float m_stickBuf[STICK_MAX_NUM];			//パッドのスティック情報
	float m_triggerBuf[TRIGGER_MAX_NUM];		//コントローラーのトリガー情報格納配列

public:
	static CPad* CreatePad();
	static CPad* GetPad() { return m_padInfo; }

	//初期化
	void Init() override;
	//ステップ
	void Step() override;

	//今押された
	bool IsPush(int button_code) override;
	//押しているか
	bool IsKeep(int button_code) override;
	//離された
	bool IsRelease(int button_code) override;
	//単純に押されているか
	bool IsDown(int button_code) override;

	//スティック値ゲット
	float GetStickBuf(StickKind kind)const { return m_stickBuf[kind]; }
	//スティックが入力されているか
	bool IsLeftStick();
	bool IsRightStick();
	//トリガー値ゲット
	float GetTriggerBuf(TriggerKind kind)const { return m_triggerBuf[kind]; }
};


#endif