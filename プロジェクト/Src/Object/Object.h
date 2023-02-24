#pragma once

#include "DxLib.h"

//オブジェクトクラス (座標と描画があるもの)
class CObject
{
public:
	//オブジェクト種類
	enum ObjectKind
	{
		OBJECT_KIND_NONE = -1,

		OBJECT_KIND_SKY,		//空
		OBJECT_KIND_FIELD,		//フィールド
		OBJECT_KIND_PLAYER,		//プレイヤ-

		OBJECT_KIND_NUM,
	};

public:
	virtual void Init() = 0;	//初期化
	virtual void Load() = 0;	//ロード
	virtual void Step() = 0;	//ステップ関数
	virtual void Update() = 0;	//アップデート
	virtual void Draw() = 0;	//描画関数
	virtual void Fin() = 0;		//後処理

protected:
	VECTOR		m_pos;			//座標
	VECTOR		m_rot;			//回転値
	VECTOR		m_startPos;		//初期座標
	VECTOR		m_oldPos;		//1フレーム前の座標
	VECTOR		m_move;			//移動量
	VECTOR		m_size;			//モデルサイズ
	ObjectKind	m_kind;			//種類
	float		m_radius;		//モデル半径
	int			m_handle;		//ハンドル
	int			m_drawID;		//描画用ID

	//コンストラクタ
	CObject() :
		m_handle(-1),
		m_pos{ 0.f, 0.f, 0.f },
		m_rot{ 0.f, 0.f, 0.f },
		m_oldPos{ 0.f, 0.f, 0.f },
		m_startPos{ 0.f, 0.f, 0.f },
		m_move{ 0.f, 0.f, 0.f },
		m_size{ 0.f, 0.f, 0.f },
		m_radius(0.f),
		m_drawID(-1)
	{}

public:
	
	/*
		Getter&Setter
	*/

	//座標
	VECTOR GetPos()const
	{
		return m_pos;
	}
	void SetPos(const VECTOR& pos)
	{
		m_pos = pos;
	}

	//1フレーム前の座標
	VECTOR GetOldPos()const 
	{
		return m_oldPos; 
	}
	void SetOldPos(const VECTOR& old_pos)
	{ 
		m_oldPos = old_pos; 
	}

	//スタート座標
	VECTOR GetStartPos()const
	{
		return m_startPos;
	}
	void SetStartPos(const VECTOR& start_pos)
	{
		m_startPos = start_pos;
	}

	//移動量
	VECTOR GetMove()const 
	{ 
		return m_move;
	}
	void SetMove(const VECTOR& move)
	{ 
		m_move = move; 
	}

	//回転値
	VECTOR GetRot()const
	{
		return m_rot;
	}
	void SetRot(const VECTOR& rot)
	{
		m_rot = rot;
	}

	//サイズ
	VECTOR GetSize()const
	{
		return m_size;
	}
	void SetSize(const VECTOR& size)
	{
		m_size = size;
	}

	//オブジェクトの種類
	ObjectKind GetKind()const
	{
		return m_kind;
	}
	void SetKind(const ObjectKind kind)
	{
		m_kind = kind;
	}

	//半径
	float GetRadius()const
	{
		return m_radius;
	}
	void SetRadius(const float radius)
	{
		m_radius = radius;
	}

	//描画ID
	int GetDrawID()const
	{
		return m_drawID;
	}
	void SetDrawID(const int id)
	{
		m_drawID = id;
	}

	//ハンドル
	int GetHandle()const
	{
		return m_handle;
	}
	void SetHandle(const int id)
	{
		m_handle = id;
	}
};
