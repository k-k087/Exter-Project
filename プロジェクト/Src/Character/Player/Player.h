#pragma once

#include "DxLib.h"

#include "../Character.h"

#include "../../Anime/Anime.h"



static const float PLAYER_CENTER_Y = 23.f;

//�v���C���[�A�j�����
enum PlayerAnimeKind
{
	PLAYER_ANIME_KIND_NONE = -1,

	PLAYER_ANIME_KIND_1COMBO_ATTACK,		//���U���P
	PLAYER_ANIME_KIND_2COMBO_ATTACK,		//���U���Q
	PLAYER_ANIME_KIND_3COMBO_ATTACK,		//���U���R

	PLAYER_ANIME_KIND_2SLASH,				//�ォ��̉�
	PLAYER_ANIME_KIND_3SLASH_1,				//������������
	PLAYER_ANIME_KIND_3SLASH_2,				//������������
	PLAYER_ANIME_KIND_3ATTACK,				//�悱�悱����
	PLAYER_ANIME_KIND_KICK,					//�L�b�N
	PLAYER_ANIME_KIND_OUT_SLASH,			//�A�E�g�T�C�hSlash
	PLAYER_ANIME_KIND_ROT_JUMP_ATTACK,		//��]�W�����v
	PLAYER_ANIME_KIND_ROT_UP_ATTACK,		//��]�A�^�b�N
	PLAYER_ANIME_KIND_SIDE1_ATTACK,			//�T�C�hSlash
	PLAYER_ANIME_KIND_SIDE2_ATTACK,			//�T�C�h���T�C�h
	PLAYER_ANIME_KIND_UNDER_SLASH,			//��������Slash
	PLAYER_ANIME_KIND_UNDER_ATTACK,			//��������͋����U��

	PLAYER_ANIME_KIND_AVOID,				//���
	PLAYER_ANIME_KIND_DEATH,				//���S
	PLAYER_ANIME_KIND_DEFAULT,				//T�|�[�Y	
	PLAYER_ANIME_KIND_GET_UP,				//�N���オ��
	PLAYER_ANIME_KIND_HIT,					//�q�b�g
	PLAYER_ANIME_KIND_IDLE,					//�ҋ@
	PLAYER_ANIME_KIND_KNOCK_BACK,			//�m�b�N�o�b�N
	PLAYER_ANIME_KIND_POWER_UP,				//�p���[�A�b�v
	PLAYER_ANIME_KIND_RUN,					//����
	PLAYER_ANIME_KIND_SWORD_ATTACK1,		//�ʏ�U���P
	PLAYER_ANIME_KIND_SWORD_ATTACK2,		//�ʏ�U���Q
	PLAYER_ANIME_KIND_SWORD_ATTACK3,		//�ʏ�U���R
	PLAYER_ANIME_KIND_WALK,					//����
	
	PLAYER_ANIME_KIND_NUM,
};

//�v���C���[���
enum PlayerState
{
	PLAYER_STATE_NONE = -1,

	PLAYER_STATE_IDLE,						//�ҋ@
	PLAYER_STATE_WALK,						//����
	PLAYER_STATE_RUN,						//����
	PLAYER_STATE_AVOID,						//���
	PLAYER_STATE_ATTACK,					//�U��
	PLAYER_STATE_HIT,						//�q�b�g
	PLAYER_STATE_KNOCK_BACK,				//�m�b�N�o�b�N
	PLAYER_STATE_GET_UP,					//�N���オ��
	PLAYER_STATE_DEATH,						//���S	

	PLAYER_STATE_NUM,
};

enum PlayerAttackKind
{
	PLAYER_ATTACK_NONE = -1,

	PLAYER_ATTACK_NORMAL1,					//�ʏ�U���P
	PLAYER_ATTACK_NORMAL2,					//�ʏ�U���Q
	PLAYER_ATTACK_NORMAL3,					//�ʏ�U���R

	PLAYER_ATTACK_NORMAL4,					//�S

	PLAYER_ATTACK_NORMAL5,					//
	PLAYER_ATTACK_NORMAL6,					//
	PLAYER_ATTACK_NORMAL7,					//
	PLAYER_ATTACK_NORMAL8,					//
	PLAYER_ATTACK_NORMAL9,					//


	PLAYER_ATTACK_STRONG1,					//���U���P
	PLAYER_ATTACK_STRONG2,					//���U���Q
	PLAYER_ATTACK_STRONG3,					//���U���R

	PLAYER_ATTACK_RISING,

	PLAYER_ATTACK_NUM,
};

enum PlayerHitKind
{
	PLAYER_HIT_NONE = -1,

	PLAYER_HIT_ENEMY,
	PLAYER_HIT_NORMAL_ENEMY_ATTACK,
	PLAYER_HIT_GOLEM_ATTACK,
	PLAYER_HIT_GOLEM_MAGIC,

	PLAYER_HIT_NUM,
};

enum PlayerEffectKind
{
	PLAYER_EFFECT_KIND_NONE = -1,

	PLAYER_EFFECT_KIND_BARIIER,
	PLAYER_EFFECT_KIND_BIG_FIRE,
	PLAYER_EFFECT_KIND_FIRE,
	PLAYER_EFFECT_KIND_HIT1,
	PLAYER_EFFECT_KIND_HIT2,
	PLAYER_EFFECT_KIND_METEOR,
	PLAYER_EFFECT_KIND_SONIC_BOOM,
	PLAYER_EFFECT_KIND_SWORD1,
	PLAYER_EFFECT_KIND_SWORD2,
	PLAYER_EFFECT_KIND_THUNDER,
	PLAYER_EFFECT_KIND_POWER_UP,

	PLAYER_EFFECT_KIND_BEAM,
	PLAYER_EEFECT_KIND_ROAR,

	PLAYER_EFFECT_KIND_NUM
};

enum PlayerBoostKind
{
	PLAYER_BOOST_NONE = -1,

	PLAYER_BOOST_CHARGE,
	PLAYER_BOOST_GO,
	PLAYER_BOOST_NOW,

	PLAYER_BOOST_NUM,
};

struct PlayerAttackInfo
{
	int damage;
	int	barrier_damage;

	PlayerAttackKind attackKind;
};

//�v���C���[�N���X
class CPlayer : public CCharacter
{
public:
	static const int COMBO_NUM = 8;
	static const int BOOST_MAX_NUM = 100;
private:
	VECTOR	m_risingPos;						//���C�W���O�U���I�_���W
	VECTOR	m_risingMove;						//�ǂꂾ������Ă��邩
	VECTOR	m_risingStartPos;					//���C�W���O�U���X�^�[�g���W

	float	m_mp;								//MP
	float	m_maxMP;							//MaxMP
	float	m_boostNum;							//���݂̃u�[�X�g��
	int		m_swordHandle;						//���̃n���h��
	int		m_invincibleVal;					//���G�Ԋu
	int		m_normalAttackCount;				//�ʏ�U��
	int		m_attackCount;						//������U���̍U���J�E���g�p�ϐ�
	int		m_effectID[PLAYER_EFFECT_KIND_NUM];	//�G�t�F�N�gID
	int		m_boostEffect;	
	int		m_kickEffectID;	
	int		m_kickEffectCount;
	int		m_boostComboCount;

	int		m_comboMemory[COMBO_NUM];
	int		m_comboNum;

	float   m_speed;							//�X�s�[�h

	bool	m_isInvincible;						//���G���
	bool	m_isAvoid;							//������Ă��邩1
	bool	m_isDraw;							//�`�悷�邩
	bool	m_isAttack;							//�U������
	bool	m_isRisingAttack;					//���C�W���O�U������
	bool	m_isFloating;						//���V���邩
	bool	m_isRising;							//���C�W���O�U��



	int		m_paramI;
	float	m_paramF;
	bool	m_paramB;


	PlayerState			m_state;				//���
	PlayerAttackKind	m_attackKind;			//�U�����
	PlayerBoostKind		m_boostKind;			//���݂̃u�[�X�g���

	CAnimeInfo			m_animeInfo;			//�A�j�����

	CPlayer() :
		m_paramI(),
		m_paramF(),
		m_boostNum(),
		m_paramB(false),
		m_comboNum(),
		m_boostComboCount(),
		m_comboMemory{0},
		m_risingMove{},
		m_risingPos{},
		m_risingStartPos{},
		m_state(PLAYER_STATE_NONE),
		m_attackKind(PLAYER_ATTACK_NONE),
		m_boostKind(PLAYER_BOOST_CHARGE),
		m_swordHandle(-1),
		m_mp(),
		m_kickEffectCount(),
		m_maxMP(),
		m_attackCount(),
		m_kickEffectID(-1),
		m_isAvoid(false),
		m_boostEffect(-1),
		m_speed{},
		m_effectID{ PLAYER_EFFECT_KIND_NONE },
		m_isInvincible(false),
		m_isFloating(false),
		m_isRisingAttack(false),
		m_isDraw(true),
		m_isAttack(false),
		m_isRising(false),
		m_invincibleVal()
	{
		m_kind = OBJECT_KIND_PLAYER;
	}
private:
	//�v���C���[���͏���
	void InputPlayer();

	//�_�ŏ���
	void StepFlash();
	void DamagePlayer();

	//���̍X�V����
	void UpdateSword();

	//�d�͏���
	void StepGravity();

	//���C�W���O�X�e�b�v
	void StepRising();
	
	//MP�����񏈗�
	void HealingMP();
	bool CheckMP(const float mp);

	bool StepBoost();
	bool StepBoostTutorial();

	void ExecIdle();
	void ExecWalk();
	void ExecRun();
	void ExecAvoid();
	void ExecAttack();
	void ExecHit();
	void ExecKnockBack();
	void ExecGetUp();
	void ExecDown();
	void (CPlayer::*ExecFunc[PLAYER_STATE_NUM])() =
	{
		&CPlayer::ExecIdle,
		&CPlayer::ExecWalk,
		&CPlayer::ExecRun,
		&CPlayer::ExecAvoid,
		&CPlayer::ExecAttack,
		&CPlayer::ExecHit,
		&CPlayer::ExecKnockBack,
		&CPlayer::ExecGetUp,
		&CPlayer::ExecDown,
	};



	void ExecNormalAttack1();
	void ExecNormalAttack2();
	void ExecNormalAttack3();
	void ExecNormalAttack4();

	void ExecNormalAttack5();
	void ExecNormalAttack6();
	void ExecNormalAttack7();
	void ExecNormalAttack8();
	void ExecNormalAttack9();

	void ExecStrongAttack1();
	void ExecStrongAttack2();
	void ExecStrongAttack3();

	void ExecRisingAttack();
	void (CPlayer::*ExecAttackFunc[PLAYER_ATTACK_NUM])() =
	{
		&CPlayer::ExecNormalAttack1,
		&CPlayer::ExecNormalAttack2,
		&CPlayer::ExecNormalAttack3,
		&CPlayer::ExecNormalAttack4,

		&CPlayer::ExecNormalAttack5,
		&CPlayer::ExecNormalAttack6,
		&CPlayer::ExecNormalAttack7,
		&CPlayer::ExecNormalAttack8,
		&CPlayer::ExecNormalAttack9,

		&CPlayer::ExecStrongAttack1,
		&CPlayer::ExecStrongAttack2,
		&CPlayer::ExecStrongAttack3,

		&CPlayer::ExecRisingAttack,
	};



	void InputIdle();
	void InputWalk();
	void InputRun();
	void InputAvoid();
	void InputAttack();
	void InputHit();
	void InputKnockBack();
	void InputGetUp();
	void InputDown();
	void (CPlayer::*InputFunc[PLAYER_STATE_NUM])() =
	{
		&CPlayer::InputIdle,
		&CPlayer::InputWalk,
		&CPlayer::InputRun,
		&CPlayer::InputAvoid,
		&CPlayer::InputAttack,
		&CPlayer::InputHit,
		&CPlayer::InputKnockBack,
		&CPlayer::InputGetUp,
		&CPlayer::InputDown,
	};


	void IFWalkAndRun();
	void IFAvoid();
	void IFNormalAttack();
	void IFNormalAttack2();
	void IFStrongAttack();
	void IFRisingAttack();
	void IFBoost();



	void InputNormalAttack1();
	void InputNormalAttack2();
	void InputNormalAttack3();
	void InputNormalAttack4();

	void InputNormalAttack5();
	void InputNormalAttack6();
	void InputNormalAttack7();
	void InputNormalAttack8();
	void InputNormalAttack9();

	void InputStrongAttack1();
	void InputStrongAttack2();
	void InputStrongAttack3();

	void InputRisingAttack();
	void (CPlayer::*InputAttackFunc[PLAYER_ATTACK_NUM])() =
	{
		&CPlayer::InputNormalAttack1,
		&CPlayer::InputNormalAttack2,
		&CPlayer::InputNormalAttack3,
		&CPlayer::InputNormalAttack4,

		&CPlayer::InputNormalAttack5,
		&CPlayer::InputNormalAttack6,
		&CPlayer::InputNormalAttack7,
		&CPlayer::InputNormalAttack8,
		&CPlayer::InputNormalAttack9,

		&CPlayer::InputStrongAttack1,
		&CPlayer::InputStrongAttack2,
		&CPlayer::InputStrongAttack3,

		&CPlayer::InputRisingAttack,
	};
	

public:
	void Init() override;			//������
	void Load() override;			//���[�h
	void Step() override;			//�X�e�b�v
	void StepStartPhase();
	void StepFinPhase();
	void Update() override;			//�f�[�^���f����
	void Draw() override;			//�`��
	void Fin()override;				//�㏈��

	void ChangeAnime(PlayerAnimeKind kind);

	//�v���C���[�����蔻�菈��
	void HitFunc(PlayerHitKind kind);
	//�����蔻�肷�邩�ǂ���(true�Ȃ瓖���蔻����Ƃ�)
	bool JudgeCollission(PlayerHitKind kind);

	//�v���C���[���X�e�B�b�N�̌����ɂ���
	void LookLeftStick();
	//�v���C���[����ԋ߂��G�̌����ɂ���
	void FacingNearEnemy();

	void HealingMP(float heal);

	//�R���{�L��
	void MemoryCombo(int mem);
	bool IsMemoryCombo(int num);
	//�R���{���Z�b�g
	void InitCombo();

	void SetZeroMoveXY();
	void SetMoveXY(float x, float y);
	void SetMoveXY(VECTOR move);


	bool IsAttackRising();
	void ResetRising();
	void SetKeyRot();

	void StepStop();

	void ChargeBoost(const float charge_num);

	//MP����
	void SubMP(const float mp)
	{
		m_mp -= mp;
		if (m_mp <= 0) { m_mp = 0; }
	}

	//����
	static CPlayer* Create();

	//============================================================
	void StepTutorial();
	bool IsTutorialNormalAttack();
	bool IsTutorialNormalAttack2();
	bool IsTutorialStrongAttack();
	bool IsTutorialMove();
	bool IsTutorialAvoid();
	bool IsTutorialBoostGo();
	bool IsTutorialBoostAttack();
	//============================================================

	/*
		Getter&Setter
	*/

	//���W(Add)
	VECTOR GetAddPos(float add);

	//���S���W
	VECTOR GetCenterPos()const {
		VECTOR center_pos = m_pos;
		center_pos.y += PLAYER_CENTER_Y;
		return center_pos;
	}

	/*VECTOR GetUpCenterPos()const
	{
		VECTOR center_pos = m_pos;
	}*/

	float GetMP()const { return m_mp; }
	void SetMP(const float mp) { m_mp = mp; }

	float GetMaxMP()const { return m_maxMP; }
	void SetMaxMP(const float max_mp) { m_maxMP = max_mp; }

	//�X�s�[�h
	float GetSpeed()const { return m_speed; }
	void SetSpeed(float speed) { m_speed = speed; }

	//���G���
	bool GetIsInvincible()const { return m_isInvincible; }
	void SetIsInvincible(bool flg) { m_isInvincible = flg; }

	//�v���C���[�̏��
	PlayerState GetState()const { return m_state; }
	void SetState(const PlayerState state) { m_state = state; }

	PlayerAttackKind GetAttackKind()const { return m_attackKind; }
	void SetAttackKind(const PlayerAttackKind kind) { m_attackKind = kind; }

	float GetAttackRadius()const;
	VECTOR GetAttackCenterPos()const;
	float GetAttackCollisionFrame()const;

	VECTOR GetRisingStartPos()const { return m_risingStartPos; }
	void SetRisingStartPos(const VECTOR pos) { m_risingStartPos = pos; }

	VECTOR GetRisingPos()const { return m_risingPos; }
	void SetRisingPos(const VECTOR pos) { m_risingPos = pos; }

	VECTOR GetRisingMove()const { return m_risingMove; }
	void SetRisingMove(const VECTOR move) { m_risingMove = move; }

	bool GetIsAttack()const { return m_isAttack; }
	void SetIsAttack(const bool is_attack) { m_isAttack = is_attack; }

	bool GetIsRisingAttack()const { return m_isRisingAttack; }
	void SetIsRisingAttack(const bool is_attack) { m_isRisingAttack = is_attack; }

	bool GetIsAvoid()const { return m_isAvoid; }
	void SetIsAvoid(const bool flg) { m_isAvoid = flg; }

	float GetBoostNum()const { return m_boostNum; }
	void SetBoostNum(const float num) { m_boostNum = num; }
	
	PlayerBoostKind GetBoostKind()const { return m_boostKind; }
	void SetBoostKind(const PlayerBoostKind kind) { m_boostKind = kind; }

	int GetBoostComboCount()const { return m_boostComboCount; }
	void SetBoostComboCount(const int count) { m_boostComboCount = count; }

	int GetSwordHandle()const { return m_swordHandle; }
	void SetSwordHandle(const int handle) { m_swordHandle = handle; }

	int GetEffectID(PlayerEffectKind kind)const { return m_effectID[kind]; }

	CAnimeInfo* GetAnimeInfo() { return &m_animeInfo; }

	AttackType GetAttackType();

	PlayerAttackInfo GetPlayerAttackInfo();
};
