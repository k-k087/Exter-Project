#pragma once

#include "../Common.h"

#include "../Character/Character.h"

//魔法の種類
enum MagicKind
{
	MAGIC_KIND_NONE = -1,

	MAGIC_KIND_GOLEM_MAGIC,

	MAGIC_KIND_NUM,
};


struct MagicDetailInfo
{
	MagicKind	m_magicKind;	//種類

	VECTOR		size;			//モデルサイズ
	VECTOR		scale;			//拡大率
	float		radius;			//モデル半径
	float		speed;			//スピード
	float		maxLange;		//最大距離
};

struct MagicRequestInfo
{
	VECTOR startPos;
	VECTOR rot;
	VECTOR move;

	int			paramI;
	float		paramF;
};

class CMagic
{
private:

	MagicDetailInfo		m_detailInfo;				//詳細情報（エフェクトによってここが変わる）

	VECTOR				m_pos;					//座標
	VECTOR				m_rot;					//回転値
	VECTOR				m_startPos;				//初期座標
	VECTOR				m_oldPos;				//1フレーム前の座標
	VECTOR				m_move;					//移動量

	int					m_effectHandleID;		//画像ハンドル
	bool				m_isUse;				//使用中フラグ

	CMagic() :
		m_detailInfo{},
		m_isUse(false),
		m_effectHandleID(-1)
	{}
	

public:

	~CMagic() {}

	void		Init();			
	void		Step();
	void		Update();
	void		Fin();

	static CMagic* Create()
	{
		CMagic* magic = _NEW(CMagic);	magic->Init();

		return magic;
	}

	void Request(MagicRequestInfo& request_info, MagicDetailInfo& detail_info, int handle_id);


	/*
		Getter&Setter
	*/

	MagicDetailInfo* GetDetailInfo() { return &m_detailInfo; }
	void SetDetailInfo(MagicDetailInfo& detail) { m_detailInfo = detail; }

	//使用フラグ
	bool GetIsUse()const { return m_isUse; }
	void SetIsUse(const bool is_use) { m_isUse = is_use; }

	//ハンドル
	int GetEffectHandleID()const { return m_effectHandleID; }
	void SetEffectHandleID(const int handle_id) { m_effectHandleID = handle_id; }

	// 座標
	VECTOR GetPos()const { return m_pos; }
	void SetPos(const VECTOR& pos) { m_pos = pos; }

	//1フレーム前の座標
	VECTOR GetOldPos()const { return m_oldPos; }
	void SetOldPos(const VECTOR& old_pos) { m_oldPos = old_pos; }

	//スタート座標
	VECTOR GetStartPos()const { return m_startPos; }
	void SetStartPos(const VECTOR& start_pos) { m_startPos = start_pos; }

	//移動量
	VECTOR GetMove()const { return m_move; }
	void SetMove(const VECTOR& move) { m_move = move; }

	//回転値
	VECTOR GetRot()const { return m_rot; }
	void SetRot(const VECTOR& rot) { m_rot = rot; }

	//半径
	float GetRadius()const { return m_detailInfo.radius; }
	void SetRadius(const float radius) { m_detailInfo.radius = radius; }

};



