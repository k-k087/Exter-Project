#pragma once 

const int NUMBER_NUM = 11;	//数字の数

const int NUMBER_SIZE_X = 20;

const int NUMBER_SIZE_Y = 32;


class CScore
{
private:
	static int m_score;	//スコア
	static int m_memoryScore;	//実際のスコア保存用変数
	static int m_handle[NUMBER_NUM];	//数字画像ハンドル
	static int m_disp_score;

public:
	CScore();
	~CScore();

	//	初期化
	static void Init();

	//	画像読みこみ
	static void Load();

	//	更新
	static void Update();

	//	後処理
	static void Fin();

	//	スコア加算
	static void AddScore(const int score);

	//	リザルト時のスコア更新処理
	static bool ResultUpdate();
	
	//	プレイ中のスコア表示
	static void DrawScorePlay();

	//	リザルト中のスコア表示
	static void DrawScoreResult();

	//スコアセット
	static void SetScore(int score);

	static int GetScore() { return m_score; }

	//スコアリセット
	static void ResetScore();
};