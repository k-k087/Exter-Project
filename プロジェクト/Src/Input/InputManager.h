#pragma once

#include "../GameManager/ManagerBase.h"

enum InputKind
{
	INPUT_KIND_NONE = -1,

	I_KEY,
	I_MOUSE,
	I_PAD,

	INPUT_KIND_NUM,
};

class CInput;

//インプットマネージャークラス
class CInputManager : public CManagerBase
{

private:
	CInput* m_input[INPUT_KIND_NUM];	//インスタンス
	static CInputManager* m_inputManager;	//インプットマネージャー実態

	//コンストラクタ
	CInputManager() :
		m_input{ NULL }
	{}

public:

	//初期化
	void Init();
	//ステップ
	void Step();
	//後処理
	void Fin();

	//マネージャー生成、取得、削除
	static void CreateManager();
	static CInputManager* GetManager();
	void DeleteManager();

	static bool IsPush(int code, InputKind kind);
	static bool IsKeep(int code, InputKind kind);
	static bool IsRelease(int code, InputKind kind);
	static bool IsDown(int code, InputKind kind);
};