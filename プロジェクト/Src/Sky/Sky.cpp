#include "DxLib.h"

#include "Sky.h"

#include "../Draw/DrawManager.h"
#include "../Character/Player/PlayerManager.h"
#include "../Character/Player/Player.h"
#include "../Common.h"


//モデルパス
static const char* SKY_PATH = "Data/Model/Sky/Sky2.x";

//生成
CSky* CSky::Create()
{
	CSky* sky = _NEW(CSky);

	//DrawListに追加
	CDrawManager::AddData(sky);

	return sky;
}

//初期化
void CSky::Init()
{
	m_pos = VGet(0.f, 10.f, 0.f);
	m_handle = -1;
	m_rot = VGet(0.f, 0.f, 0.f);
}

//ロード
void CSky::Load()
{
	if (m_handle == -1)
	{
		m_handle = MV1LoadModel(SKY_PATH);
	}

	//大きさセット
	MV1SetScale(m_handle, VGet(50.f, 50.f, 50.f));
	m_rot.x = -DX_PI_F / 180.f * 2.f;
}

//ステップ
void CSky::Step()
{
	m_rot.y += 0.0004f;
	m_rot.z += 0.0002f;
}

//アップデート
void CSky::Update()
{
	MV1SetPosition(m_handle, m_pos);
	MV1SetRotationXYZ(m_handle, m_rot);
}

//描画
void CSky::Draw()
{
	MV1DrawModel(m_handle);
}

void CSky::Fin()
{

}