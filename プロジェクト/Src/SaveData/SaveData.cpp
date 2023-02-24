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
	"Stage1 �̃^�C�������L���O",
	"Stage2 �̃^�C�������L���O",
	"Stage3 �̃^�C�������L���O",
	"StageEX �̃^�C�������L���O",
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

//�X�R�A���������ފ֐�
bool CSaveData::WriteData()
{
	if (m_endSaveFlg == false)
	{
		StageKind stage_kind = CSceneManager::GetManager()->GetCuurentStage();

		//�ŏ��Ƀf�[�^��ǂݍ���
		ReadData();
		fopen_s(&m_fileP, SAVE_DATA[stage_kind], "w");

		//�v���C��̃X�R�A������
		m_newScore = (CSceneManager::GetManager()->GetGameTime() / 1000);

		//���O�����߂�
		SelectName();

		if (m_finishSelectNameFlg == true)
		{
			//�����L���O
			SaveInfo* save_info = m_saveInfo;
			for (int data_index = 0; data_index < MAX_RANK; data_index++,
				save_info++)
			{
				//���������Ă��Ȃ��Ȃ�
				if (save_info->saveScore < 0)
				{
					//�X�R�A������
					save_info->saveScore = m_newScore;

					//���O������
					/*strcpy_s(save_info->saveName, MAX_NAME + 1, m_newName);*/
					for (int index = 0; index < MAX_NAME; index++)
					{
						save_info->saveName[index] = m_newName[index];
					}


					//���͂��I������珉�������Ă���
					for (int index = 0; index < MAX_NAME; index++)
						m_newName[index] = { 'A' };
					m_digitIndex = 0;
					break;
				}

				//������ɓ����Ă���X�R�A���Ⴂ�X�R�A�Ȃ�
				if (save_info->saveScore >= m_newScore)
				{
					SaveInfo* change_save = &m_saveInfo[MAX_RANK - 1];
					for (int change_index = MAX_RANK - 1; change_index > data_index; change_index--, change_save--)
					{
						SaveInfo* m = change_save - 1;

						//�X�R�A�����炷
						change_save->saveScore = m->saveScore;

						//���O�����炷
						strcpy_s(change_save->saveName, MAX_NAME + 1, m->saveName);

					}

					//���̃X�R�A������
					save_info->saveScore = m_newScore;

					//���O������
					for (int index = 0; index < MAX_NAME; index++)
					{
						save_info->saveName[index] = m_newName[index];
					}

					//���͂��I������珉�������Ă���
					for (int index = 0; index < MAX_NAME; index++)
						m_newName[index] = { 'A' };
					m_digitIndex = 0;
					break;
				}
			}

			save_info = m_saveInfo;
			//�f�[�^�ɏ�������
			for (int data_index = 0; data_index < MAX_RANK; data_index++, save_info++)
				fprintf(m_fileP, "%s %d \n", save_info->saveName, save_info->saveScore);

			m_startSaveFlg = false;
			m_endSaveFlg = true;
		}
		fclose(m_fileP);
	}

	return m_finishSelectNameFlg;
}


//�f�[�^��ǂ�
void CSaveData::ReadData()
{
	StageKind stage_kind = CSceneManager::GetManager()->GetCuurentStage();

	//�ŏ��Ƀf�[�^��ǂݍ���
	fopen_s(&m_fileP, SAVE_DATA[stage_kind], "r");

	for (int data_index = 0; data_index < MAX_RANK; data_index++)
	{
		//�f�[�^��ǂݍ���
		fscanf_s(m_fileP, "%s %d\n", m_saveInfo[data_index].saveName, MAX_NAME + 1, &m_saveInfo[data_index].saveScore);
	}
	fclose(m_fileP);
}

//�f�[�^�ǂݍ���ŕ\������
void CSaveData::DrawData()
{
	StageKind stage_kind = CSceneManager::GetManager()->GetCuurentStage();

	//�ŏ��Ƀf�[�^��ǂݍ���
	fopen_s(&m_fileP, SAVE_DATA[stage_kind], "r");

	int color_red[MAX_RANK] = { 255, 192, 196, 50, 50 };
	int color_green[MAX_RANK] = { 215, 192, 112, 50, 50 };
	int color_blue[MAX_RANK] = { 0, 192, 34, 50, 50 };

	DrawFormatStringToHandle(100, 60, GetColor(200, 100, 100), m_fontHandle, TIME_RANKING_BUF[stage_kind]);

	SaveInfo* save_info = m_saveInfo;
	for (int data_index = 0; data_index < MAX_RANK; data_index++, save_info++)
	{
		//�f�[�^��ǂݍ���
		fscanf_s(m_fileP, "%s %d\n", save_info->saveName, MAX_NAME + 1, &save_info->saveScore);

		DrawFormatStringToHandle(150, 195 + 105 * data_index,
			GetColor(color_red[data_index], color_green[data_index], color_blue[data_index]),
			m_fontHandle, "%d��", data_index + 1);

		//�f�[�^�������Ă����炻���\������
		if (save_info->saveScore >= 0)
		{
			int minutes = save_info->saveScore / 60;
			int seconds = save_info->saveScore % 60;

			DrawFormatStringToHandle(550, 195 + 105 * data_index,
				GetColor(0, 0, 0), m_fontHandle, "  %02d�� %02d�b", minutes, seconds);

			DrawFormatStringToHandle(350, 195 + 105 * data_index,
				GetColor(0, 0, 0), m_fontHandle, "%s", save_info->saveName);
		}
		//�������������Ă��Ȃ��Ȃ�0��\������
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


//���O�����߂�֐�
void CSaveData::SelectName()
{
	if (m_finishSelectNameFlg == false)
	{
		//���O�̌����Ƃɕ\��
		for (int index = 0; index < MAX_NAME; index++)
		{
			DrawFormatStringToHandle(800 + (150 * index), 600, GetColor(255, 0, 0), m_fontHandle, "%c",
				m_newName[index]);
		}

		//���ǂ�����͂��Ă��邩�����O�p
		DrawFormatStringToHandle(777 + m_digitIndex * 150, 500,  GetColor(255, 0, 255), m_fontHandle, "��");
	}

	//�����R�[�h�ŏ������Ă���
	if (m_newName[m_digitIndex] >= 65 && m_newName[m_digitIndex] <= 90)
	{
		//�㉟���ƃA���t�@�x�b�g���������Ă���
		if (CInputManager::IsPush(KEY_INPUT_UP, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_UP, I_PAD))
		{
			m_newName[m_digitIndex]--;

			//A�̏�Ԃŏ㉟����Z�ɂȂ�
			if (m_newName[m_digitIndex] < 65)
				m_newName[m_digitIndex] = 'Z';
		}

		//���������ƃA���t�@�x�b�g���オ���Ă���
		if (CInputManager::IsPush(KEY_INPUT_DOWN, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_DOWN, I_PAD))
		{
			m_newName[m_digitIndex]++;

			//Z�̏�Ԃŉ���������A�ɂȂ�
			if (m_newName[m_digitIndex] > 90)
				m_newName[m_digitIndex] = 'A';
		}

		//�E�������Ǝ��̌���
		if (CInputManager::IsPush(KEY_INPUT_RIGHT, I_KEY) || CInputManager::IsPush(XINPUT_BUTTON_DPAD_RIGHT, I_PAD))
		{
			m_digitIndex++;

			if (m_digitIndex > 2)
				m_digitIndex = 0;
		}

		//���������ƑO�̌���
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
