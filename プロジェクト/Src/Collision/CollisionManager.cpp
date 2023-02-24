#include "DxLib.h"
#include "Collision.h"
#include "CollisionManager.h"

#include "../Common.h"

#include <list>
#include <vector>

#include "../Character/CharacterManager.h"
#include "../Character/Player/PlayerManager.h"
#include "../Character/Enemy/EnemyManager.h"
#include "../Character/Enemy/NormalEnemy/NormalEnemy.h"
#include "../Character/Player/Player.h"
#include "../GameManager/GameManager.h"
#include "../Magic/MagicManager.h"
#include "../MyMath/MyMath.h"
#include "../Effekseer.h"
#include "../Beam/BeamManager.h"
#include "../Character/Enemy/Boss/Boss1/Boss1.h"
#include "../Field/FieldManager.h"

using namespace std;

CCollisionManager* CCollisionManager::m_collisionManager = NULL;

//初期化
void CCollisionManager::Init()
{
	//マネージャーリストに追加


	CGameManager::AddMGList(this);
}

//ステップ
//すべての当たり判定をする
void CCollisionManager::Step()
{
	

	CheckPlayerHitEnemy();

	CheckPlayerHitEnemyHorizon();

	
	CheckPlayerAttackHitEnemy();


	CheckPlayerRisingHitEnemy();


	CheckEnemyHitEnemy();

	
	CheckEnemyAttackRangeHitPlayer();
	
	CheckEnemyAttackHitPlayer();
	
	CheckEnemyMagicHitPlayer();
	
	CheckEnemyBeamHitPlayer();


	CheckPlayerHitStage();

}

void CCollisionManager::CheckPlayerHitEnemy()
{
	//プレイヤーと敵の当たり判定
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	if (player->GetAttackKind() == PLAYER_ATTACK_RISING) { return; }

	//必要情報ゲット
	VECTOR player_center_pos = player->GetCenterPos();
	float player_radius = player->GetRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//当たり判定をとるか
		if (!enemy_list->JudgeCollission(ENEMY_HIT_PLAYER))continue;

		//必要情報ゲット
		VECTOR enemy_center_pos = enemy_list->GetCenterPos();
		float enemy_radius = enemy_list->GetRadius();

		//当たっているかチェック
		if (CCollision::IsHitBall(player_center_pos, player_radius, enemy_center_pos, enemy_radius))
		{
			VECTOR enemy_move = enemy_list->GetMove();
			VECTOR player_move = player->GetMove();

			//埋まっている長さを出す
			float radius_sum = player_radius + enemy_radius;

			//プレイヤーから敵へのベクトル
			VECTOR distance_vec = CMyMath::VecCreate(player_center_pos, enemy_center_pos);

			float distance = CMyMath::VecLong(distance_vec);
			float scale = radius_sum - distance;

			//Y成分は０にする
			distance_vec.y = 0.f;

			//埋まっている分その向きに押し出す
			distance_vec = CMyMath::VecNormalize(distance_vec);
			distance_vec = CMyMath::VecScale(distance_vec, scale);

			//まず、特定の時の処理
			if (enemy_list->GetEnemyKind() == ENEMY_KIND_BOSS1 &&
				enemy_list->GetAttackKind() == BOSS1_ATTACK_JUMP)
			{
				//プレイヤーだけ押し出す
				distance_vec = CMyMath::VecScale(distance_vec, -1.f);
				player->AddPos(distance_vec);
			}
			//プレイヤーだけ動いている場合
			else if (!CCommon::CheckZeroVector(player_move) && CCommon::CheckZeroVector(enemy_move))
			{
				//プレイヤーだけ押し出す
				distance_vec = CMyMath::VecScale(distance_vec, -1.f);
				player->AddPos(distance_vec);

			}
			//敵だけ動いている場合
			else if (CCommon::CheckZeroVector(player_move) && !CCommon::CheckZeroVector(enemy_move))
			{
				//敵だけ押し出す
				enemy_list->AddPos(distance_vec);
			}
			//両方動いている場合
			else if (!CCommon::CheckZeroVector(player_move) && !CCommon::CheckZeroVector(enemy_move))
			{
				//両方半分ずつ押し出す
				distance_vec = CMyMath::VecScale(distance_vec, 0.5f);
				VECTOR distance_vec_ = CMyMath::VecScale(distance_vec, -1.f);
				enemy_list->AddPos(distance_vec);
				player->AddPos(distance_vec_);
			}
		}
	}
}
void CCollisionManager::CheckPlayerHitEnemyHorizon()
{
	//プレイヤーと敵視界の当たり判定
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	//必要情報ゲット
	VECTOR player_center_pos = player->GetCenterPos();
	float player_radius = player->GetRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//当たり判定をとるか
		if (!enemy_list->JudgeCollission(ENEMY_HORIZON_HIT_PLAYER))continue;

		//必要情報ゲット
		VECTOR enemy_center_pos = enemy_list->GetCenterPos();
		float enemy_horizon_radius = enemy_list->GetHorizonRadius();

		//当たっているかチェック
		if (CCollision::IsHitBall(player_center_pos, player_radius, enemy_center_pos, enemy_horizon_radius))
		{
			//追跡状態に
			enemy_list->SetState(ENEMY_STATE_TRACK);
			enemy_list->ChangeAnimeToState();
		}
		else
		{
			//待機状態に
			enemy_list->SetState(ENEMY_STATE_IDLE);
			enemy_list->ChangeAnimeToState();
		}
	}
}

//	プレイヤー攻撃
void CCollisionManager::CheckPlayerAttackHitEnemy()
{

	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	//攻撃判定がないならreturn
	if (!player->GetIsAttack()) { return; }
	
	//必要情報ゲット
	VECTOR attack_center_pos = player->GetAttackCenterPos();
	float player_attack_radius = player->GetAttackRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//当たり判定をとるか
		if (!enemy_list->JudgeCollission(ENEMY_HIT_PLAYER_ATTACK))continue;

		//必要情報ゲット
		VECTOR enemy_center_pos = enemy_list->GetCenterPos();
		float enemy_radius = enemy_list->GetRadius();

		//当たっているかチェック
		if (CCollision::IsHitBall(attack_center_pos, player_attack_radius, enemy_center_pos, enemy_radius))
		{

			enemy_list->HitFunc(ENEMY_HIT_PLAYER_ATTACK);

			CAnimeInfo::AllStopAnime(140);

			//int handle_id = CEffekseerCtrl::Request(player->GetEffectID(PLAYER_EFFECT_KIND_HIT2), attack_center_pos, false);
			//CEffekseerCtrl::SetScale(handle_id, VGet(1.5f, 1.5f, 1.5f));

			PlayerAttackKind kind = player->GetAttackKind();
			float heal = 0;
			switch (kind)
			{
			case PLAYER_ATTACK_NORMAL1:
			case PLAYER_ATTACK_NORMAL2:
			case PLAYER_ATTACK_NORMAL3:
				heal = 4.0f; break;
			case PLAYER_ATTACK_NORMAL5:
			case PLAYER_ATTACK_NORMAL6:
				heal = 8.5f; break;
			case PLAYER_ATTACK_NORMAL7:
				heal = 9.5f; break;
			}
			player->HealingMP(heal);
			player->ChargeBoost(4.5f);
		}
	}

	//攻撃判定フラグをfalseに
	player->SetIsAttack(false);

#ifdef DEBUG_MODE

	DrawSphere3D(attack_center_pos, player_attack_radius, 32, GetColor(255, 50, 50), GetColor(0, 0, 0), false);

#endif
}
bool CCollisionManager::IsCheckPlayerAttackHitEnemy()
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	//攻撃判定がないならreturn
	if (!player->GetIsAttack()) { return false; }

	//必要情報ゲット
	VECTOR attack_center_pos = player->GetAttackCenterPos();
	float player_attack_radius = player->GetAttackRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//当たり判定をとるか
		if (!enemy_list->JudgeCollission(ENEMY_HIT_PLAYER_ATTACK))continue;

		//必要情報ゲット
		VECTOR enemy_center_pos = enemy_list->GetCenterPos();
		float enemy_radius = enemy_list->GetRadius();

		//当たっているかチェック
		if (CCollision::IsHitBall(attack_center_pos, player_attack_radius, enemy_center_pos, enemy_radius))
		{
			return true;
		}
	}

	return false;
}
void CCollisionManager::CheckEnemyAttackHitPlayerTutorial()
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	if (!player->JudgeCollission(PLAYER_HIT_NORMAL_ENEMY_ATTACK)) { return; }

	//必要情報ゲット
	VECTOR player_center_pos = player->GetCenterPos();
	float player_radius = player->GetRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//当たり判定をとるか
		if (!enemy_list->JudgeCollission(ENEMY_ATTACK_HIT_PLAYER))continue;

		//必要情報ゲット
		VECTOR enemy_attack_pos = enemy_list->GetAttackCenterPos();
		float enemy_attack_radius = enemy_list->GetAttackRadius();

		//当たっているかチェック
		if (CCollision::IsHitBall(player_center_pos, player_radius, enemy_attack_pos, enemy_attack_radius))
		{
			player->HitFunc(PLAYER_HIT_NORMAL_ENEMY_ATTACK);

			//敵の方を向く
			VECTOR enemy_pos = enemy_list->GetPos();
			VECTOR vec = CMyMath::VecCreate(player_center_pos, enemy_pos);
			VECTOR rot = { 0.f, CMyMath::VecDir(vec.x, vec.z) + DX_PI_F, 0.f };

			player->SetRot(rot);
		}


#ifdef DEBUG_MODE

		DrawSphere3D(enemy_attack_pos, enemy_attack_radius, 32, GetColor(255, 50, 50), GetColor(0, 0, 0), false);

#endif

		////攻撃判定フラグをfalseに
		//enemy_list->SetIsAttack(false);
	}
}

void CCollisionManager::CheckPlayerRisingHitEnemy()
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	//ライジング攻撃判定がないならreturn
	if (!player->GetIsRisingAttack()) { return; }

	//必要情報ゲット
	float player_attack_radius = player->GetAttackRadius();

	VECTOR start_pos = player->GetRisingStartPos();
	VECTOR end_pos = player->GetRisingPos();
	VECTOR rising_move = CMyMath::VecScale(CMyMath::VecNormalize(player->GetRisingMove()),
		player_attack_radius);


	//保存用
	vector<CEnemy*> enemy_p; enemy_p.clear();
	bool is_stop = false;

	

	while (1)
	{
		//どんどん進める
		start_pos = CMyMath::VecAdd(start_pos, rising_move);

		for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
		{
			//当たり判定をとるか
			if (!enemy_list->JudgeCollission(ENEMY_HIT_PLAYER_ATTACK))continue;

			//すでに当たり判定をとったやつとはとらない
			bool is_check = true;
			for (auto enemy_p_vec : enemy_p)
				if (enemy_p_vec == enemy_list) { is_check = false; break; }
			if (!is_check) { continue; }
			

			//必要情報ゲット
			VECTOR enemy_center_pos = enemy_list->GetCenterPos();
			float enemy_radius = enemy_list->GetRadius();

			//当たっているかチェック
			if (CCollision::IsHitBall(start_pos, player_attack_radius, enemy_center_pos, enemy_radius))
			{
				enemy_p.push_back(enemy_list);

				enemy_list->HitFunc(ENEMY_HIT_PLAYER_ATTACK);

				is_stop = true;

				player->ChargeBoost(4.5f);
			}
		}

#ifdef DEBUG_MODE

		DrawSphere3D(start_pos, player_attack_radius, 32, GetColor(255, 50, 50), GetColor(0, 0, 0), false);

#endif

		//終点と当たったら無限ループ終了
		if (CCollision::IsHitBall(start_pos, player_attack_radius, end_pos, 1.f))
			break;
	}


	if (is_stop) { /*CAnimeInfo::AllStopAnime(150);*/ }
	enemy_p.clear();

	//攻撃判定フラグをfalseに
	player->SetIsRisingAttack(false);
}

/*
	敵同士の当たり判定
*/
void CCollisionManager::CheckEnemyHitEnemy()
{
	for (const auto& enemy_list1 : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//当たり判定をとるか
		if (!enemy_list1->JudgeCollission(ENEMY_HIT_ENEMY))continue;

		//必要情報ゲット
		VECTOR enemy1_center_pos = enemy_list1->GetCenterPos();
		float enemy1_radius = enemy_list1->GetEnemyHitRadius();

		for (const auto& enemy_list2 : *CEnemyManager::GetManager()->GetEnemyList())
		{
			//同じ敵とは当たり判定はとれない
			if (enemy_list1 == enemy_list2)continue;
			//当たり判定をとるか
			if (!enemy_list2->JudgeCollission(ENEMY_HIT_ENEMY))continue;

			//必要情報ゲット
			VECTOR enemy2_center_pos = enemy_list2->GetCenterPos();
			float enemy2_radius = enemy_list2->GetEnemyHitRadius();

			//当たっているかチェック
			if (CCollision::IsHitBall(enemy1_center_pos, enemy1_radius, enemy2_center_pos, enemy2_radius))
			{
				VECTOR enemy1_move = enemy_list1->GetMove();
				VECTOR enemy2_move = enemy_list2->GetMove();

				//埋まっている長さを出す
				float radius_sum = enemy1_radius + enemy2_radius;

				//敵1から敵２へのベクトル
				VECTOR distance_vec = CMyMath::VecCreate(enemy1_center_pos, enemy2_center_pos);

				float distance = CMyMath::VecLong(distance_vec);
				float scale = radius_sum - distance;

				//Y成分は０にする
				distance_vec.y = 0.f;

				//埋まっている分その向きに押し出す
				distance_vec = CMyMath::VecNormalize(distance_vec);
				distance_vec = CMyMath::VecScale(distance_vec, scale);

				//敵１だけ動いている場合
				if (!CCommon::CheckZeroVector(enemy1_move) && CCommon::CheckZeroVector(enemy2_move))
				{
					//敵１だけ押し出す
					distance_vec = CMyMath::VecScale(distance_vec, -1.f);
					enemy_list1->AddPos(distance_vec);

				}
				//敵２だけ動いている場合
				else if (CCommon::CheckZeroVector(enemy1_move) && !CCommon::CheckZeroVector(enemy2_move))
				{
					//敵２だけ押し出す
					enemy_list2->AddPos(distance_vec);
				}
				//両方動いている場合
				else if (!CCommon::CheckZeroVector(enemy1_move) && !CCommon::CheckZeroVector(enemy2_move))
				{
					//両方半分ずつ押し出す
					distance_vec = CMyMath::VecScale(distance_vec, 0.5f);
					VECTOR distance_vec_ = CMyMath::VecScale(distance_vec, -1.f);
					enemy_list2->AddPos(distance_vec);
					enemy_list1->AddPos(distance_vec_);
				}
			}
		}
	}
}


//	敵攻撃範囲とプレイヤーの当たり判定
void CCollisionManager::CheckEnemyAttackRangeHitPlayer()
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	//プレイヤーの状態がダウンならreturn
	if (player->GetState() == PLAYER_STATE_KNOCK_BACK) { return; }

	//必要情報ゲット
	VECTOR player_center_pos = player->GetCenterPos();
	float player_radius = player->GetRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//当たり判定をとるか
		if (!enemy_list->JudgeCollission(ENEMY_RANGE_HIT_PLAYER))continue;

		//必要情報ゲット
		VECTOR enemy_center_pos = enemy_list->GetCenterPos();
		float enemy_range = enemy_list->GetAttackRange();

		//当たっているかチェック
		if (CCollision::IsHitBall(player_center_pos, player_radius, enemy_center_pos, enemy_range))
		{
			//攻撃準備状態に
			enemy_list->SetState(ENEMY_STATE_ATTACK_READY);
			enemy_list->ChangeAnimeToState();
		}
		else
		{
			//追跡状態に
			enemy_list->SetState(ENEMY_STATE_TRACK);
			enemy_list->ChangeAnimeToState();
		}
	}
}

//	敵攻撃とプレイヤーの当たり判定
void CCollisionManager::CheckEnemyAttackHitPlayer()
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	if (!player->JudgeCollission(PLAYER_HIT_NORMAL_ENEMY_ATTACK)) { return; }

	//必要情報ゲット
	VECTOR player_center_pos = player->GetCenterPos();
	float player_radius = player->GetRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//当たり判定をとるか
		if (!enemy_list->JudgeCollission(ENEMY_ATTACK_HIT_PLAYER))continue;

		//必要情報ゲット
		VECTOR enemy_attack_pos = enemy_list->GetAttackCenterPos();
		float enemy_attack_radius = enemy_list->GetAttackRadius();

		//当たっているかチェック
		if (CCollision::IsHitBall(player_center_pos, player_radius, enemy_attack_pos, enemy_attack_radius))
		{
			player->HitFunc(PLAYER_HIT_NORMAL_ENEMY_ATTACK);

			//敵の方を向く
			VECTOR enemy_pos = enemy_list->GetPos();
			VECTOR vec = CMyMath::VecCreate(player_center_pos, enemy_pos);
			VECTOR rot = { 0.f, CMyMath::VecDir(vec.x, vec.z) + DX_PI_F, 0.f };

			player->SetRot(rot);
		}


#ifdef DEBUG_MODE

		DrawSphere3D(enemy_attack_pos, enemy_attack_radius, 32, GetColor(255, 50, 50), GetColor(0, 0, 0), false);

#endif

		////攻撃判定フラグをfalseに
		//enemy_list->SetIsAttack(false);
	}
}


//敵魔法とプレイヤーの当たり判定
void CCollisionManager::CheckEnemyMagicHitPlayer()
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	if (!player->JudgeCollission(PLAYER_HIT_NORMAL_ENEMY_ATTACK)) { return; }

	//必要情報ゲット
	VECTOR player_center_pos = player->GetCenterPos();
	float player_radius = player->GetRadius();

	for (const auto& enemy_magic : *CMagicManager::GetManager()->GetEnemyMagic())
	{
		if (!enemy_magic->GetIsUse()) { return; }

		//必要情報ゲット
		VECTOR magic_pos = enemy_magic->GetPos();
		float magic_radius = enemy_magic->GetRadius();

		if (CCollision::IsHitBall(player_center_pos, player_radius, magic_pos, magic_radius))
		{
			player->HitFunc(PLAYER_HIT_GOLEM_MAGIC);

			//魔法の中心座標の方を向く
			VECTOR vec = CMyMath::VecCreate(player_center_pos, magic_pos);
			VECTOR rot = { 0.f, CMyMath::VecDir(vec.x, vec.z) + DX_PI_F, 0.f };

			player->SetRot(rot);
		}
	}
}

//敵ビームとプレイヤー当たり判定
void CCollisionManager::CheckEnemyBeamHitPlayer()
{
	
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	if (!player->JudgeCollission(PLAYER_HIT_NORMAL_ENEMY_ATTACK)) { return; }

	//必要情報ゲット
	VECTOR player_center_pos = player->GetCenterPos();
	float player_radius = player->GetRadius();

	for (const auto& enemy_beam : *CBeamManager::GetManager()->GetEnemyBeam())
	{
		if (!enemy_beam->GetIsUse()) { return; }

		//必要情報ゲット
		VECTOR beam_coll_pos = enemy_beam->GetPos();
		float beam_coll_radius = enemy_beam->GetRadius();

		if (CCollision::IsHitBall(player_center_pos, player_radius, beam_coll_pos, beam_coll_radius))
		{
			player->HitFunc(PLAYER_HIT_GOLEM_MAGIC);

			//魔法の中心座標の方を向く
			//VECTOR vec = CMyMath::VecCreate(player_center_pos, beam_coll_pos);
			VECTOR vec2 = enemy_beam->GetMove();

			if (enemy_beam->GetBeamKind() == BEAM_KIND_BOSS1_THUNDER)
				vec2 = VGet(CCommon::GetPMRandF(1.f), 0.f, CCommon::GetPMRandF(1.f));

			VECTOR rot = { 0.f, CMyMath::VecDir(-vec2.x, -vec2.z) + DX_PI_F, 0.f };

			player->SetRot(rot);

			break;
		}
	}

	
}

//	プレイヤーとステージの当たり判定
void CCollisionManager::CheckPlayerHitStage()
{
	//プレイヤーと敵の当たり判定
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	//必要情報ゲット
	VECTOR player_center_pos = player->GetCenterPos();
	player_center_pos.y = 0.f;
	float player_radius = player->GetRadius();

	CField* field = CFieldManager::GetManager()->GetField();

	VECTOR field_pos = field->GetPos();
	field_pos.y = 0.f;
	float field_radius = field->GetRadius();

	//	あったっていなければ戻す
	if (!CCollision::IsHitBall(player_center_pos, player_radius, field_pos, field_radius))
	{
		//埋まっている長さを出す
		float radius_sum = player_radius + field_radius;

		//ベクトル
		VECTOR distance_vec = CMyMath::VecCreate(player_center_pos, field_pos);
		float len = CMyMath::VecLong(distance_vec);
		
		float sub = len - radius_sum;

		//	ベクトルを正規化し、拡大してタス
		distance_vec = CMyMath::VecScale(CMyMath::VecNormalize(distance_vec), sub);

		player->AddPos(distance_vec);
	}

}

//マネージャー生成
CCollisionManager* CCollisionManager::CreateManager()
{
	//ないなら作る
	if (!m_collisionManager)
	{
		m_collisionManager = _NEW(CCollisionManager);
	}

	return m_collisionManager;
}
//マネージャーゲット
CCollisionManager* CCollisionManager::GetManager()
{
	if (!m_collisionManager) { return nullptr; }
	return m_collisionManager;
}
//マネージャー削除
void CCollisionManager::DeleteManager()
{
	//あるなら消す
	if (m_collisionManager)
	{
		CGameManager::DeleteMGList(m_collisionManager);
		_DELETE(m_collisionManager);

		m_collisionManager = nullptr;
	}
}

void CCollisionManager::CheckTutorialCollision(TutorialPhase tutorial_phase)
{
	CheckPlayerHitEnemy();

	CheckPlayerHitEnemyHorizon();

	CheckPlayerAttackHitEnemyTutorial1(tutorial_phase);
	CheckPlayerAttackHitEnemyTutorial2(tutorial_phase);

	CheckEnemyHitEnemy();

	CheckEnemyAttackRangeHitPlayer();

	CheckEnemyAttackHitPlayer();

	CheckEnemyMagicHitPlayer();

	CheckEnemyBeamHitPlayer();

	CheckPlayerHitStage();
}

void CCollisionManager::CheckPlayerAttackHitEnemyTutorial1(TutorialPhase tutorial_phase)
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	//攻撃判定がないならreturn
	if (!player->GetIsAttack()) { return; }

	//必要情報ゲット
	VECTOR attack_center_pos = player->GetAttackCenterPos();
	float player_attack_radius = player->GetAttackRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//当たり判定をとるか
		if (!enemy_list->JudgeCollission(ENEMY_HIT_PLAYER_ATTACK))continue;

		//必要情報ゲット
		VECTOR enemy_center_pos = enemy_list->GetCenterPos();
		float enemy_radius = enemy_list->GetRadius();

		//当たっているかチェック
		if (CCollision::IsHitBall(attack_center_pos, player_attack_radius, enemy_center_pos, enemy_radius))
		{
			
			//	プレイヤーの攻撃力をもってくる
			PlayerAttackInfo attack_info = player->GetPlayerAttackInfo();
			//	敵のHPを持ってくる
			int enemy_hp = enemy_list->GetHP();
			//	攻撃力がHPを上回っていたら＋5する
			if (attack_info.damage >= enemy_hp)
			{
			int re_hp = attack_info.damage + 5;
				enemy_list->SetHP(re_hp);
			}
			

			enemy_list->HitFunc(ENEMY_HIT_PLAYER_ATTACK);

			CAnimeInfo::AllStopAnime(140);

			//int handle_id = CEffekseerCtrl::Request(player->GetEffectID(PLAYER_EFFECT_KIND_HIT2), attack_center_pos, false);
			//CEffekseerCtrl::SetScale(handle_id, VGet(1.5f, 1.5f, 1.5f));

			PlayerAttackKind kind = player->GetAttackKind();
			float heal = 0;
			switch (kind)
			{
			case PLAYER_ATTACK_NORMAL1:
			case PLAYER_ATTACK_NORMAL2:
			case PLAYER_ATTACK_NORMAL3:
				heal = 2.f;	break;
			case PLAYER_ATTACK_NORMAL5:
			case PLAYER_ATTACK_NORMAL6:
				heal = 7.f; break;
			case PLAYER_ATTACK_NORMAL7:
				heal = 8.f; break;
			}
			player->HealingMP(heal);
		}
	}


	//攻撃判定フラグをfalseに
	player->SetIsAttack(false);
}
void CCollisionManager::CheckPlayerAttackHitEnemyTutorial2(TutorialPhase tutorial_phase)
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();


	//ライジング攻撃判定がないならreturn
	if (!player->GetIsRisingAttack()) { return; }

	float player_attack_radius = player->GetAttackRadius();

	VECTOR start_pos = player->GetRisingStartPos();
	VECTOR end_pos = player->GetRisingPos();
	VECTOR rising_move = CMyMath::VecScale(CMyMath::VecNormalize(player->GetRisingMove()),
		player_attack_radius);


	//保存用
	vector<CEnemy*> enemy_p; enemy_p.clear();
	bool is_stop = false;


	while (1)
	{
		//どんどん進める
		start_pos = CMyMath::VecAdd(start_pos, rising_move);

		for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
		{
			//当たり判定をとるか
			if (!enemy_list->JudgeCollission(ENEMY_HIT_PLAYER_ATTACK))continue;

			//すでに当たり判定をとったやつとはとらない
			bool is_check = true;
			for (auto enemy_p_vec : enemy_p)
				if (enemy_p_vec == enemy_list) { is_check = false; break; }
			if (!is_check) { continue; }


			//必要情報ゲット
			VECTOR enemy_center_pos = enemy_list->GetCenterPos();
			float enemy_radius = enemy_list->GetRadius();

			//当たっているかチェック
			if (CCollision::IsHitBall(start_pos, player_attack_radius, enemy_center_pos, enemy_radius))
			{
				
				//	プレイヤーの攻撃力をもってくる
				PlayerAttackInfo attack_info = player->GetPlayerAttackInfo();
				//	敵のHPを持ってくる
				int enemy_hp = enemy_list->GetHP();

				//	攻撃力がHPを上回っていたら＋5する
				if (attack_info.damage >= enemy_hp)
				{
					int re_hp = attack_info.damage + 5;
					enemy_list->SetHP(re_hp);
				}

				enemy_p.push_back(enemy_list);

				enemy_list->HitFunc(ENEMY_HIT_PLAYER_ATTACK);

				is_stop = true;
			}
		}

		//終点と当たったら無限ループ終了
		if (CCollision::IsHitBall(start_pos, player_attack_radius, end_pos, 1.f))
			break;
	}


	if (is_stop) { /*CAnimeInfo::AllStopAnime(150);*/ }
	enemy_p.clear();

	//攻撃判定フラグをfalseに
	player->SetIsRisingAttack(false);
}