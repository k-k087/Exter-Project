#pragma once

#include <list>

#include "../GameManager/ManagerBase.h"

#include "../Object/Object.h"

using namespace std;

class CDrawManager
{
public:

private:
	int	m_shadowHandle;					//影ハンドル
	list<CObject*> m_objectVec;			//描画するもののリスト
	static CDrawManager* m_drawManager;	//インスタンス
	static int m_drawCount;				//描画数

	//コンストラクタ
	CDrawManager();

public:
	//初期化
	void Init();

	//すべてのDraw
	static void DrawAllObject();
	void Draw();

	//生成、取得、削除
	static CDrawManager* CreateManager();
	static CDrawManager* GetManager();
	void DeleteManager();

	//データ追加
	static int AddData(CObject* object);
	int Add(CObject* object);
	//データ削除
	static void DeleteData(int draw_id);
	void Delete(int draw_id);

	/*
		Getter&Setter
	*/

	list<CObject*>* GetObjectVec()
	{
		return &m_objectVec;
	}

};