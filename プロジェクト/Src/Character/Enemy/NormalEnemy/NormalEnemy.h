#pragma once

#include "DxLib.h"

#include "../Enemy.h"
#include "../../../Draw/DrawManager.h"

#include "../../../Common.h"

static const float NORMAL_ENEMY_CENTER_Y = 18.f;

//�G�A�j�����
enum NormalEnemyAnimeKind
{
	NORMAL_ENEMY_ANIME_KIND_NONE = -1,

	NORMAL_ENEMY_ANIME_KIND_ATTACK,
	NORMAL_ENEMY_ANIME_KIND_DOWN,
	NORMAL_ENEMY_ANIME_KIND_HIT,
	NORMAL_ENEMY_ANIME_KIND_IDLE,
	NORMAL_ENEMY_ANIME_KIND_WALK,

	NORMAL_ENEMY_ANIME_KIND_NUM,
};


//�ʏ�G�N���X
class CNormalEnemy : public CEnemy
{

private:
	int			m_attackCount;			//�U���J�E���g�p�ϐ�

	CNormalEnemy() : 
		m_attackCount()
	{
		m_kind = OBJECT_KIND_PLAYER;
		m_enemyKind = ENEMY_KIND_NORMAL;
	}
private:

	//�_�ŏ���
	void StepFlash();

	void ExecIdle();
	void ExecAvoid();
	void ExecWalk();
	void ExecTrack();
	void ExecAttackReady();
	void ExecAttack();
	void ExecHit();
	void ExecDown();



public:
	void Init()		override;			//������
	void Load()		override;			//���[�h
	void Load(int model_handle) override;
	void Step()		override;			//�X�e�b�v
	void Update()	override;			//�f�[�^���f����
	void Draw()		override;			//�`��
	void Fin()		override;			//�㏈��

	void Reset();	

	//�A�j���ύX
	void ChangeAnime(NormalEnemyAnimeKind kind);
	void ChangeAnimeToState();

	//�����蔻�菈��
	void HitFunc(EnemyHitKind hit_kind);

	//�����蔻�肷�邩�ǂ���(true�Ȃ瓖���蔻����Ƃ�)
	bool JudgeCollission(EnemyHitKind kind);

	void Appear();
	void Appear(VECTOR start_pos, bool is_barrier = false);

	void Killing();


	//����
	static CNormalEnemy* Create(int create_num)
	{
		CNormalEnemy* normal_enemy = _NEW(CNormalEnemy);

		normal_enemy->Init();
		normal_enemy->SetCreateNum(create_num);

		//DrawList�ɒǉ�
		normal_enemy->m_drawID = CDrawManager::AddData(normal_enemy);

		return normal_enemy;
	}


	/*
		Getter&Setter
	*/

	//���S���W
	VECTOR GetCenterPos() {
		VECTOR center_pos = m_pos;
		center_pos.y += NORMAL_ENEMY_CENTER_Y;
		return center_pos;
	}

	//Hp
	int GetHP()const { return m_hp; }
	void SetHP(int hp) { m_hp = hp; }


	float GetAttackRadius();
	VECTOR GetAttackCenterPos();


	AttackType GetAttackType();

	int GetAttackKind() { return -1; }
};
