#pragma once

#include "DxLib.h"

#include "../Character.h"

#include "../../Anime/Anime.h"



static const float PLAYER_CENTER_Y = 23.f;

//プレイヤーアニメ種類
enum PlayerAnimeKind
{
	PLAYER_ANIME_KIND_NONE = -1,

	PLAYER_ANIME_KIND_1COMBO_ATTACK,		//強攻撃１
	PLAYER_ANIME_KIND_2COMBO_ATTACK,		//強攻撃２
	PLAYER_ANIME_KIND_3COMBO_ATTACK,		//強攻撃３

	PLAYER_ANIME_KIND_2SLASH,				//上からの下
	PLAYER_ANIME_KIND_3SLASH_1,				//したうえうえ
	PLAYER_ANIME_KIND_3SLASH_2,				//うえうえうえ
	PLAYER_ANIME_KIND_3ATTACK,				//よこよこたて
	PLAYER_ANIME_KIND_KICK,					//キック
	PLAYER_ANIME_KIND_OUT_SLASH,			//アウトサイドSlash
	PLAYER_ANIME_KIND_ROT_JUMP_ATTACK,		//回転ジャンプ
	PLAYER_ANIME_KIND_ROT_UP_ATTACK,		//回転アタック
	PLAYER_ANIME_KIND_SIDE1_ATTACK,			//サイドSlash
	PLAYER_ANIME_KIND_SIDE2_ATTACK,			//サイド＆サイド
	PLAYER_ANIME_KIND_UNDER_SLASH,			//したからSlash
	PLAYER_ANIME_KIND_UNDER_ATTACK,			//したから力強い攻撃

	PLAYER_ANIME_KIND_AVOID,				//回避
	PLAYER_ANIME_KIND_DEATH,				//死亡
	PLAYER_ANIME_KIND_DEFAULT,				//Tポーズ	
	PLAYER_ANIME_KIND_GET_UP,				//起き上がる
	PLAYER_ANIME_KIND_HIT,					//ヒット
	PLAYER_ANIME_KIND_IDLE,					//待機
	PLAYER_ANIME_KIND_KNOCK_BACK,			//ノックバック
	PLAYER_ANIME_KIND_POWER_UP,				//パワーアップ
	PLAYER_ANIME_KIND_RUN,					//走り
	PLAYER_ANIME_KIND_SWORD_ATTACK1,		//通常攻撃１
	PLAYER_ANIME_KIND_SWORD_ATTACK2,		//通常攻撃２
	PLAYER_ANIME_KIND_SWORD_ATTACK3,		//通常攻撃３
	PLAYER_ANIME_KIND_WALK,					//歩き
	
	PLAYER_ANIME_KIND_NUM,
};

//プレイヤー状態
enum PlayerState
{
	PLAYER_STATE_NONE = -1,

	PLAYER_STATE_IDLE,						//待機
	PLAYER_STATE_WALK,						//歩き
	PLAYER_STATE_RUN,						//走り
	PLAYER_STATE_AVOID,						//回避
	PLAYER_STATE_ATTACK,					//攻撃
	PLAYER_STATE_HIT,						//ヒット
	PLAYER_STATE_KNOCK_BACK,				//ノックバック
	PLAYER_STATE_GET_UP,					//起き上がる
	PLAYER_STATE_DEATH,						//死亡	

	PLAYER_STATE_NUM,
};

enum PlayerAttackKind
{
	PLAYER_ATTACK_NONE = -1,

	PLAYER_ATTACK_NORMAL1,					//通常攻撃１
	PLAYER_ATTACK_NORMAL2,					//通常攻撃２
	PLAYER_ATTACK_NORMAL3,					//通常攻撃３

	PLAYER_ATTACK_NORMAL4,					//４

	PLAYER_ATTACK_NORMAL5,					//
	PLAYER_ATTACK_NORMAL6,					//
	PLAYER_ATTACK_NORMAL7,					//
	PLAYER_ATTACK_NORMAL8,					//
	PLAYER_ATTACK_NORMAL9,					//


	PLAYER_ATTACK_STRONG1,					//強攻撃１
	PLAYER_ATTACK_STRONG2,					//強攻撃２
	PLAYER_ATTACK_STRONG3,					//強攻撃３

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

//プレイヤークラス
class CPlayer : public CCharacter
{
public:
	static const int COMBO_NUM = 8;
	static const int BOOST_MAX_NUM = 100;
private:
	VECTOR	m_risingPos;						//ライジング攻撃終点座標
	VECTOR	m_risingMove;						//どれだけ離れているか
	VECTOR	m_risingStartPos;					//ライジング攻撃スタート座標

	float	m_mp;								//MP
	float	m_maxMP;							//MaxMP
	float	m_boostNum;							//現在のブースト数
	int		m_swordHandle;						//剣のハンドル
	int		m_invincibleVal;					//無敵間隔
	int		m_normalAttackCount;				//通常攻撃
	int		m_attackCount;						//複数回攻撃の攻撃カウント用変数
	int		m_effectID[PLAYER_EFFECT_KIND_NUM];	//エフェクトID
	int		m_boostEffect;	
	int		m_kickEffectID;	
	int		m_kickEffectCount;
	int		m_boostComboCount;

	int		m_comboMemory[COMBO_NUM];
	int		m_comboNum;

	float   m_speed;							//スピード

	bool	m_isInvincible;						//無敵状態
	bool	m_isAvoid;							//回避しているか1
	bool	m_isDraw;							//描画するか
	bool	m_isAttack;							//攻撃判定
	bool	m_isRisingAttack;					//ライジング攻撃判定
	bool	m_isFloating;						//浮遊するか
	bool	m_isRising;							//ライジング攻撃



	int		m_paramI;
	float	m_paramF;
	bool	m_paramB;


	PlayerState			m_state;				//状態
	PlayerAttackKind	m_attackKind;			//攻撃種類
	PlayerBoostKind		m_boostKind;			//現在のブースト種類

	CAnimeInfo			m_animeInfo;			//アニメ情報

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
	//プレイヤー入力処理
	void InputPlayer();

	//点滅処理
	void StepFlash();
	void DamagePlayer();

	//剣の更新処理
	void UpdateSword();

	//重力処理
	void StepGravity();

	//ライジングステップ
	void StepRising();
	
	//MPかんれん処理
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
	void Init() override;			//初期化
	void Load() override;			//ロード
	void Step() override;			//ステップ
	void StepStartPhase();
	void StepFinPhase();
	void Update() override;			//データ反映処理
	void Draw() override;			//描画
	void Fin()override;				//後処理

	void ChangeAnime(PlayerAnimeKind kind);

	//プレイヤー当たり判定処理
	void HitFunc(PlayerHitKind kind);
	//当たり判定するかどうか(trueなら当たり判定をとる)
	bool JudgeCollission(PlayerHitKind kind);

	//プレイヤーをスティックの向きにする
	void LookLeftStick();
	//プレイヤーを一番近い敵の向きにする
	void FacingNearEnemy();

	void HealingMP(float heal);

	//コンボ記憶
	void MemoryCombo(int mem);
	bool IsMemoryCombo(int num);
	//コンボリセット
	void InitCombo();

	void SetZeroMoveXY();
	void SetMoveXY(float x, float y);
	void SetMoveXY(VECTOR move);


	bool IsAttackRising();
	void ResetRising();
	void SetKeyRot();

	void StepStop();

	void ChargeBoost(const float charge_num);

	//MP減少
	void SubMP(const float mp)
	{
		m_mp -= mp;
		if (m_mp <= 0) { m_mp = 0; }
	}

	//生成
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

	//座標(Add)
	VECTOR GetAddPos(float add);

	//中心座標
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

	//スピード
	float GetSpeed()const { return m_speed; }
	void SetSpeed(float speed) { m_speed = speed; }

	//無敵状態
	bool GetIsInvincible()const { return m_isInvincible; }
	void SetIsInvincible(bool flg) { m_isInvincible = flg; }

	//プレイヤーの状態
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
