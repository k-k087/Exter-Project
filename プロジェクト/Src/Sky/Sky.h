#pragma once

#include "DxLib.h"

#include "../Object/Object.h"

class CSky : public CObject
{
private:
	//�R���X�g���N�^
	CSky()
	{
		m_rot = VGet(0.f, 0.f, 0.f);
		m_kind = OBJECT_KIND_SKY;
	}

public:

	void Init();
	void Load();
	void Step();
	void Update();
	void Draw();
	void Fin();

	//����
	static CSky* Create();
};
