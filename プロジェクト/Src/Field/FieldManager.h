#pragma once

#include "DxLib.h"

#include "../GameManager/ManagerBase.h"
#include "Field.h"

class CField;

class CFieldManager : public CManagerBase
{
public:
private:
	CField* m_field;
	static CFieldManager* m_fieldManager;

	CFieldManager() :
		m_field{nullptr}
	{
		m_kind = MG_KIND_FIELD;
	}

public:
	void Init();			//初期化
	void Load();			//ロード
	void Step();		//ステップ
	void Draw();			//描画
	void Fin();				//終了処理

	//マネージャー生成
	static CFieldManager* CreateManager();
	//マネージャーゲット
	static CFieldManager* GetManager();
	//マネージャー削除
	void DeleteManager();

	CField* GetField()
	{
		return m_field;
	}
};