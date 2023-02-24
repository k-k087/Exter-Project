#include "DxLib.h"

#include "Field.h"

#include "../Draw/DrawManager.h"
#include "FieldManager.h"
#include "../Common.h"

static const char* FIELD_PATH = "Data/Model/Stage/STAGE1.x";

static const VECTOR FIELD_SCALE = { 1.7f, 1.2f, 1.7f };

static const float FIELD_SPEED = 10.f;

static const float MAX_FIELD_Z = 4500.f;

//生成
CField* CField::Create()
{
	CField* field = _NEW(CField);

	//DrawListに追加
	CDrawManager::AddData(field);

	return field;
}

//初期化
void CField::Init()
{
	m_radius = 800;
}

//ロード
void CField::Load()
{
	m_handle = MV1LoadModel(FIELD_PATH);

	MV1SetScale(m_handle, FIELD_SCALE);
}

//ステップ
void CField::Step()
{

}

//アップデート
void CField::Update()
{
	//座標セット
	MV1SetPosition(m_handle, m_pos);
}

//描画
void CField::Draw()
{

	//最後に描画
	MV1DrawModel(m_handle);
}

void CField::Fin()
{

}