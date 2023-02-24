#pragma once

//	アニメ種類
enum AnimeType
{
	ANIME_TYPE_NONE = -1,

	ANIME_TYPE_LOOP,		//アニメループ
	ANIME_TYPE_END,			//アニメエンド

	ANIME_TYPE_NUM,
};



/*
	アニメ再生クラス
*/
class CPlayAnimeData
{
public:
	int		m_animeID;		//アニメID
	float	m_speed;		//スピード
	int		m_handleIndex;	//アニメハンドルのインデックス
	bool	m_nameCheck;	//アニメチェック
	AnimeType	m_type;		//アニメ種類


	CPlayAnimeData() :
		m_animeID(0),
		m_speed(0),
		m_handleIndex(-1),
		m_nameCheck(false),
		m_type(ANIME_TYPE_NONE)
	{}
	CPlayAnimeData(int anime_id, float speed, int handle_index, bool name_check, AnimeType type)
	{
		m_animeID = anime_id;
		m_speed = speed;
		m_handleIndex = handle_index;
		m_nameCheck = name_check;
		m_type = type;
	}
	~CPlayAnimeData()
	{}
};

/*
	アニメーションするモデルに持たせるクラス
*/
class CAnimeInfo
{
public:
	float			m_frameTime;			//アニメ再生時間
	float			m_currentFrame;			//現在の再生時間

	float			m_loopStartFrame;		//ループのスタートフレーム
	float			m_loopEndFrame;			//ループの最終フレーム
	
	float			m_oldFrame;				//1フレーム前の再生時間
	float			m_speed;				//再生スピード
	float			m_reproductionSpeed;	//再生倍速
	float			m_allReproductionSpeed;	//再生倍速（全部）

	int				m_attachIndex;			//アニメアタッチインデックス
	int				m_animeID;				//現在のアニメID

	bool			m_stopFlg;				//アニメストップフラグ
	bool			m_reproductionFlg;		//ずっと倍速するかフラグ
	bool			m_loopFlg;				//特別にループするかフラグ

	AnimeType		m_type;					//アニメ状態

	int*			m_handle;				//元ハンドルのポインタ
											//注意：ポインタで管理しているので、
											//	　　vector管理の時は不具合がでるかも！！


	int				m_animeNum;				//アニメ数
	CPlayAnimeData* m_animeData;			//アニメデータの先頭ポインタ


public:
	CAnimeInfo():
		m_frameTime(),
		m_currentFrame(),
		m_loopStartFrame(),
		m_loopEndFrame(),
		m_oldFrame(),
		m_speed(),
		m_attachIndex(-1),
		m_reproductionSpeed(1.f),
		m_allReproductionSpeed(1.f),
		m_animeID(),
		m_stopFlg(false),
		m_loopFlg(false),
		m_reproductionFlg(false),
		m_animeNum(),
		m_handle(nullptr),
		m_animeData(nullptr),
		m_type(ANIME_TYPE_NONE)
	{}
	~CAnimeInfo() {}

public:

	//セットアニメデータ
	void SetAnimeData(const CPlayAnimeData* anime_data, int anime_num, int& handle);

	//アニメアップデート
	void UpdateAnime();

	//アニメ再生
	void PlayAnime(int anime_index);
	//アニメ切り替え
	void ChangeAnime(int anime_index);
	void ChangeAnime(int anime_index, int start_frame);
	void ChangeAnime(int anime_index, float frame_scal);
	//アニメ一時ストップ
	void StopAnime();
	//アニメが終了しているか
	bool IsEndAnime();

	//ループアニメ
	void LoopAnime();
	void LoopAnime(int start_frame, int end_frame);
	void LoopStopAnime();

	//特定のフレームか
	bool IsSpecificFrame(float frame);

	//間のフレームか
	bool IsBetweenFrame(int min, int max);
	bool IsBetweenFrame(float min, float max);

	//特定フレームまでに何回処理するか
	int GetProcessingNumToFrame(float frame);

	//再生倍速セット
	void SetReproductionSpeed(float reproduction_speed);
	//再生倍速オールセット
	void SetAllReproductionSpeed(float reproduction_speed);

	//再生倍速リセット
	void ResetReproductionSpeed();
	//再生倍速オールリセット
	void ResetAllReproductionSpeed();

private:
	//アニメデタッチ
	void DetachAnime();

public:
	//	ヒットストップ用静的変数＆関数=============================================================
	static bool		m_isAllStop;
	static int		m_AllstopCount;
	static int		m_AllstopMaxCount;

	static void		AllStopAnime(int stop_count);
	static void		AllStopAnimeStep();
	//	======================================================================================

};