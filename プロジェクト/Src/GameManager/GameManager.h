#pragma once

#include "DxLib.h"

#include "ManagerBase.h"

class CGameManager
{
private:
	//マネジャーノード
	//(マネージャーの種類決まってるから別にリスト管理しなくてもいいけど勉強のためにしました)
	struct ManagerNode
	{
		CManagerBase* m_manager;	//ポインタ
		ManagerNode* m_next;		//次のアドレス
		ManagerNode* m_prev;		//前のアドレス

		//コンストラクタ
		ManagerNode() :
			m_manager(nullptr),
			m_next(nullptr),
			m_prev(nullptr)
		{}
	};

	ManagerNode* m_leadManagerNode;	//先頭のマネージャー
	static CGameManager* m_gameManager;	//インスタンス

	CGameManager() :
		m_leadManagerNode(nullptr)
	{}

public:
	//初期化
	void Init();

	//すべてのマネージャーのステップ
	static void StepAllManager();
	static void StepAllManagerN(CManagerBase::ManagerKind not_step_mg);
	static void StepAllManagerN(CManagerBase::ManagerKind not_step_mg1, CManagerBase::ManagerKind not_step_mg2);
	void Step();

	//すべてのDeleteを回す関数


	//リスト追加
	static void AddMGList(CManagerBase* manager);
	void Add(CManagerBase* manager);

	//リスト削除系
	static void DeleteMGList(CManagerBase* manager);
	void Delete(CManagerBase* manager);
	void DeleteAllList();

	//生成、削除
	static void CreateManager();
	void DeleteManager();

	//Getter&Setter

	static CGameManager* GetGameManager()
	{
		if (!m_gameManager) { return nullptr; }
		return m_gameManager;
	}

	ManagerNode* GetLeadManagerNode()
	{
		if (m_leadManagerNode)
		{
			return m_leadManagerNode;
		}

		return nullptr;
	}
	void SetLeadManagerNode()
	{

	}


};