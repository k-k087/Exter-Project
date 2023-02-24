#include "DxLib.h"

#include "GameManager.h"
#include "ManagerBase.h"
#include "../Common.h"

#include "../Effekseer/Effekseer.h"

CGameManager* CGameManager::m_gameManager = nullptr;	//インスタンス

void CGameManager::Init()
{
	
}

void CGameManager::StepAllManager()
{
	if (!m_gameManager->GetLeadManagerNode())return;

	ManagerNode* node = m_gameManager->GetLeadManagerNode();


	//順番にすべてのステップを回す
	for (; node; node = node->m_next)
	{
		node->m_manager->Step();
	}

}
void CGameManager::StepAllManagerN(CManagerBase::ManagerKind not_step_mg)
{
	if (!m_gameManager->GetLeadManagerNode())return;

	ManagerNode* node = m_gameManager->GetLeadManagerNode();


	//順番にすべてのステップを回す(引数のはまわさない)
	for (; node; node = node->m_next)
	{
		if (node->m_manager->GetKind() == not_step_mg)
			continue;

		node->m_manager->Step();
	}
}
void CGameManager::StepAllManagerN(CManagerBase::ManagerKind not_step_mg1, CManagerBase::ManagerKind not_step_mg2)
{
	if (!m_gameManager->GetLeadManagerNode())return;

	ManagerNode* node = m_gameManager->GetLeadManagerNode();


	//順番にすべてのステップを回す(引数のはまわさない)
	for (; node; node = node->m_next)
	{
		if (node->m_manager->GetKind() == not_step_mg1)
			continue;
		if (node->m_manager->GetKind() == not_step_mg2)
			continue;

		node->m_manager->Step();
	}
}
void CGameManager::Step()
{

}

void CGameManager::CreateManager()
{
	if (!m_gameManager)
	{
		m_gameManager = _NEW(CGameManager);
	}
}

void CGameManager::DeleteManager()
{
	if (!m_gameManager) { return; }
	
	_DELETE(m_gameManager);

	m_gameManager = nullptr;
	
}

//リストに追加する処理
void CGameManager::AddMGList(CManagerBase* manager)
{
	if (m_gameManager)
	{
		m_gameManager->Add(manager);
	}
}
void CGameManager::Add(CManagerBase* manager)
{
	CManagerBase::ManagerKind kind = manager->GetKind();

	//新しくノードを作る

	ManagerNode* new_node = _NEW(ManagerNode);
	new_node->m_manager = manager;

	//先頭がいなければ先頭にする
	if (m_leadManagerNode == nullptr)
	{
		m_leadManagerNode = new_node;
	}
	//先頭があるなら
	else
	{
		//先頭取得
		ManagerNode* node = m_leadManagerNode;

		//優先度順にリストに追加
		//入れる前のノードまで空ループ
		for (; node->m_next && node->m_next->m_manager->GetKind() <= kind; node = node->m_next){}

		//中間なら
		if (node->m_next)
		{
			//nextとprevをつなぎなおし、間に入れる
			ManagerNode* next_node = node->m_next;

			node->m_next = new_node;
			new_node->m_prev = node;
			new_node->m_next = next_node;
			next_node->m_prev = new_node;
		}
		//最後まで回ったなら
		else if (!node->m_next)
		{
			//最後尾に追加
			new_node->m_prev = node;
			node->m_next = new_node;
		}
	}

}

void CGameManager::DeleteMGList(CManagerBase* manager)
{
	if (m_gameManager)
	{
		m_gameManager->Delete(manager);
	}
}
void CGameManager::Delete(CManagerBase* manager)
{
	//先頭取得
	ManagerNode* node = m_leadManagerNode;

	//探す
	for (; node->m_manager->GetKind() != manager->GetKind(); node = node->m_next) {}

	//最後の一つか見る
	if (!node->m_prev && !node->m_next)
	{
		m_leadManagerNode = nullptr;
	}
	//先頭なら
	else if (!node->m_prev)
	{
		m_leadManagerNode = node->m_next;
		m_leadManagerNode->m_prev = nullptr;
	}
	//中間なら
	else if (node->m_next&&node->m_prev)
	{
		ManagerNode* prev = node->m_prev;
		ManagerNode* next = node->m_next;

		prev->m_next = next;
		next->m_prev = prev;
		node->m_next = node->m_prev = nullptr;
	}
	//最後なら
	else if (!node->m_next)
	{
		ManagerNode* prev = node->m_prev;
		prev->m_next = nullptr;
	}

	_DELETE(node);
	node = nullptr;
}
void CGameManager::DeleteAllList()
{

}