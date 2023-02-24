#ifndef __COLLISION_MANAGER_H__
#define __COLLISION_MANAGER_H__

#include "../GameManager/ManagerBase.h"
#include "../../Scene/PlayScene/PlayScene.h"

//当たり判定マネージャー
class CCollisionManager : public CManagerBase
{
private:
	static const int CHECK_COLLISION_NUM = 4;

	static CCollisionManager* m_collisionManager;

	CCollisionManager()
	{
		m_kind = MG_KIND_COLLISION;
	}

	

public:
	void Init();			//初期化
	void Step() override;	//ステップ

	//マネージャー生成
	static CCollisionManager* CreateManager();
	//マネージャーゲット
	static CCollisionManager* GetManager();
	//マネージャー削除
	void DeleteManager();



private: 

	void CheckPlayerHitEnemy();
	void CheckPlayerHitEnemyHorizon();
	void CheckPlayerAttackHitEnemy();
	void CheckEnemyHitEnemy();
	void CheckEnemyAttackRangeHitPlayer();
	void CheckEnemyAttackHitPlayer();
	void CheckEnemyMagicHitPlayer();
	void CheckEnemyBeamHitPlayer();
	void CheckPlayerRisingHitEnemy();
	void CheckPlayerHitStage();


	void CheckPlayerAttackHitEnemyTutorial1(TutorialPhase tutorial_phase);
	void CheckPlayerAttackHitEnemyTutorial2(TutorialPhase tutorial_phase);


public:

	bool IsCheckPlayerAttackHitEnemy();
	void CheckEnemyAttackHitPlayerTutorial();

	void CheckTutorialCollision(TutorialPhase tutorial_phase);
};

#endif