#pragma once
#include "DxLib.h"

//カメラクラス
class CDebugCamera
{
private:
	VECTOR m_pos;
	VECTOR m_rot;
	float m_near, m_far;

	//コンストラクタ
	CDebugCamera() :
		m_pos{ 0.f, 0.f, 0.f },
		m_rot{ 0.f, 0.f, 0.f }
	{}

public:

	//初期化
	void Init();
	//ロード
	void Load();
	//ステップ
	void Step();
	//座標セット
	void Update();

	//生成
	static CDebugCamera* Create();

	/*
		Getter&Setter
	*/

	//座標
	VECTOR GetPos()const { return m_pos; }
	void SetPos(VECTOR pos) { m_pos = pos; }

	//回転値
	VECTOR GetRot()const { return m_rot; }
	void SetRot(VECTOR rot) { m_rot = rot; }
};

////初期化
//void InitCameraInfo();
////生成
//void CreateCamera();
////後処理
//void FinCamera();
////箱情報取得
//CCamera* GetCameraInfo();
