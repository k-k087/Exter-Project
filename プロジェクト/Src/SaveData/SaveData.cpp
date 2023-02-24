#include "DxLib.h"
#include "SaveData.h"
#include "string.h"

#include "../Common.h"

#include "../Input/InputManager.h"
#include "../Scene/SceneManager.h"

#include "../Score/Score.h"

CSaveData* CSaveData::m_saveData = nullptr;

static char* SAVE_DATA[STAGE_KIND_NUM] =
{
	{ " " },
	{ "Data/SaveData/SaveData1.txt" },
	{ "Data/SaveData/SaveData2.txt" },
	{ "Data/SaveData/SaveData3.txt" },
	{ "Data/SaveData/SaveDataEX.txt" },
};

static const char* TIME_RANKING_BUF[STAGE_KIND_NUM] =
{
	" ",
	"Stage1 のタイムランキング",
	"Stage2 のタイムランキング",
	"Stage3 のタイムランキング",
	"StageEX のタイムランキング",
};


void CSaveData::Init()
{
	m_digitIndex = 0;
	m_endSaveFlg = false;
	m_fileP = nullptr;
	m_newScore = 0;
	m_startSaveFlg = false;
	m_finishSelectNameFlg = false;
	m_newName[0] = 'A';
	m_newName[1] = 'A';
	m_newName[2] = 'A';


	SaveInfo* save_info = m_saveInfo;
	for (int save_index = 0; save_index < MAX_RANK; save_index++, save_info++)
	{
		for (int name_index = 0; name_index < MAX_NAME; name_index++)
		{
			save_info->saveName[name_index] = { '=' };
		}
		save_info->saveScore = -1;
	}

	m_fontHandle = CreateFontToHandle(nullptr, 80, -1, DX_FONTTYPE_ANTIALIASING_EDGE);
}

//スコアを書き込む関数
bool CSaveData::WriteData()
{
	if (m_endSaveFlg == false)
	{
		StageKind stage_kind = CSceneManager::GetManager()->GetCuurentStage();

		//最初にデータを読み込む
		ReadData();
		fopen_s(&m_fileP, SAVE_DATA[stage_kind], "w");

		//プレイ後のスコアを入れる
		m_newScore = (CSceneManager::GetManager()->GetGameTime() / 1000);

		//名前を決める
		SelectName();

		if (m_finishSelectNameFlg == true)
		{
			//ランキング
			SaveInfo* save_info = m_saveInfo;
			for (int data_index = 0; data_index < MAX_RANK; data_index++,
				save_info++)
			{
				//何も入っていないなら
				if (save_info->saveScore < 0)
				{
					//スコアを入れる
					save_info->saveScore = m_newScore;

					//名前を入れる
					/*strcpy_s(save_info->saveName, MAX_NAME + 1, m_newName);*/
					for (int index = 0; index < MAX_NAME; index++)
					{
						save_info->saveName[index] = m_newName[index];
					}


					//入力が終わったら初期化しておく
					for (int index = 0; index < MAX_NAME; index++)
						m_newName[index] = { 'A' };
					m_digitIndex = 0;
					break;
				}

				//もし先に入っているスコアより低いスコアなら
				if (save_info->saveScore >= m_newScore)
				{
					SaveInfo* change_save = &m_saveInfo[MAX_RANK - 1];
					for (int change_index = MAX_RANK - 1; change_index > data_index; change_index--, change_save--)
					{
						SaveInfo* m = change_save - 1;

						//スコアをずらす
						change_save->saveScore = m->saveScore;

						//名前をずらす
						strcpy_s(change_save->saveName, MAX_NAME + 1, m->saveName);

					}

					//そのスコアを入れる
					save_info->saveScore = m_newScore;

					//名前を入れる
					for (int index = 0; index < MAX_NAME; index++)
					{
						save_info->saveName[index] = m_newName[index];
					}

					//入力が終わったら初期化しておく
					for (int index = 0; index < MAX_NAME; index++)
						m_newName[index] = { 'A' };
					m_digitIndex = 0;
					break;
				}
			}

			save_info = m_saveInfo;
			//データに書き込む
			for (int data_index = 0; data_index < MAX_RANK; data_index++, save_info++)
				fprintf(m_fileP, "%s %d \n", save_info->saveName, save_info->saveScore);

			m_startSaveFlg = false;
			m_endSaveFlg = true;
		}
		fclose(m_fileP);
	}

	return m_finishSelectNameFlg;
}


//データを読む
void CSaveData::ReadData()
{
	StageKind stage_kind = CSceneManager::GetManager()->GetCuurentStage();

	//最初にデータを読み込む
	fopen_s(&m_fileP, SAVE_DATA[stage_kind], "r");

	for (int data_index = 0; data_index < MAX_RANK; data_index++)
	{
		//データを読み込む
		fscanf_s(m_fileP, "%s %d\n", m_saveInfo[data_index].saveName, MAX_NAME + 1, &m_saveInfo[data_index].saveScore);
	}
	fclose(m_fileP);
}

//データ読み込んで表示する
void CSaveData::DrawData()
{
	StageKind stage_kind = CSceneManager::GetManager()->GetCuurentStage();

	//最初にデータを読み込む
	fopen_s(&m_fileP, SAVE_DATA[stage_kind], "r");

	int color_red[MAX_RANK] = { 255, 192, 196, 50, 50 };
	int color_green[MAX_RANK] = { 215, 192, 112, 50, 50 };
	int color_blue[MAX_RANK] = { 0, 192, 34, 50, 50 };

	DrawFormatStringToHandle(100, 60, GetColor(200, 100, 100), m_fontHandle, TIME_RANKING_BUF[stage_kind]);

	SaveInfo* save_info = m_saveInfo;
	for (int data_index = 0; data_index < MAX_RANK; data_index++, save_info++)
	{
		//データを読み込む
		fscanf_s(m_fileP, "%s %d\n", save_info->saveName, MAX_NAME + 1, &save_info->saveScore);

		DrawFormatStringToHandle(150, 195 + 105 * data_index,
			GetColor(color_red[data_index], color_green[data_index], color_blue[data_index]),
			m_fontHandle, "%d位", data_index + 1);

		//データが入っていたらそれを表示する
		if (save_info->saveScore >= 0)
		{
			int minutes = save_info->saveScore / 60;
			int seconds = save_info->saveScore % 60;

			DrawFormatStringToHandle(550, 195 + 105 * data_index,
				GetColor(0, 0, 0), m_fontHandle, "  %02d分 %02d秒", minutes, seconds);

			DrawFormatStringToHandle(350, 195 + 105 * data_index,
				GetColor(0, 0, 0), m_fontHandle, "%s", save_info->saveName);
		}
		//もし何も入っていないなら0を表示する
		else
		{
			DrawFormatStringToHandle(550, 195 + 105 * data_index,
				GetColor(0, 0, 0), m_fontHandle, "  ---------");

			DrawFormatStringToHandle(350, 195 + 105 * data_index,
				GetColor(0, 0, 0), m_fontHandle, "NONAME");
		}
	}
	fclose(m_fileP);
}


//名前を決める関数
void CSaveData::SelectName()
{
	if (m_finishSelectNameFlg == false)
	{
		//名前の桁ごとに表示
		for (int index = 0; index < MAX_NAME; index++)
		{
			DrawFormatStringToHandle(800 + (150 * index), 600, GetColor(255, 0, 0), m_fontHandle, "%c",
				m_newName[index]);
		}

		//今どこを入力しているか示す三角
		DrawFormatStringToHandle(777 + m_digitIndex * 150, 500,  GetColor(255, 0, 255), m_fontHandle, "▽");
	}

	//文字コードで処理している
	if (m_newName[m_digitIndex] >= 65 && m_newName[m_digitIndex] <= 90)
	{
		//上押すとアルファベットが下がっていく
		if (CInputManager::IsPush(KEY_INPUT_UP, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_UP, I_PAD))
		{
			m_newName[m_digitIndex]--;

			//Aの状態で上押すとZになる
			if (m_newName[m_digitIndex] < 65)
				m_newName[m_digitIndex] = 'Z';
		}

		//下を押すとアルファベットが上がっていく
		if (CInputManager::IsPush(KEY_INPUT_DOWN, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_DOWN, I_PAD))
		{
			m_newName[m_digitIndex]++;

			//Zの状態で下を押すとAになる
			if (m_newName[m_digitIndex] > 90)
				m_newName[m_digitIndex] = 'A';
		}

		//右を押すと次の桁へ
		if (CInputManager::IsPush(KEY_INPUT_RIGHT, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_RIGHT, I_PAD))
		{
			m_digitIndex++;

			if (m_digitIndex > 2)
				m_digitIndex = 0;
		}

		//左を押すと前の桁へ
		if (CInputManager::IsPush(KEY_INPUT_LEFT, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_LEFT, I_PAD))
		{
			m_digitIndex--;
			if (m_digitIndex < 0)
				m_digitIndex = 2;
		}

		if (CInputManager::IsPush(KEY_INPUT_RETURN, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_B, I_PAD))
		{
			m_finishSelectNameFlg = true;
		}
	}
}

void CSaveData::Fin()
{
	DeleteFontToHandle(m_fontHandle);
}

CSaveData* CSaveData::GetSaveData()
{
	return m_saveData;
}
CSaveData* CSaveData::CreateSaveData()
{
	if (!m_saveData)
	{
		m_saveData = _NEW(CSaveData);
	}

	return m_saveData;
}
void CSaveData::DeleteSaveData()
{
	if (m_saveData)
	{
		this->Fin();

		_DELETE(m_saveData);

		m_saveData = nullptr;
	}
}
