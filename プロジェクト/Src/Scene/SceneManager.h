#pragma once

#include "DxLib.h"

#include "Scene.h"

#include "TitleScene/TitleScene.h"
#include "PlayScene/PlayScene.h"
#include "SelectScene/SelectScene.h"

#include "../Common.h"

class CSceneManager
{
public:
	typedef void (CSceneManager::*SMFunc)();
public:

private:
	SceneID m_sceneID;
	SceneID m_nextSceneID;
	SceneFuncID m_funcID;
	CScene* m_scene;
	bool m_clearFlg;
	bool m_finFlg;

	int m_gameTime;
	bool m_isStopGameTime;

	StageKind m_currentStage;

	bool m_isTurorial;
	bool m_isOpenEX;

	void (CSceneManager::* m_fadeStep)();

	static CSceneManager* m_sceneManager;

	CSceneManager():
		m_fadeStep(nullptr),
		m_isStopGameTime(false),
		m_isOpenEX(false),
		m_clearFlg(false),
		m_isTurorial(false),
		m_gameTime(),
		m_finFlg(false),
		m_currentStage(STAGE_KIND_NONE),
		m_nextSceneID(SCENE_ID_NONE),
		m_sceneID(SCENE_ID_NONE),
		m_funcID(SCENE_FUNC_ID_NONE),
		m_scene(nullptr)
	{}

private:
	//現在のシーンの初期化
	void Init();
	//現在のシーンのロード
	void Load();
	//現在のシーンのステップ
	void Loop();
	//現在のシーンの後処理
	void Fin();


	

public:

	void StepFadeOut();

	//=======全てミリ秒==========
	void StopGameTime()
	{
		m_isStopGameTime = true;
	}
	void RestartGameTime()
	{
		m_isStopGameTime = false;
	}
	void StepGameTime();
	void DrawGameTime();
	int GetGameTime()const;
	void SetGameTime(const int time);

	float GetGameTimeT();
	//=========================


	static void CreateManager();
	static CSceneManager* GetManager();
	void DeleteManager();
	
	void CurrrentSceneFunc();

	void (CSceneManager::*SceneFunc[SCENE_FUNC_ID_NUM])() =
	{
		&CSceneManager::Init,
		&CSceneManager::Load,
		&CSceneManager::Loop,
		&CSceneManager::Fin,
	};

	void ChangeScene(SceneID scene_id);

	/*
		Getter&Setter
	*/

	SceneFuncID GetFuncID()const
	{
		return m_funcID;
	}
	void SetFuncID(SceneFuncID current_func_id);


	SceneID GetSceneID()const
	{
		return m_sceneID;
	}
	void SetSceneID(SceneID scene_id)
	{
		m_sceneID = scene_id;
	}


	bool GetClearFlg()const
	{
		return m_clearFlg;
	}
	void SetClearFlg(bool flg)
	{
		m_clearFlg = flg;
	}
	

	bool GetFinFlg()const
	{
		return m_finFlg;
	}
	void SetFinFlg(bool flg)
	{
		m_finFlg = flg;
	}

	bool GetIsTutorial()const
	{
		return m_isTurorial;
	}
	void SetIsTutorial(const bool is_tutorial)
	{
		m_isTurorial = is_tutorial;
	}

	SceneID GetNextSceneID()const 
	{ 
		return m_nextSceneID; 
	}
	void SetNextSceneID(const SceneID id) 
	{ 
		m_nextSceneID = id; 
	}

	StageKind GetCuurentStage()const
	{
		if (m_currentStage != STAGE_KIND_NONE)
			return m_currentStage;
		else
			return STAGE_KIND_1;
	}
	void SetCurrentStage(const StageKind kind)
	{
		m_currentStage = kind;
	}

	bool GetIsOpenEX()const
	{
		return m_isOpenEX;
	}
	void SetIsOpenEX(const bool is_ex)
	{
		m_isOpenEX = is_ex;
	}
};