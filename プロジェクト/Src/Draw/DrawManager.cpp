#include "DxLib.h"

#include <list>

#include "DrawManager.h"

#include "../Object/Object.h"
#include "../Common.h"


CDrawManager* CDrawManager::m_drawManager = NULL;
int CDrawManager::m_drawCount;

//コンストラクタ
CDrawManager::CDrawManager() :
	m_shadowHandle(-1)
{
	m_drawCount = 0;
}

//初期化
void CDrawManager::Init()
{
	m_drawCount = 0;
	m_shadowHandle = MakeShadowMap(4096, 4096);
	SetShadowMapLightDirection(m_shadowHandle, VGet(1.f, -1.f, 0.3f));

	//	影範囲をセット
	SetShadowMapDrawArea(m_shadowHandle, VGet(-1000.f, -1.f, -1000.f),
		VGet(1000.f, 200.f, 1000.f));
}

void CDrawManager::DrawAllObject()
{
	m_drawManager->Draw();
}
void CDrawManager::Draw()
{
	//まずUptate
	for (const auto& draw_vec : m_objectVec)
	{
		draw_vec->Update();
	}

	//影だけ先に描画
	if (m_shadowHandle != -1)
	{
		ShadowMap_DrawSetup(m_shadowHandle);

		//Drawを順番に回す
		for (const auto& draw_vec : m_objectVec)
		{
			draw_vec->Draw();
		}

		ShadowMap_DrawEnd();
	}

	//通常Drawを順番に回す
	SetUseShadowMap(0, m_shadowHandle);

	for (const auto& draw_vec : m_objectVec)
	{
		draw_vec->Draw();
	}

	SetUseShadowMap(0, -1);
}

CDrawManager* CDrawManager::CreateManager()
{
	//ないなら作る
	if (!m_drawManager)
	{
		m_drawManager = _NEW(CDrawManager);
	}

	return m_drawManager;
}
CDrawManager* CDrawManager::GetManager()
{
	if (!m_drawManager) { return nullptr; }

	//あるなら渡す
	return m_drawManager;
}
void CDrawManager::DeleteManager()
{
	if (!m_drawManager) { return; }
	
	//リストの中身クリア
	m_objectVec.clear();

	DeleteShadowMap(m_shadowHandle);

	//削除
	_DELETE(m_drawManager);

	m_drawManager = nullptr;
	
}

//データ追加
int CDrawManager::AddData(CObject* object)
{
	return m_drawManager->Add(object);
}
int CDrawManager::Add(CObject* object)
{
	//ID用変数を保持
	int draw_id = m_drawCount;
	
#if 0
	//種類取得
	CObject::ObjectKind kind = object->GetKind();


	//先頭ならそのまま追加
	if (m_drawCount == 0)
	{
		m_objectVec.push_back(object);
	}
	//先頭があるなら優先度順に追加
	else
	{
		//入れたい場所まで回る
		auto draw_vec = m_objectVec.begin();
		auto vec_end = m_objectVec.end();
		for (; draw_vec != vec_end && (*draw_vec)->GetKind() < kind; draw_vec++) {}

		//データの追加
		m_objectVec.insert(draw_vec, object);
	}
#endif

	//後ろにデータを入れる
	m_objectVec.push_back(object);

	//カウント加算
	++m_drawCount;

	return draw_id;
}

//データ削除
void CDrawManager::DeleteData(int draw_id)
{
	m_drawManager->Delete(draw_id);
}
void CDrawManager::Delete(int draw_id)
{
	////消したいデータまで回る
	//for (auto draw_vec = m_)
}