#pragma once



class CSaveData
{
public:
	static const int MAX_NAME = 3;
	static const int MAX_RANK = 5;

	struct SaveInfo
	{
		int saveScore;				//�ǂݍ��񂾃X�R�A
		char saveName[MAX_NAME];	//�ǂݍ��񂾖��O
	};
private:
	static CSaveData* m_saveData;

	SaveInfo m_saveInfo[MAX_RANK];

	int m_newScore;
	char m_newName[MAX_NAME];
	FILE* m_fileP;

	bool m_startSaveFlg;
	bool m_endSaveFlg;
	bool m_finishSelectNameFlg;
	int m_digitIndex;

	int m_fontHandle;

public:
	CSaveData() :
		m_saveInfo{},
		m_newScore(),
		m_fileP(),
		m_startSaveFlg(),
		m_endSaveFlg(),
		m_finishSelectNameFlg(),
		m_newName{'A','A','A'}
	{
	}
	~CSaveData() {}

	void Init();
	void Fin();

	//�X�R�A���������ފ֐�
	bool WriteData();
	//�f�[�^��ǂ�
	void ReadData();
	void DrawData();
	//���O���������ފ֐�
	void SelectName();

	static CSaveData* CreateSaveData();
	static CSaveData* GetSaveData();
	void DeleteSaveData();
};