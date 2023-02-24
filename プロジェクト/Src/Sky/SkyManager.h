#pragma once

#include "DxLib.h"

#include "../GameManager/ManagerBase.h"

class CSky;

class CSkyManager : public CManagerBase
{
private:
	CSky* m_sky;
	static CSkyManager* m_skyManager;

	CSkyManager()
	{
		m_sky = nullptr;
		m_kind = MG_KIND_SKY;
	}

public:
	void Init();			//初期化
	void Load();			//ロード
	void Step() override;	//ステップ
	void Draw();			//描画
	void Fin();				//終了処理

	//マネージャー生成
	static CSkyManager* CreateManager();
	//マネージャーゲット
	static CSkyManager* GetManager();
	//マネージャー削除
	void DeleteManager();
};
