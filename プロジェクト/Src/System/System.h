#pragma once

class CSystem
{
public:
	static bool m_isEnd;

	static void FinGame();

	//初期化
	static bool Init();

	//メインシステム
	static void SystemMain();

	//共通の初期化
	static void InitSystem();
	//共通のステップ
	static void StepSystem();

	static int AllFin();

	//後処理
	static void Fin();
};