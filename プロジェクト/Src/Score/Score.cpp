#include "Score.h"
#include "DxLib.h"
#include "../Input/InputManager.h"

#include "../Scene/SceneManager.h"

int CScore::m_score;
int CScore::m_memoryScore;
int CScore::m_handle[NUMBER_NUM];
int CScore::m_disp_score;

const int ADD_SCORE_SPD = 50;	//スコア加算桁数

const int DIGIT_NUMBER = 6;	//表示桁数

//プレイ中描画開始位置
const int DRAW_START_X = 1250;	
const int DRAW_START_Y = 20;

//	クリア中描画開始位置
const int CLEAR_DRAW_START_X = 700;
const int CLEAR_DRAW_START_Y = 150;

//コンストラクタ
CScore::CScore()
{

}
//デストラクタ
CScore::~CScore()
{

}

//	初期化
void CScore::Init() 
{
	m_score = 0;
	m_memoryScore = 0;
	m_disp_score = 0;
	for (int score_index = 0; score_index < NUMBER_NUM; ++score_index)
		m_handle[score_index] = -1;
}

//	画像読みこみ
void CScore::Load()
{
	LoadDivGraph("Data/Graph/Score/score.png", NUMBER_NUM, 11, 1, NUMBER_SIZE_X, NUMBER_SIZE_Y, m_handle);
}

//更新処理
void CScore::Update()
{
	/*	スコアを描画用に変換	*/

	if (m_disp_score < m_score)
	{
		m_disp_score += ADD_SCORE_SPD;

		if (m_disp_score > m_score)
			m_disp_score = m_score;
	}
}

//リザルトシーンスコア更新処理
bool CScore::ResultUpdate()
{
	bool is_end = false;

	if (m_memoryScore == 0)
	{
		//スコアを保存用変数に代入
		m_memoryScore = m_score;
		//m_scoreを一度リセット
		CScore::ResetScore();
	}

	//エンタキー押したら実際のスコア表示
	if (CInputManager::IsPush(KEY_INPUT_RETURN, I_KEY))
	{
		m_score = m_memoryScore;
		is_end = true;
	}

	//実際のスコアを超えるまでm_scoreを加算しなおす
	if (m_score < m_memoryScore)
	{
		CScore::AddScore(10);

		if (m_score >= m_memoryScore)
		{
			CScore::SetScore(m_score);
			is_end = true;
		}

	}
	//超えたらスコアを実際のスコアの値に直してセット
	else
	{
		CScore::SetScore(m_score);
		is_end = true;
	}

	return is_end;
}

//後処理
void CScore::Fin()
{
	for (int index = 0; index < NUMBER_NUM; index++)
		DeleteGraph(m_handle[index]);
}

//	スコア加算
void CScore::AddScore(const int score)
{
	m_score += score;
}

//	プレイ中のスコア表示
void CScore::DrawScorePlay()
{
	//描画用スコアを一時的に格納
	int temp_score = m_disp_score;

	for (int index = 0; index < DIGIT_NUMBER; index++)
	{
		//10で割った余りを格納
		int num = temp_score % 10;

		//余りの数字の画像を描画
		DrawRotaGraph(DRAW_START_X - (NUMBER_SIZE_X * index)-20, DRAW_START_Y,
			1.0, 0.0, m_handle[num], true);

		//桁を減らす
		temp_score /= 10;
	}

	DrawRotaGraph(DRAW_START_X, DRAW_START_Y, 1.0, 0.0, m_handle[10], true);
}

//	リザルト中のスコア表示
void CScore::DrawScoreResult()
{
	//描画用スコアを一時的に格納
	int temp_score = m_score;

	for (int index = 0; index < DIGIT_NUMBER; index++)
	{
		//10で割った余りを格納
		int num = temp_score % 10;

		//余りの数字の画像を描画
		DrawRotaGraph(CLEAR_DRAW_START_X - (NUMBER_SIZE_X * 2 * index), CLEAR_DRAW_START_Y,
			2.0, 0.0, m_handle[num], true);
		

		//桁を減らす
		temp_score /= 10;
	}

	DrawRotaGraph(CLEAR_DRAW_START_X + 40, CLEAR_DRAW_START_Y, 2.0, 0.0, m_handle[10], true);
}

//スコアセット
void CScore::SetScore(int score)
{
	m_score = score;
}

//スコアリセット
void CScore::ResetScore()
{
	m_score = 0;
}