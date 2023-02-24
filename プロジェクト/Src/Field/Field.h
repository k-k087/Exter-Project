#pragma once
#include "DxLib.h"

#include"../Object/Object.h"

class CField : public CObject
{
private:
	//コンストラクタ
	CField()
	{
		m_kind = OBJECT_KIND_FIELD;
	}

public:
	void Init();
	void Load();
	void Step();
	void Update();
	void Draw();
	void Fin();

	//生成
	static CField* Create();
};
