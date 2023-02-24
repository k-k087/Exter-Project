#pragma once

#include "../Enemy.h"

enum BossKind
{
	BOSS_KIND_NONE = -1,

	BOSS_KIND_1,
	BOSS_KIND_2,

	BOSS_KIND_NUM,
};

class CBoss : public CEnemy
{
protected:
	BossKind m_bossKind;


	CBoss():
		m_bossKind(BOSS_KIND_NONE)
	{
	}


public:
	void Killing()override {};

	BossKind GetBossKind()const { return m_bossKind; }
	void SetBossKind(const BossKind kind) { m_bossKind = kind; }
};