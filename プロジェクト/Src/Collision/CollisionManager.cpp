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

//������
void CCollisionManager::Init()
{
	//�}�l�[�W���[���X�g�ɒǉ�


	CGameManager::AddMGList(this);
}

//�X�e�b�v
//���ׂĂ̓����蔻�������
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
	//�v���C���[�ƓG�̓����蔻��
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	if (player->GetAttackKind() == PLAYER_ATTACK_RISING) { return; }

	//�K�v���Q�b�g
	VECTOR player_center_pos = player->GetCenterPos();
	float player_radius = player->GetRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//�����蔻����Ƃ邩
		if (!enemy_list->JudgeCollission(ENEMY_HIT_PLAYER))continue;

		//�K�v���Q�b�g
		VECTOR enemy_center_pos = enemy_list->GetCenterPos();
		float enemy_radius = enemy_list->GetRadius();

		//�������Ă��邩�`�F�b�N
		if (CCollision::IsHitBall(player_center_pos, player_radius, enemy_center_pos, enemy_radius))
		{
			VECTOR enemy_move = enemy_list->GetMove();
			VECTOR player_move = player->GetMove();

			//���܂��Ă��钷�����o��
			float radius_sum = player_radius + enemy_radius;

			//�v���C���[����G�ւ̃x�N�g��
			VECTOR distance_vec = CMyMath::VecCreate(player_center_pos, enemy_center_pos);

			float distance = CMyMath::VecLong(distance_vec);
			float scale = radius_sum - distance;

			//Y�����͂O�ɂ���
			distance_vec.y = 0.f;

			//���܂��Ă��镪���̌����ɉ����o��
			distance_vec = CMyMath::VecNormalize(distance_vec);
			distance_vec = CMyMath::VecScale(distance_vec, scale);

			//�܂��A����̎��̏���
			if (enemy_list->GetEnemyKind() == ENEMY_KIND_BOSS1 &&
				enemy_list->GetAttackKind() == BOSS1_ATTACK_JUMP)
			{
				//�v���C���[���������o��
				distance_vec = CMyMath::VecScale(distance_vec, -1.f);
				player->AddPos(distance_vec);
			}
			//�v���C���[���������Ă���ꍇ
			else if (!CCommon::CheckZeroVector(player_move) && CCommon::CheckZeroVector(enemy_move))
			{
				//�v���C���[���������o��
				distance_vec = CMyMath::VecScale(distance_vec, -1.f);
				player->AddPos(distance_vec);

			}
			//�G���������Ă���ꍇ
			else if (CCommon::CheckZeroVector(player_move) && !CCommon::CheckZeroVector(enemy_move))
			{
				//�G���������o��
				enemy_list->AddPos(distance_vec);
			}
			//���������Ă���ꍇ
			else if (!CCommon::CheckZeroVector(player_move) && !CCommon::CheckZeroVector(enemy_move))
			{
				//���������������o��
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
	//�v���C���[�ƓG���E�̓����蔻��
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	//�K�v���Q�b�g
	VECTOR player_center_pos = player->GetCenterPos();
	float player_radius = player->GetRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//�����蔻����Ƃ邩
		if (!enemy_list->JudgeCollission(ENEMY_HORIZON_HIT_PLAYER))continue;

		//�K�v���Q�b�g
		VECTOR enemy_center_pos = enemy_list->GetCenterPos();
		float enemy_horizon_radius = enemy_list->GetHorizonRadius();

		//�������Ă��邩�`�F�b�N
		if (CCollision::IsHitBall(player_center_pos, player_radius, enemy_center_pos, enemy_horizon_radius))
		{
			//�ǐՏ�Ԃ�
			enemy_list->SetState(ENEMY_STATE_TRACK);
			enemy_list->ChangeAnimeToState();
		}
		else
		{
			//�ҋ@��Ԃ�
			enemy_list->SetState(ENEMY_STATE_IDLE);
			enemy_list->ChangeAnimeToState();
		}
	}
}

//	�v���C���[�U��
void CCollisionManager::CheckPlayerAttackHitEnemy()
{

	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	//�U�����肪�Ȃ��Ȃ�return
	if (!player->GetIsAttack()) { return; }
	
	//�K�v���Q�b�g
	VECTOR attack_center_pos = player->GetAttackCenterPos();
	float player_attack_radius = player->GetAttackRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//�����蔻����Ƃ邩
		if (!enemy_list->JudgeCollission(ENEMY_HIT_PLAYER_ATTACK))continue;

		//�K�v���Q�b�g
		VECTOR enemy_center_pos = enemy_list->GetCenterPos();
		float enemy_radius = enemy_list->GetRadius();

		//�������Ă��邩�`�F�b�N
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

	//�U������t���O��false��
	player->SetIsAttack(false);

#ifdef DEBUG_MODE

	DrawSphere3D(attack_center_pos, player_attack_radius, 32, GetColor(255, 50, 50), GetColor(0, 0, 0), false);

#endif
}
bool CCollisionManager::IsCheckPlayerAttackHitEnemy()
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	//�U�����肪�Ȃ��Ȃ�return
	if (!player->GetIsAttack()) { return false; }

	//�K�v���Q�b�g
	VECTOR attack_center_pos = player->GetAttackCenterPos();
	float player_attack_radius = player->GetAttackRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//�����蔻����Ƃ邩
		if (!enemy_list->JudgeCollission(ENEMY_HIT_PLAYER_ATTACK))continue;

		//�K�v���Q�b�g
		VECTOR enemy_center_pos = enemy_list->GetCenterPos();
		float enemy_radius = enemy_list->GetRadius();

		//�������Ă��邩�`�F�b�N
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

	//�K�v���Q�b�g
	VECTOR player_center_pos = player->GetCenterPos();
	float player_radius = player->GetRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//�����蔻����Ƃ邩
		if (!enemy_list->JudgeCollission(ENEMY_ATTACK_HIT_PLAYER))continue;

		//�K�v���Q�b�g
		VECTOR enemy_attack_pos = enemy_list->GetAttackCenterPos();
		float enemy_attack_radius = enemy_list->GetAttackRadius();

		//�������Ă��邩�`�F�b�N
		if (CCollision::IsHitBall(player_center_pos, player_radius, enemy_attack_pos, enemy_attack_radius))
		{
			player->HitFunc(PLAYER_HIT_NORMAL_ENEMY_ATTACK);

			//�G�̕�������
			VECTOR enemy_pos = enemy_list->GetPos();
			VECTOR vec = CMyMath::VecCreate(player_center_pos, enemy_pos);
			VECTOR rot = { 0.f, CMyMath::VecDir(vec.x, vec.z) + DX_PI_F, 0.f };

			player->SetRot(rot);
		}


#ifdef DEBUG_MODE

		DrawSphere3D(enemy_attack_pos, enemy_attack_radius, 32, GetColor(255, 50, 50), GetColor(0, 0, 0), false);

#endif

		////�U������t���O��false��
		//enemy_list->SetIsAttack(false);
	}
}

void CCollisionManager::CheckPlayerRisingHitEnemy()
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	//���C�W���O�U�����肪�Ȃ��Ȃ�return
	if (!player->GetIsRisingAttack()) { return; }

	//�K�v���Q�b�g
	float player_attack_radius = player->GetAttackRadius();

	VECTOR start_pos = player->GetRisingStartPos();
	VECTOR end_pos = player->GetRisingPos();
	VECTOR rising_move = CMyMath::VecScale(CMyMath::VecNormalize(player->GetRisingMove()),
		player_attack_radius);


	//�ۑ��p
	vector<CEnemy*> enemy_p; enemy_p.clear();
	bool is_stop = false;

	

	while (1)
	{
		//�ǂ�ǂ�i�߂�
		start_pos = CMyMath::VecAdd(start_pos, rising_move);

		for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
		{
			//�����蔻����Ƃ邩
			if (!enemy_list->JudgeCollission(ENEMY_HIT_PLAYER_ATTACK))continue;

			//���łɓ����蔻����Ƃ�����Ƃ͂Ƃ�Ȃ�
			bool is_check = true;
			for (auto enemy_p_vec : enemy_p)
				if (enemy_p_vec == enemy_list) { is_check = false; break; }
			if (!is_check) { continue; }
			

			//�K�v���Q�b�g
			VECTOR enemy_center_pos = enemy_list->GetCenterPos();
			float enemy_radius = enemy_list->GetRadius();

			//�������Ă��邩�`�F�b�N
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

		//�I�_�Ɠ��������疳�����[�v�I��
		if (CCollision::IsHitBall(start_pos, player_attack_radius, end_pos, 1.f))
			break;
	}


	if (is_stop) { /*CAnimeInfo::AllStopAnime(150);*/ }
	enemy_p.clear();

	//�U������t���O��false��
	player->SetIsRisingAttack(false);
}

/*
	�G���m�̓����蔻��
*/
void CCollisionManager::CheckEnemyHitEnemy()
{
	for (const auto& enemy_list1 : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//�����蔻����Ƃ邩
		if (!enemy_list1->JudgeCollission(ENEMY_HIT_ENEMY))continue;

		//�K�v���Q�b�g
		VECTOR enemy1_center_pos = enemy_list1->GetCenterPos();
		float enemy1_radius = enemy_list1->GetEnemyHitRadius();

		for (const auto& enemy_list2 : *CEnemyManager::GetManager()->GetEnemyList())
		{
			//�����G�Ƃ͓����蔻��͂Ƃ�Ȃ�
			if (enemy_list1 == enemy_list2)continue;
			//�����蔻����Ƃ邩
			if (!enemy_list2->JudgeCollission(ENEMY_HIT_ENEMY))continue;

			//�K�v���Q�b�g
			VECTOR enemy2_center_pos = enemy_list2->GetCenterPos();
			float enemy2_radius = enemy_list2->GetEnemyHitRadius();

			//�������Ă��邩�`�F�b�N
			if (CCollision::IsHitBall(enemy1_center_pos, enemy1_radius, enemy2_center_pos, enemy2_radius))
			{
				VECTOR enemy1_move = enemy_list1->GetMove();
				VECTOR enemy2_move = enemy_list2->GetMove();

				//���܂��Ă��钷�����o��
				float radius_sum = enemy1_radius + enemy2_radius;

				//�G1����G�Q�ւ̃x�N�g��
				VECTOR distance_vec = CMyMath::VecCreate(enemy1_center_pos, enemy2_center_pos);

				float distance = CMyMath::VecLong(distance_vec);
				float scale = radius_sum - distance;

				//Y�����͂O�ɂ���
				distance_vec.y = 0.f;

				//���܂��Ă��镪���̌����ɉ����o��
				distance_vec = CMyMath::VecNormalize(distance_vec);
				distance_vec = CMyMath::VecScale(distance_vec, scale);

				//�G�P���������Ă���ꍇ
				if (!CCommon::CheckZeroVector(enemy1_move) && CCommon::CheckZeroVector(enemy2_move))
				{
					//�G�P���������o��
					distance_vec = CMyMath::VecScale(distance_vec, -1.f);
					enemy_list1->AddPos(distance_vec);

				}
				//�G�Q���������Ă���ꍇ
				else if (CCommon::CheckZeroVector(enemy1_move) && !CCommon::CheckZeroVector(enemy2_move))
				{
					//�G�Q���������o��
					enemy_list2->AddPos(distance_vec);
				}
				//���������Ă���ꍇ
				else if (!CCommon::CheckZeroVector(enemy1_move) && !CCommon::CheckZeroVector(enemy2_move))
				{
					//���������������o��
					distance_vec = CMyMath::VecScale(distance_vec, 0.5f);
					VECTOR distance_vec_ = CMyMath::VecScale(distance_vec, -1.f);
					enemy_list2->AddPos(distance_vec);
					enemy_list1->AddPos(distance_vec_);
				}
			}
		}
	}
}


//	�G�U���͈͂ƃv���C���[�̓����蔻��
void CCollisionManager::CheckEnemyAttackRangeHitPlayer()
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	//�v���C���[�̏�Ԃ��_�E���Ȃ�return
	if (player->GetState() == PLAYER_STATE_KNOCK_BACK) { return; }

	//�K�v���Q�b�g
	VECTOR player_center_pos = player->GetCenterPos();
	float player_radius = player->GetRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//�����蔻����Ƃ邩
		if (!enemy_list->JudgeCollission(ENEMY_RANGE_HIT_PLAYER))continue;

		//�K�v���Q�b�g
		VECTOR enemy_center_pos = enemy_list->GetCenterPos();
		float enemy_range = enemy_list->GetAttackRange();

		//�������Ă��邩�`�F�b�N
		if (CCollision::IsHitBall(player_center_pos, player_radius, enemy_center_pos, enemy_range))
		{
			//�U��������Ԃ�
			enemy_list->SetState(ENEMY_STATE_ATTACK_READY);
			enemy_list->ChangeAnimeToState();
		}
		else
		{
			//�ǐՏ�Ԃ�
			enemy_list->SetState(ENEMY_STATE_TRACK);
			enemy_list->ChangeAnimeToState();
		}
	}
}

//	�G�U���ƃv���C���[�̓����蔻��
void CCollisionManager::CheckEnemyAttackHitPlayer()
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	if (!player->JudgeCollission(PLAYER_HIT_NORMAL_ENEMY_ATTACK)) { return; }

	//�K�v���Q�b�g
	VECTOR player_center_pos = player->GetCenterPos();
	float player_radius = player->GetRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//�����蔻����Ƃ邩
		if (!enemy_list->JudgeCollission(ENEMY_ATTACK_HIT_PLAYER))continue;

		//�K�v���Q�b�g
		VECTOR enemy_attack_pos = enemy_list->GetAttackCenterPos();
		float enemy_attack_radius = enemy_list->GetAttackRadius();

		//�������Ă��邩�`�F�b�N
		if (CCollision::IsHitBall(player_center_pos, player_radius, enemy_attack_pos, enemy_attack_radius))
		{
			player->HitFunc(PLAYER_HIT_NORMAL_ENEMY_ATTACK);

			//�G�̕�������
			VECTOR enemy_pos = enemy_list->GetPos();
			VECTOR vec = CMyMath::VecCreate(player_center_pos, enemy_pos);
			VECTOR rot = { 0.f, CMyMath::VecDir(vec.x, vec.z) + DX_PI_F, 0.f };

			player->SetRot(rot);
		}


#ifdef DEBUG_MODE

		DrawSphere3D(enemy_attack_pos, enemy_attack_radius, 32, GetColor(255, 50, 50), GetColor(0, 0, 0), false);

#endif

		////�U������t���O��false��
		//enemy_list->SetIsAttack(false);
	}
}


//�G���@�ƃv���C���[�̓����蔻��
void CCollisionManager::CheckEnemyMagicHitPlayer()
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	if (!player->JudgeCollission(PLAYER_HIT_NORMAL_ENEMY_ATTACK)) { return; }

	//�K�v���Q�b�g
	VECTOR player_center_pos = player->GetCenterPos();
	float player_radius = player->GetRadius();

	for (const auto& enemy_magic : *CMagicManager::GetManager()->GetEnemyMagic())
	{
		if (!enemy_magic->GetIsUse()) { return; }

		//�K�v���Q�b�g
		VECTOR magic_pos = enemy_magic->GetPos();
		float magic_radius = enemy_magic->GetRadius();

		if (CCollision::IsHitBall(player_center_pos, player_radius, magic_pos, magic_radius))
		{
			player->HitFunc(PLAYER_HIT_GOLEM_MAGIC);

			//���@�̒��S���W�̕�������
			VECTOR vec = CMyMath::VecCreate(player_center_pos, magic_pos);
			VECTOR rot = { 0.f, CMyMath::VecDir(vec.x, vec.z) + DX_PI_F, 0.f };

			player->SetRot(rot);
		}
	}
}

//�G�r�[���ƃv���C���[�����蔻��
void CCollisionManager::CheckEnemyBeamHitPlayer()
{
	
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	if (!player->JudgeCollission(PLAYER_HIT_NORMAL_ENEMY_ATTACK)) { return; }

	//�K�v���Q�b�g
	VECTOR player_center_pos = player->GetCenterPos();
	float player_radius = player->GetRadius();

	for (const auto& enemy_beam : *CBeamManager::GetManager()->GetEnemyBeam())
	{
		if (!enemy_beam->GetIsUse()) { return; }

		//�K�v���Q�b�g
		VECTOR beam_coll_pos = enemy_beam->GetPos();
		float beam_coll_radius = enemy_beam->GetRadius();

		if (CCollision::IsHitBall(player_center_pos, player_radius, beam_coll_pos, beam_coll_radius))
		{
			player->HitFunc(PLAYER_HIT_GOLEM_MAGIC);

			//���@�̒��S���W�̕�������
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

//	�v���C���[�ƃX�e�[�W�̓����蔻��
void CCollisionManager::CheckPlayerHitStage()
{
	//�v���C���[�ƓG�̓����蔻��
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();

	//�K�v���Q�b�g
	VECTOR player_center_pos = player->GetCenterPos();
	player_center_pos.y = 0.f;
	float player_radius = player->GetRadius();

	CField* field = CFieldManager::GetManager()->GetField();

	VECTOR field_pos = field->GetPos();
	field_pos.y = 0.f;
	float field_radius = field->GetRadius();

	//	���������Ă��Ȃ���Ζ߂�
	if (!CCollision::IsHitBall(player_center_pos, player_radius, field_pos, field_radius))
	{
		//���܂��Ă��钷�����o��
		float radius_sum = player_radius + field_radius;

		//�x�N�g��
		VECTOR distance_vec = CMyMath::VecCreate(player_center_pos, field_pos);
		float len = CMyMath::VecLong(distance_vec);
		
		float sub = len - radius_sum;

		//	�x�N�g���𐳋K�����A�g�債�ă^�X
		distance_vec = CMyMath::VecScale(CMyMath::VecNormalize(distance_vec), sub);

		player->AddPos(distance_vec);
	}

}

//�}�l�[�W���[����
CCollisionManager* CCollisionManager::CreateManager()
{
	//�Ȃ��Ȃ���
	if (!m_collisionManager)
	{
		m_collisionManager = _NEW(CCollisionManager);
	}

	return m_collisionManager;
}
//�}�l�[�W���[�Q�b�g
CCollisionManager* CCollisionManager::GetManager()
{
	if (!m_collisionManager) { return nullptr; }
	return m_collisionManager;
}
//�}�l�[�W���[�폜
void CCollisionManager::DeleteManager()
{
	//����Ȃ����
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

	//�U�����肪�Ȃ��Ȃ�return
	if (!player->GetIsAttack()) { return; }

	//�K�v���Q�b�g
	VECTOR attack_center_pos = player->GetAttackCenterPos();
	float player_attack_radius = player->GetAttackRadius();

	for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
	{
		//�����蔻����Ƃ邩
		if (!enemy_list->JudgeCollission(ENEMY_HIT_PLAYER_ATTACK))continue;

		//�K�v���Q�b�g
		VECTOR enemy_center_pos = enemy_list->GetCenterPos();
		float enemy_radius = enemy_list->GetRadius();

		//�������Ă��邩�`�F�b�N
		if (CCollision::IsHitBall(attack_center_pos, player_attack_radius, enemy_center_pos, enemy_radius))
		{
			
			//	�v���C���[�̍U���͂������Ă���
			PlayerAttackInfo attack_info = player->GetPlayerAttackInfo();
			//	�G��HP�������Ă���
			int enemy_hp = enemy_list->GetHP();
			//	�U���͂�HP�������Ă�����{5����
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


	//�U������t���O��false��
	player->SetIsAttack(false);
}
void CCollisionManager::CheckPlayerAttackHitEnemyTutorial2(TutorialPhase tutorial_phase)
{
	CPlayer* player = CPlayerManager::GetManager()->GetPlayer();


	//���C�W���O�U�����肪�Ȃ��Ȃ�return
	if (!player->GetIsRisingAttack()) { return; }

	float player_attack_radius = player->GetAttackRadius();

	VECTOR start_pos = player->GetRisingStartPos();
	VECTOR end_pos = player->GetRisingPos();
	VECTOR rising_move = CMyMath::VecScale(CMyMath::VecNormalize(player->GetRisingMove()),
		player_attack_radius);


	//�ۑ��p
	vector<CEnemy*> enemy_p; enemy_p.clear();
	bool is_stop = false;


	while (1)
	{
		//�ǂ�ǂ�i�߂�
		start_pos = CMyMath::VecAdd(start_pos, rising_move);

		for (const auto& enemy_list : *CEnemyManager::GetManager()->GetEnemyList())
		{
			//�����蔻����Ƃ邩
			if (!enemy_list->JudgeCollission(ENEMY_HIT_PLAYER_ATTACK))continue;

			//���łɓ����蔻����Ƃ�����Ƃ͂Ƃ�Ȃ�
			bool is_check = true;
			for (auto enemy_p_vec : enemy_p)
				if (enemy_p_vec == enemy_list) { is_check = false; break; }
			if (!is_check) { continue; }


			//�K�v���Q�b�g
			VECTOR enemy_center_pos = enemy_list->GetCenterPos();
			float enemy_radius = enemy_list->GetRadius();

			//�������Ă��邩�`�F�b�N
			if (CCollision::IsHitBall(start_pos, player_attack_radius, enemy_center_pos, enemy_radius))
			{
				
				//	�v���C���[�̍U���͂������Ă���
				PlayerAttackInfo attack_info = player->GetPlayerAttackInfo();
				//	�G��HP�������Ă���
				int enemy_hp = enemy_list->GetHP();

				//	�U���͂�HP�������Ă�����{5����
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

		//�I�_�Ɠ��������疳�����[�v�I��
		if (CCollision::IsHitBall(start_pos, player_attack_radius, end_pos, 1.f))
			break;
	}


	if (is_stop) { /*CAnimeInfo::AllStopAnime(150);*/ }
	enemy_p.clear();

	//�U������t���O��false��
	player->SetIsRisingAttack(false);
}