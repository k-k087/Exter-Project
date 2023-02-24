#pragma once

#include "DxLib.h"

#include "UI.h"

#include "../Common.h"
#include "../GameManager/ManagerBase.h"


class CUIManager
{
private:

	static CUIManager* m_UIManager;
	CUI* m_UI;

	CUIManager() :
		m_UI(nullptr)
	{}

public:

	~CUIManager()
	{}

	void Init();			//初期化
	void Load();			//ロード
	void Step();			//ステップ
	void Draw();			//描画
	void Fin();				//後処理

	//マネージャー生成
	static CUIManager* CreateManager();
	//マネージャーゲット
	static CUIManager* GetManager();
	//マネージャー削除
	void DeleteManager();


	//特定のUI生成
	void CreateUI(UIKind kind);
	CUI* GetUI();

};