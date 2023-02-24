#pragma once
#include "DxLib.h"

#include"../Object/Object.h"

class CField : public CObject
{
private:
	//�R���X�g���N�^
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

	//����
	static CField* Create();
};
