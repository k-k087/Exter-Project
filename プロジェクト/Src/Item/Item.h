#pragma once

#include "../Common.h"
#include "../Object/Object.h"

#include "../Character/Character.h"
#include "../Draw/DrawManager.h"

//アイテムの種類
enum ItemKind
{
	ITEM_KIND_NONE = -1,

	ITEM_KIND_HP,
	ITEM_KIND_MP,

	ITEM_KIND_NUM,
};

struct ItemRequestData
{
	VECTOR	startPos;
	VECTOR	move;
	float	speed;
	int		useCount;

	ItemRequestData(VECTOR start_pos_, VECTOR move_, float speed_, int use_count_)
	{
		startPos = start_pos_;
		move = move_;
		speed = speed_;
		useCount = use_count_;
	}
};

class CItem : public CObject
{
private:
	ItemKind			m_itemKind;				//アイテム種類

	float				m_speed;				//スピード

	int					m_useCount;				//アイテムが存在するカウント（ミリ秒）

	bool				m_isUse;				//使用中フラグ

	CItem() :
		m_isUse(false),
		m_speed(),
		m_itemKind(ITEM_KIND_NONE)
	{}


public:

	~CItem() {}

	void		Init();
	void		Load();
	void		Load(int handle);
	void		Step();
	void		Update();
	void		Draw();
	void		Fin();

	static CItem* Create()
	{
		CItem* item = _NEW(CItem);	item->Init();

		//DrawListに追加
		item->m_drawID = CDrawManager::AddData(item);

		return item;
	}

	static CItem* Create(ItemKind kind)
	{
		CItem* item = _NEW(CItem);	item->Init();
		item->m_itemKind = kind;

		//DrawListに追加
		item->m_drawID = CDrawManager::AddData(item);

		return item;
	}

	void Request(ItemRequestData request_data);

	void HitFunc();


	/*
		Getter&Setter
	*/

	//使用フラグ
	bool GetIsUse()const { return m_isUse; }
	void SetIsUse(const bool is_use) { m_isUse = is_use; }

	ItemKind GetItemKind()const { return m_itemKind; }
	void SetItemKind(const ItemKind) { m_itemKind; }
};
