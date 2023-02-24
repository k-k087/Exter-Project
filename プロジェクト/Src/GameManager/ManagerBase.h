#pragma once

#include "DxLib.h"

class CManagerBase
{
public:
	virtual void Step() = 0;

	//マネージャー種類
	//(優先度順に並んでいる)
	enum ManagerKind
	{
		MG_KIND_NONE = -1,

		MG_KIND_CHARACTER,	//キャラクター
		MG_KIND_MAGIC,		//魔法
		MG_KIND_ITEM,		//アイテム
		MG_KIND_BEAM,		//ビーム
		MG_KIND_FIELD,		//フィールド
		MG_KIND_SKY,		//天球
		MG_KIND_CAMERA,		//カメラ
		MG_KIND_UI,			//ユーザインタフェース
		MG_KIND_COLLISION,	//当たり判定チェック

		MG_KIND_NUM,
	};

protected:
	ManagerKind m_kind; //マネージャーの種類

public:
	//Getter&Setter

	ManagerKind GetKind()
	{
		return m_kind;
	}
};