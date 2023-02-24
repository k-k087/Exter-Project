#pragma once



class CSaveData
{
public:
	static const int MAX_NAME = 3;
	static const int MAX_RANK = 5;

	struct SaveInfo
	{
		int saveScore;				//読み込んだスコア
		char saveName[MAX_NAME];	//読み込んだ名前
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

	//スコアを書き込む関数
	bool WriteData();
	//データを読む
	void ReadData();
	void DrawData();
	//名前を書き込む関数
	void SelectName();

	static CSaveData* CreateSaveData();
	static CSaveData* GetSaveData();
	void DeleteSaveData();
};