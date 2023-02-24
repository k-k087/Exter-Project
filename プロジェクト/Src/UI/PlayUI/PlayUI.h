#pragma once

#include "vector"

#include "../UI.h"
#include "../../Character/Enemy/EnemyManager.h"

using namespace std;

class CPlayUI : public CUI
{
public:
	enum YesNoKind
	{
		YES_DARK,
		YES_RIGHT,

		NO_DARK,
		NO_RIGHT,

		YES_NO_NUM,
	};

	enum SelectKind
	{
		RETURN_GAME_DARK,
		RETURN_GAME_RIGHT,

		GO_TITLE_DARK,
		GO_TITLE_RIGHT,

		HOW_TO_DARK,
		HOW_TO_RIGHT,

		SELECT_NUM,
	};

	enum WarningKind
	{
		WARNING_BACK,
		WARNING_BLACK_UP,
		WARNING_BLACK_DOWN,
		WARNING_LEN,

		WARNING_NUM,
	};

	static const int PLAYER_BOOST_HANDLE_NUM = 16;

private:
	int m_playerStatesBar;

	int m_enemyHPHandle;
	int m_enemyHPBarHandle;
	int m_playerHPHandle;
	int m_playerMPHandle;
	int m_playerBar;

	int m_bossHP1Handle;
	int m_bossHP2Handle;

	int m_currentBossHP;
	int m_bossAllHP;
	int m_bossHP1;
	int m_bossHP2;
	int m_bossCountHP;
	bool m_bossFlg;


	int m_YesNoHandle[YES_NO_NUM];
	int m_SelectHandle[SELECT_NUM];
	int m_reallyHandle;
	int m_howToHandle;
	int m_selectCur;
	int m_yesNoCur;
	bool m_isReturnTitle;
	bool m_isHowTo;


	int m_playerBoostHandle[PLAYER_BOOST_HANDLE_NUM];
	int m_playerBoostNHandle;
	int m_boostCount;
	int m_currentBoostNum;

	int m_miniMapHandle;
	int m_playerCur;
	int m_akaten;

	int m_gameFinHandle[2];

	int m_timeHandle[12];

	int m_annihilateHandle;
	VECTOR m_annihilatePos;
	int m_stateHandle;
	VECTOR m_statePos;
	bool m_isStay;
	bool m_isThrough[2];

	float m_warningBlackMileage;
	float m_warningAlpha;
	float m_alphaSin;
	int m_warningCount;
	int m_warningHandle[WARNING_NUM];

	int m_signFrameHandle;

	int m_count1;
	int m_count2;

	CPlayUI() :
		m_warningHandle{-1},
		m_signFrameHandle(-1),
		m_alphaSin(),
		m_warningCount(),
		m_warningBlackMileage(),
		m_playerStatesBar(-1),
		m_playerBoostNHandle(-1),
		m_miniMapHandle(-1),
		m_timeHandle{-1},
		m_playerCur(-1),
		m_akaten(-1),
		m_isReturnTitle(false),
		m_isHowTo(false),
		m_selectCur(0),
		m_howToHandle(-1),
		m_gameFinHandle{-1},
		m_yesNoCur(0),
		m_boostCount(0),
		m_currentBoostNum(0),
		m_YesNoHandle{ -1 },
		m_SelectHandle{ -1 },
		m_reallyHandle(-1),
		m_enemyHPHandle(-1),
		m_bossFlg(false),
		m_bossHP1Handle(-1),
		m_bossHP2Handle(-1),
		m_enemyHPBarHandle(-1),
		m_playerHPHandle(-1),
		m_playerMPHandle(-1),
		m_playerBar(-1),
		m_annihilateHandle(-1),
		m_playerBoostHandle{-1},
		m_stateHandle(-1),
		m_annihilatePos{},
		m_currentBossHP(),
		m_statePos{},
		m_isThrough{ false },
		m_isStay(false),

		m_count1(),
		m_count2(),
		m_bossHP1(),
		m_bossHP2(),
		m_bossCountHP()
	{}

	~CPlayUI();

public:
	void Init();
	void Load();
	void Step();
	void Update();
	void Draw();
	void Fin();

	int StepPause();
	void DrawPause();

	void SetBossHP(int hp);
	void DrawBossHP(CEnemy* boss);

	bool StepStartPhase();
	void DrawStartPhase();

	void StepWarning();
	void DrawWarning();

	static CPlayUI* CreatePlayUI();


};