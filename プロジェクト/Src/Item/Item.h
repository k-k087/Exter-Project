#pragma once

#include "../Common.h"
#include "../Object/Object.h"

#include "../Character/Character.h"
#include "../Draw/DrawManager.h"

//�A�C�e���̎��
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
	ItemKind			m_itemKind;				//�A�C�e�����

	float				m_speed;				//�X�s�[�h

	int					m_useCount;				//�A�C�e�������݂���J�E���g�i�~���b�j

	bool				m_isUse;				//�g�p���t���O

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

		//DrawList�ɒǉ�
		item->m_drawID = CDrawManager::AddData(item);

		return item;
	}

	static CItem* Create(ItemKind kind)
	{
		CItem* item = _NEW(CItem);	item->Init();
		item->m_itemKind = kind;

		//DrawList�ɒǉ�
		item->m_drawID = CDrawManager::AddData(item);

		return item;
	}

	void Request(ItemRequestData request_data);

	void HitFunc();


	/*
		Getter&Setter
	*/

	//�g�p�t���O
	bool GetIsUse()const { return m_isUse; }
	void SetIsUse(const bool is_use) { m_isUse = is_use; }

	ItemKind GetItemKind()const { return m_itemKind; }
	void SetItemKind(const ItemKind) { m_itemKind; }
};
