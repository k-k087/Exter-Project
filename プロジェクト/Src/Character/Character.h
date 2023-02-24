#pragma once

#include "DxLib.h"

#include "../Object/Object.h"

#include "CharacterCommon.h"

enum AttackHitKind
{
	ATTACK_HIT_KIND_NONE = -1,

	ATTACK_HIT_KIND_HIT,
	ATTACK_HIT_KIND_KNOCK_BACK,

	ATTACK_HIT_KIND_NUM,
};

class CCharacter : public CObject
{
public:
	//キャラクター種類
	//（優先度順に並んでいる）
	enum CharacterKind
	{
		CHARA_KIND_NONE = -1,

		CHARA_KIND_PLAYER,		//プレイヤー

		CHARA_KIND_NUM,
	};

protected:
	int m_hp;
	int m_maxHP;
	int m_handle;			//モデルハンドル
	int m_characterID;
	bool m_isAlive;				//生きているか

	CCharacter() :
		m_handle(-1),
		m_maxHP(0),
		m_hp(0),
		m_isAlive(true)
	{}

public:

	void AddPos(VECTOR add)
	{
		m_pos.x += add.x; m_pos.y += add.y; m_pos.z += add.z;
	}


	/*
		Getter&Setter
	*/

	int GetHandle()const { return m_handle; }
	void SetHandle(int handle) { m_handle = handle; }

	int GetHP()const { return m_hp; }
	void SetHP(int hp) { m_hp = hp; }

	int GetMaxHP()const { return m_maxHP; }
	void SetMaxHP(int hp) { m_maxHP = hp; }

	bool GetIsAlive()const { return m_isAlive; }

	virtual AttackType GetAttackType() = 0;

#if 0
	CCharacter* GetNext()
	{
		return m_next;
	}
	void SetNext(CCharacter* next)
	{
		m_next = next;
	}

	CCharacter* GetPrev()
	{
		return m_prev;
	}
	void SetPrev(CCharacter* prev)
	{
		m_prev = prev;
	}
#endif

};

