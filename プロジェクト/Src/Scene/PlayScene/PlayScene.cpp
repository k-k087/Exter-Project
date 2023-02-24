#include "DxLib.h"

#include "PlayScene.h"

#include "../Scene.h"

#include "../../Character/CharacterManager.h"
#include "../../Character/Player/PlayerManager.h"
#include "../../Character/Player/Player.h"
#include "../../Character/Enemy/EnemyManager.h"
#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"
#include "../../Collision/CollisionManager.h"
#include "../../Input/Input.h"
#include "../../Common.h"
#include "../../Camera/CameraManager.h"
#include "../../Input/Input.h"
#include "../../Debug/Debug.h"
#include "../../Field/Field.h"
#include "../../Character/Player/Player.h"
#include "../../Scene/SceneManager.h"
#include "../../Sky/SkyManager.h"
#include "../../Field/FieldManager.h"
#include "../../GameManager/GameManager.h"
#include "../../Draw/DrawManager.h"
#include "../../Sound/SoundManager.h"
#include "../../Score/Score.h"
#include "../../SaveData/SaveData.h"
#include "../../UI/UIManager.h"
#include "../../Magic/MagicManager.h"
#include "../../Effekseer/Effekseer.h"
#include "../../Beam/BeamManager.h"
#include "../../MyMath/MyMath.h"

#include "../../Text/TextData.h"

#include "../../Input/InputManager.h"

static const int TUTORIAL_TEXT_NUM[TUTORIAL_NUM] =
{
	TUTORIAL_TEXT_START_NUM,
	TUTORIAL_TEXT_AVOID_NUM,
	TUTORIAL_TEXT_MOVE_NUM,
	TUTORIAL_TEXT_ENEMY_NUM,
	TUTORIAL_TEXT_ATTACK_NUM,
	TUTORIAL_TEXT_ATTACK2_NUM,
	TUTORIAL_TEXT_STRONG_ATTACK_NUM,
	TUTORIAL_TEXT_BOOST_GO_NUM,
	TUTORIAL_TEXT_BOOST_ATTACK_NUM,
	TUTORIAL_TEXT_END_NUM,
};

static const char** TUTORIAL_TEXT[TUTORIAL_NUM] =
{
	TUTORIAL_TEXT_START,
	TUTORIAL_TEXT_AVOID,
	TUTORIAL_TEXT_MOVE,
	TUTORIAL_TEXT_ENEMY,
	TUTORIAL_TEXT_ATTACK,
	TUTORIAL_TEXT_ATTACK2,
	TUTORIAL_TEXT_STRONG_ATTACK,
	TUTORIAL_TEXT_BOOST_GO,
	TUTORIAL_TEXT_BOOST_ATTACK,
	TUTORIAL_TEXT_END,
};

CPlayScene::CPlayScene()
{

}
CPlayScene::~CPlayScene()
{
	m_textInfo.~CText();
}

//プレイ初期化
void CPlayScene::Init()
{
	//Drawマネージャー生成
	CDrawManager::CreateManager()->Init();

	//キャラマネージャー生成
	CCharacterManager::CreateManager()->Init();
	CPlayerManager::CreateManager()->Init();
	CEnemyManager::CreateManager()->Init();
	//プレイヤー生成
	CPlayerManager::CreatePlayer();


	//カメラマネージャー生成
	CCameraManager::CreateManager()->Init();

	//フィールド
	CFieldManager::CreateManager()->Init();


	//スカイマネージャー生成
	CSkyManager::CreateManager()->Init();

	//魔法マネージャー
	CMagicManager::CreateManager()->Init();

	//ビームマネージャー生成
	CBeamManager::CreateManager()->Init();

	//スコア
	CScore::Init();

	////セーブデータ
	//CSaveData::Init();

	//当たり判定管理クラス
	CCollisionManager::CreateManager()->Init();

	CSceneManager::GetManager()->SetGameTime(0);



	//ユーザインタフェース
	CUIManager::CreateManager()->CreateUI(UI_KIND_PLAY);
	CUIManager::GetManager()->Init();


	//エフェクト
	CEffekseerCtrl::Init(100, 5000);


	//フラグセット
	CSceneManager* scene_manager = CSceneManager::GetManager();

	scene_manager->SetClearFlg(false);
	scene_manager->SetFinFlg(false);

	m_startcount = 0;
	m_finCount = 0;
	m_textCount = 0;
	m_isStart = false;
	m_isText = false;
	m_isTutorial = false;
	m_finCountFlg = false;

	m_fontHandle = CreateFontToHandle(nullptr, 40, -1, DX_FONTTYPE_ANTIALIASING_EDGE);


	if (scene_manager->GetIsTutorial())
	{
		m_tutorialPhase = TUTORIAL_START;
		m_tutorialState = TUTORIAL_STATE_STOP;
		m_isTutorial = true;
		m_stopCount = 1000;
		m_isCurrentTutorialEnd = false;
		m_playCount = 1000;
		memset(m_checkTutorial, false, sizeof(m_checkTutorial));
	}

	if (CSceneManager::GetManager()->GetCuurentStage() == STAGE_KIND_NONE) { CSceneManager::GetManager()->SetCurrentStage(STAGE_KIND_1); }
}

//プレイロード
void CPlayScene::Load()
{
	//スカイロード
	CSkyManager::GetManager()->Load();
	CFieldManager::GetManager()->Load();

	//プレイヤーロード
	CPlayerManager::GetManager()->Load();

	//敵ロード
	CEnemyManager::GetManager()->Load();

	//サウンドロード

	//魔法ロード
	CMagicManager::GetManager()->Load();

	//ビームロード
	CBeamManager::CreateManager()->Load();
	

	//カメラロード
	CCameraManager::GetManager()->Load();

	//スコアオード
	//CScore::Load();


	CUIManager::GetManager()->Load();

	//フェーズをループに
	ChangePhase(PLAY_PHASE_START);

	m_textInfo.Load();
	m_TextBackHandle = LoadGraph("Data/Graph/Text/BoxBlue.png");
	CSceneManager::GetManager()->SetNextSceneID(SCENE_ID_CLEAR);

	SetMouseDispFlag(false);

	//========================================
	CSoundManager::LoadSE(SE_KIND_ATTACK1);
	CSoundManager::LoadSE(SE_KIND_ATTACK2);
	CSoundManager::LoadSE(SE_KIND_ATTACK3);
	CSoundManager::LoadSE(SE_KIND_ATTACK4);
	CSoundManager::LoadSE(SE_KIND_BEAM1);
	CSoundManager::LoadSE(SE_KIND_DAMAGE1);
	CSoundManager::LoadSE(SE_KIND_DAMAGE2);
	CSoundManager::LoadSE(SE_KIND_HIT1);
	CSoundManager::LoadSE(SE_KIND_HIT2);
	CSoundManager::LoadSE(SE_KIND_HIT3);
	CSoundManager::LoadSE(SE_KIND_THUNDER);
	CSoundManager::LoadSE(SE_KIND_KO);
	CSoundManager::LoadSE(SE_KIND_SELECT1);
	CSoundManager::LoadSE(SE_KIND_SELECT2);
	CSoundManager::LoadSE(SE_KIND_SHOOT1);
	CSoundManager::LoadSE(SE_KIND_SWORD1);
	CSoundManager::LoadSE(SE_KIND_SWORD_SUPER);
	CSoundManager::LoadSE(SE_KIND_WARNING);

	CSoundManager::LoadBGM(BGM_KIND_PLAY);
	CSoundManager::LoadBGM(BGM_KIND_BOSS);

	CSoundManager::PlayBGM(BGM_KIND_PLAY);

	//========================================

	//========================================
	CSceneManager* scene_manager = CSceneManager::GetManager();
	if (scene_manager->GetIsTutorial())
	{
		GoLoop();
	}
	//========================================
}

//プレイ通常処理
void CPlayScene::Step()
{
	if (m_phase != PLAY_PHASE_PAUSE)
	{
		SetMousePoint(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	}

	//ポーズ画面
	if (CInputManager::IsPush(KEY_INPUT_ESCAPE, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_START, I_PAD))
	{
		ChangePhase(PLAY_PHASE_PAUSE);
	}

	if (m_isTutorial)
	{
		if (m_phase == PLAY_PHASE_PAUSE)
			StepPause();
		else if (m_phase == PLAY_PHASE_FIN)
			StepFin();
		else
		{
			StepTutorial();
		}
		return;
	}


	//現在のフェーズのステップへ
	(this->*PlayPhaseFunc[m_phase])();


	CUIManager::GetManager()->Step();

	CAnimeInfo::AllStopAnimeStep();

	m_textInfo.Update();

#ifdef DEBUG_MODE


#endif
}
void CPlayScene::StepTutorial()
{
	switch (m_tutorialState)
	{
	case TUTORIAL_STATE_STOP:

		//一定時間動けない
		m_stopCount -= FRAME_TIME;

		if (m_stopCount <= 0)
		{
			m_textInfo.Request(TUTORIAL_TEXT[m_tutorialPhase][m_textCount++], 5);
			m_isText = true;
			m_tutorialState = TUTORIAL_STATE_TEXT;
			m_stopCount = 500;
		}

		CPlayerManager::GetManager()->GetPlayer()->StepStop();
		break;

	case TUTORIAL_STATE_TEXT:

		if (CInputManager::IsPush(MOUSE_INPUT_LEFT, I_MOUSE) || CInputManager::IsPush(KEY_INPUT_RETURN, I_KEY) || 
			CInputManager::IsPush(XINPUT_BUTTON_B, I_PAD))
		{
			if (m_textCount >= TUTORIAL_TEXT_NUM[m_tutorialPhase] &&
				m_textInfo.IsEnd())
			{
				m_isText = false;
				m_textCount = 0;
				m_tutorialState = TUTORIAL_STATE_ACTION;
			}
			else
			{
				if (m_textInfo.IsEnd())
				{
					m_textInfo.Request(TUTORIAL_TEXT[m_tutorialPhase][m_textCount++], 5);
				}
				else
				{
					m_textInfo.SetAllText();
				}
			}

		}

		break;

	case TUTORIAL_STATE_ACTION:
		CCameraManager::GetManager()->Step();
		CPlayerManager::GetManager()->GetPlayer()->StepTutorial();

		if(!m_isCurrentTutorialEnd)
			if (CheckTutorial())
				m_isCurrentTutorialEnd = true;

			

		CEnemyManager::GetManager()->Step();
		CGameManager::StepAllManagerN(CManagerBase::MG_KIND_CHARACTER, CManagerBase::MG_KIND_COLLISION);
		CCollisionManager::GetManager()->CheckTutorialCollision(static_cast<TutorialPhase>(m_tutorialPhase));
		CUIManager::GetManager()->Step();

		//	遊びを持たせる
		if (m_isCurrentTutorialEnd)
		{
			m_playCount -= FRAME_TIME;
			
			if (m_playCount <= 0)
			{
				m_isCurrentTutorialEnd = false;

				if (m_tutorialPhase == TUTORIAL_ATTACK2)
				{
					CPlayerManager::GetManager()->GetPlayer()->HealingMP(500.f);
				}
				else if (m_tutorialPhase == TUTORIAL_STRONG_ATTACK)
				{
					CPlayerManager::GetManager()->GetPlayer()->ChargeBoost(500.f);
				}
				

				m_tutorialPhase++;
				m_playCount = 2000;
				m_tutorialState = TUTORIAL_STATE_STOP;

				//終了
				if (m_tutorialPhase == TUTORIAL_NUM)
				{
					CSceneManager::GetManager()->SetNextSceneID(SCENE_ID_SELECT);
					//フェーズを終了に
					ChangePhase(PLAY_PHASE_FIN);
				}
			}
		}
		//	Tキー、または十字上キーでもう一度読む
		else if (CInputManager::IsPush(KEY_INPUT_T, I_KEY) ||
			CInputManager::IsPush(XINPUT_BUTTON_DPAD_UP, I_PAD))
		{
			m_textInfo.Request(TUTORIAL_TEXT[m_tutorialPhase][m_textCount++], 5);
			m_isText = true;
			m_tutorialState = TUTORIAL_STATE_TEXT;
		}
	}

	CAnimeInfo::AllStopAnimeStep();
	m_textInfo.Update();

}

bool CPlayScene::CheckTutorial()
{
	switch (m_tutorialPhase)
	{
		//	移動
	case TUTORIAL_START:
		if (CPlayerManager::GetManager()->GetPlayer()->IsTutorialMove())
		{
			return true;
		}

		break;

	case TUTORIAL_AVOID:
		if(CPlayerManager::GetManager()->GetPlayer()->IsTutorialAvoid())
		{
			return true;
		}

		break;

	case TUTORIAL_MOVE:
		return true;
		break;
		

	case TUTORIAL_ENEMY:
		//	敵の生成が終わったらTrue
		VECTOR start_pos = CPlayerManager::GetManager()
			->
			GetPlayer()
			->
			GetPos();
		start_pos = CMyMath::VecAdd(start_pos, VGet(170, 0, 100));
		CEnemyManager::GetManager()
			->
			AppearEnemy(ENEMY_KIND_NORMAL, start_pos);
		return true; break;

	case TUTORIAL_ATTACK:
		if (!m_checkTutorial[0])
			m_checkTutorial[0] = 
			CCollisionManager::GetManager()
			->
			IsCheckPlayerAttackHitEnemy();

		if (!m_checkTutorial[1])
			m_checkTutorial[1] = 
			CPlayerManager::GetManager()
			->
			GetPlayer()
			->
			IsTutorialNormalAttack();

		if (m_checkTutorial[0] && m_checkTutorial[1])
		{
			memset(m_checkTutorial, false, sizeof(m_checkTutorial));

			return true;
		}

		break;

	case TUTORIAL_ATTACK2:
		if (!m_checkTutorial[0])
			m_checkTutorial[0] =
			CCollisionManager::GetManager()
			->
			IsCheckPlayerAttackHitEnemy();

		if (!m_checkTutorial[1])
			m_checkTutorial[1] =
			CPlayerManager::GetManager()
			->
			GetPlayer()
			->
			IsTutorialNormalAttack2();

		if (m_checkTutorial[0] && m_checkTutorial[1])
		{
			memset(m_checkTutorial, false, sizeof(m_checkTutorial));

			return true;
		}

		break;

	case TUTORIAL_STRONG_ATTACK:

		if (!m_checkTutorial[0])
			m_checkTutorial[0] =
			CPlayerManager::GetManager()
			->
			GetPlayer()
			->
			IsTutorialStrongAttack();

		if (m_checkTutorial[0])
		{
			memset(m_checkTutorial, false, sizeof(m_checkTutorial));

			return true;
		}

		break;

	case TUTORIAL_BOOST_GO:
		if (CPlayerManager::GetManager()->GetPlayer()->IsTutorialBoostGo())
			return true;

		break;

	case TUTORIAL_BOOST_ATTACK:
		if (CPlayerManager::GetManager()->GetPlayer()->IsTutorialBoostAttack())
			return true;

		break;

	case TUTORIAL_END:

		return true;

		break;
	}

	return false;
}

void CPlayScene::StepText()
{
	if (m_textCount == -1) { return; }

	
}


//プレイ描画処理
void CPlayScene::Draw()
{
	if (m_phase != PLAY_PHASE_PAUSE || 
		m_phase != PLAY_PHASE_FIN)
	{
		//最強
		CEffekseerCtrl::UpdateAutoCamera();
	}

	//すべてのオブジェクト描画
	CDrawManager::DrawAllObject();


	//effectDraw
	CMagicManager::GetManager()->Update();
	CBeamManager::GetManager()->Update();
	CEffekseerCtrl::Draw();

	//現在のフェーズのdrawへ
	(this->*DrawPhaseFunc[m_phase])();


	//最後にテキスト描画
	if (m_phase != PLAY_PHASE_PAUSE)
	{
		if (m_isTutorial)
		{
			if (m_tutorialState == TUTORIAL_STATE_STOP ||
				m_tutorialState == TUTORIAL_STATE_TEXT)
			{
				//	画面を少し暗くする
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
				DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(0, 0, 0), true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			DrawTextData();
		}
	}

	//	チュートリアル中の描画
	if (m_isTutorial)
	{
		switch (m_tutorialState)
		{
		case TUTORIAL_STATE_STOP:
		{

		}
			break;
		case TUTORIAL_STATE_TEXT:
		{
			//	Enter、左クリ、Bボタンで全文描画アナウンス
			DrawExtendFormatStringF(WINDOW_WIDTH - 300, 430, 1.f, 1.f, GetColor(255, 255, 255), "Enter、左クリ、Bボタンで全文描画");
		}
			break;
		case TUTORIAL_STATE_ACTION:
		{
			if (!m_isCurrentTutorialEnd)
			{
				//	Tキー、十字上でもう一度読む
				DrawFormatStringToHandle(200, 30, GetColor(255, 100, 100), m_fontHandle, "Tキー、十字上ボタンでもう一度読む");
			}
		}
			break;
		}
	}
}

void CPlayScene::DrawTextData()
{
	if (!m_isText) {
		return;
	}

	DrawExtendGraph(0, 450, WINDOW_WIDTH, WINDOW_HEIGHT, m_TextBackHandle, true);
	m_textInfo.Draw();
}

void CPlayScene::StepStart()
{
	CPlayerManager::GetManager()->GetPlayer()->Step();

	CCameraManager::GetManager()->Step();

	if(CUIManager::GetManager()->GetUI()->StepStartPhase())
		m_isStart = true;

	if(m_isStart)
		GoLoop();
}

void CPlayScene::GoLoop()
{
	//フェーズをループに
	ChangePhase(PLAY_PHASE_LOOP);

	int stage_num = static_cast<int>(CSceneManager::GetManager()->GetCuurentStage());

	CEnemyManager::GetManager()->StartAppear(stage_num);

	//カウントを戻す
	m_startcount = 0;
}

void CPlayScene::StepLoop()
{


	//シーン以外のすべてのマネージャーのステップを回す
	CGameManager::StepAllManager();


	//プレイヤーがしんだらStepFinへ
	if (CPlayerManager::GetManager()->GetPlayer()->GetHP() <= 0)
	{
		//フェーズを終了に
		ChangePhase(PLAY_PHASE_FIN);
		CSceneManager::GetManager()->SetNextSceneID(SCENE_ID_GAMEOVER);
	}

	//	ボスがすべて死んだらFinへ
	if (CEnemyManager::GetManager()->IsDownBoss())
	{
		//フェーズを終了に
		ChangePhase(PLAY_PHASE_FIN);
		CSceneManager::GetManager()->SetNextSceneID(SCENE_ID_CLEAR);
		CEnemyManager::GetManager()->KillAllEnemy();
		CSoundManager::StopBGM(BGM_KIND_PLAY);
		CSoundManager::StopBGM(BGM_KIND_BOSS);
		CSoundManager::PlaySE(SE_KIND_KO);
	}

	//if (CInputManager::IsPush(KEY_INPUT_RETURN, I_KEY))
	//{
	//	//フェーズを終了に
	//	ChangePhase(PLAY_PHASE_FIN);
	//	CSceneManager::GetManager()->SetNextSceneID(SCENE_ID_CLEAR);
	//	CEnemyManager::GetManager()->KillAllEnemy();
	//	CSoundManager::StopBGM(BGM_KIND_PLAY);
	//	CSoundManager::StopBGM(BGM_KIND_BOSS);
	//	CSoundManager::PlaySE(SE_KIND_KO);
	//}

	//========================================================================================
	CSceneManager::GetManager()->StepGameTime();
	//========================================================================================

}

void CPlayScene::StepFin()
{
	//	ゲームオーバーなら
	if (CSceneManager::GetManager()->GetNextSceneID() == SCENE_ID_GAMEOVER)
	{
		CPlayerManager::GetManager()->GetPlayer()->StepFinPhase();
		CSkyManager::GetManager()->Step();
		//	最強
		CEffekseerCtrl::UpdateAutoCamera();

		m_startcount += FRAME_TIME;

		if (m_startcount > 2000)
		{
			//	後処理へ
			CSceneManager::GetManager()->SetFuncID(SCENE_FUNC_ID_FIN);
		}
	}
	//	クリアなら
	else
	{
		if (!m_finCountFlg)
		{
			m_finCount += FRAME_TIME;

			if (m_finCount >= 120)
			{
				CPlayerManager::GetManager()->GetPlayer()->StepFinPhase();
				CSkyManager::GetManager()->Step();
				CEnemyManager::GetManager()->Step();
				CCameraManager::GetManager()->GetPlayCamera()->StepFin();

				//最強
				CEffekseerCtrl::UpdateAutoCamera();

				m_finCount = 0;

				m_startcount++;

				if (m_startcount >= 10)
				{
					m_finCountFlg = true;
					m_startcount = 0;
				}
			}
		}
		else
		{
			m_startcount += FRAME_TIME;

			CPlayerManager::GetManager()->GetPlayer()->StepFinPhase();
			CSkyManager::GetManager()->Step();
			CEnemyManager::GetManager()->Step();
			CCameraManager::GetManager()->GetPlayCamera()->StepFin();

			if (m_startcount >= 2000)
			{
				//後処理へ
				CSceneManager::GetManager()->SetFuncID(SCENE_FUNC_ID_FIN);
			}
		}

	}

}

void CPlayScene::StepPause()
{
	////かり
	//if (CInputManager::IsPush(KEY_INPUT_1, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_START, I_PAD))
	//{
	//	//記憶していたフェーズにする
	//	PlayPhase m = m_phase;
	//	m_phase = m_oldPhase;
	//	m_oldPhase = m;
	//
	//}

	int next = CUIManager::GetManager()->GetUI()->StepPause();

	if(next == 1)
	{
		//記憶していたフェーズにする
		PlayPhase m = m_phase;
		m_phase = m_oldPhase;
		m_oldPhase = m;

		SetMouseDispFlag(false);
	}
	if (next == 2)
	{
		//フェーズを終了に
		ChangePhase(PLAY_PHASE_FIN);
		SetMouseDispFlag(false);
	}
}

void CPlayScene::DrawStart()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 90);
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(255, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	CUIManager::GetManager()->GetUI()->DrawStartPhase();

	
	/*CScore::Update();
	CScore::DrawScorePlay();*/
}


void CPlayScene::DrawLoop()
{
#if 0

	CPlayerManager* player_manager = CPlayerManager::GetManager();
	CPlayer* player = player_manager->GetPlayer();
	player->Draw();

	CFieldManager* field_manager = CFieldManager::GetManager();
	field_manager->Draw();

	CSkyManager* sky_manager = CSkyManager::GetManager();
	sky_manager->Draw();

#endif
	////プレイヤー座標描画
	//VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	//DrawFormatString(10, 10, GetColor(255, 80, 80), "プレイヤー座標[ %.1f, %.1f, %.1f ]", player_pos.x, player_pos.y, player_pos.z);


#if 0

	//プレイヤー座標描画
	VECTOR player_pos = CPlayerManager::GetManager()->GetPlayer()->GetPos();
	DrawFormatString(10, 10, GetColor(255, 80, 80), "プレイヤー座標[ %.1f, %.1f, %.1f ]", player_pos.x, player_pos.y, player_pos.z);

	//プレイヤーHP描画
	int player_hp = CPlayerManager::GetManager()->GetPlayer()->GetHP();
	DrawFormatString(10, 30, GetColor(10, 10, 10), "プレイヤーHP[ %d ]", player_hp);

	//プレイヤーの状態
	char a[PLAYER_STATE_NUM][256] =
	{
		"たいきしてるぜ",
		"あるいているだと",
		"ラーン！！！！",
		"回避していくぅぅぅ",
		"攻撃！！",
		"うわっっっ",
		"のっくばっくなう",
		"おきています"
		"ざこだうん"
	};
	DrawExtendFormatString(900, 10, 1.0, 1.0, GetColor(255, 20, 20), "プレイヤーの状態：%s", a[CPlayerManager::GetManager()->GetPlayer()->GetState()]);

	DrawExtendFormatString(10, 70, 1.0, 1.0, GetColor(255, 20, 20), "!!コントローラー推奨です");
	DrawExtendFormatString(10, 100, 0.7, 1.0, GetColor(20, 20, 20), "ZキーorXボタン　　　　　：　早い通常攻撃（３段階）");
	DrawExtendFormatString(10, 120, 0.7, 1.0, GetColor(20, 20, 20), "CキーorBボタン　　　　　：　遅い通常攻撃（３段階）");
	DrawExtendFormatString(10, 140, 0.7, 1.0, GetColor(20, 20, 20), "十字キーor左スティック　：　移動");
	DrawExtendFormatString(10, 160, 0.7, 1.0, GetColor(20, 20, 20), "左シフトorAボタン　　　 ：　回避");
	DrawExtendFormatString(10, 180, 0.7, 1.0, GetColor(20, 20, 20), "XキーorYボタン　　　　　：　強攻撃（今はMPの回復が早いので多用できてしまう）");

	DrawExtendFormatString(10, 220, 1.0, 1.0, GetColor(255, 20, 20), "パッド専用行動！！！！");
	DrawExtendFormatString(10, 260, 0.7, 1.0, GetColor(20, 20, 20), "LBを押したあと右ステックで方向を決め、RBを押すとカッコいい直線攻撃ができる");
	DrawExtendFormatString(10, 280, 0.7, 1.0, GetColor(20, 20, 20), "右ステックでカメラ移動");

	DrawExtendFormatString(10, 320, 1.0, 1.0, GetColor(10, 10, 10), "memo:バリアは強攻撃１発で壊せます");





#endif

	//	ボス敵倒せ描画
	CEnemy** boss_p = CEnemyManager::GetManager()->GetBossEnemyPointer();
	if (boss_p[0] != nullptr)
	{
		StageKind stage = CSceneManager::GetManager()->GetCuurentStage();
		if (stage == STAGE_KIND_1)
		{
			//	Tキー、十字上でもう一度読む
			DrawFormatStringToHandle(800, 650, GetColor(255, 100, 100), m_fontHandle, "紫ゴーレム を倒せ");
		}
		else if (stage == STAGE_KIND_2)
		{
			DrawFormatStringToHandle(900, 600, GetColor(255, 100, 100), m_fontHandle, "紫ゴーレム と");
			DrawFormatStringToHandle(780, 655, GetColor(255, 100, 100), m_fontHandle, "水色アシカ を倒せ");
		}
		else if (stage == STAGE_KIND_3)
		{
			DrawFormatStringToHandle(800, 650, GetColor(255, 100, 100), m_fontHandle, "ボスゴーレム を倒せ");
		}
	}

	//========================================================================================
	if (!m_isTutorial) { CSceneManager::GetManager()->DrawGameTime(); }
	//========================================================================================

	CUIManager::GetManager()->Draw();
}
void CPlayScene::DrawFin()
{
	CUIManager::GetManager()->Draw();
}

void CPlayScene::DrawPause()
{
	CUIManager::GetManager()->GetUI()->DrawPause();
}

//プレイループ処理
void CPlayScene::Loop()
{
	Step();
	Draw();
}

//プレイ後処理
void CPlayScene::Fin()
{
	CEffekseerCtrl::Exit();

	//Delete処理

	//BEAMマネージャー削除
	CBeamManager::GetManager()->DeleteManager();

	////魔法マネージャー削除
	CMagicManager::GetManager()->DeleteManager();

	//Drawマネージャー削除
	CDrawManager::GetManager()->DeleteManager();

	//キャラマネージャー削除
	CCharacterManager::GetManager()->DeleteAllManager();

	//カメラマネージャー削除
	CCameraManager::GetManager()->DeleteManager();

	//スカイマネージャー削除
	CSkyManager::GetManager()->DeleteManager();

	//フィールドマネージャー削除
	CFieldManager::GetManager()->DeleteManager();

	//当たり判定管理クラス削除
	CCollisionManager::GetManager()->DeleteManager();

	//サウンド
	CSoundManager::Fin();

	DeleteGraph(m_TextBackHandle);

	DeleteFontToHandle(m_fontHandle);

	//ユーザインタフェース
	CUIManager::GetManager()->DeleteManager();

	//CGameManager::DeleteAllList();

	m_textInfo.~CText();
	
	CSceneManager::GetManager()->ChangeScene(CSceneManager::GetManager()->GetNextSceneID());
	
	CSceneManager::GetManager()->SetIsTutorial(false);

	SetMouseDispFlag(true);
}


void CPlayScene::JudgementClear()
{
	//ゲーム終了する
	CSceneManager::GetManager()->SetFinFlg(true);

	//クリアフラグをtrueに
	CSceneManager::GetManager()->SetClearFlg(true);
}

void CPlayScene::ChangePhase(PlayPhase phase)
{
	if (phase == m_phase) { return; }

	if (phase == PLAY_PHASE_PAUSE)
	{
		SetMouseDispFlag(true);
	}

	//今のフェーズを記憶する
	m_oldPhase = m_phase;

	//セット
	m_phase = phase;
}