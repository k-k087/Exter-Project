#pragma once

//	�A�j�����
enum AnimeType
{
	ANIME_TYPE_NONE = -1,

	ANIME_TYPE_LOOP,		//�A�j�����[�v
	ANIME_TYPE_END,			//�A�j���G���h

	ANIME_TYPE_NUM,
};



/*
	�A�j���Đ��N���X
*/
class CPlayAnimeData
{
public:
	int		m_animeID;		//�A�j��ID
	float	m_speed;		//�X�s�[�h
	int		m_handleIndex;	//�A�j���n���h���̃C���f�b�N�X
	bool	m_nameCheck;	//�A�j���`�F�b�N
	AnimeType	m_type;		//�A�j�����


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
	�A�j���[�V�������郂�f���Ɏ�������N���X
*/
class CAnimeInfo
{
public:
	float			m_frameTime;			//�A�j���Đ�����
	float			m_currentFrame;			//���݂̍Đ�����

	float			m_loopStartFrame;		//���[�v�̃X�^�[�g�t���[��
	float			m_loopEndFrame;			//���[�v�̍ŏI�t���[��
	
	float			m_oldFrame;				//1�t���[���O�̍Đ�����
	float			m_speed;				//�Đ��X�s�[�h
	float			m_reproductionSpeed;	//�Đ��{��
	float			m_allReproductionSpeed;	//�Đ��{���i�S���j

	int				m_attachIndex;			//�A�j���A�^�b�`�C���f�b�N�X
	int				m_animeID;				//���݂̃A�j��ID

	bool			m_stopFlg;				//�A�j���X�g�b�v�t���O
	bool			m_reproductionFlg;		//�����Ɣ{�����邩�t���O
	bool			m_loopFlg;				//���ʂɃ��[�v���邩�t���O

	AnimeType		m_type;					//�A�j�����

	int*			m_handle;				//���n���h���̃|�C���^
											//���ӁF�|�C���^�ŊǗ����Ă���̂ŁA
											//	�@�@vector�Ǘ��̎��͕s����ł邩���I�I


	int				m_animeNum;				//�A�j����
	CPlayAnimeData* m_animeData;			//�A�j���f�[�^�̐擪�|�C���^


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

	//�Z�b�g�A�j���f�[�^
	void SetAnimeData(const CPlayAnimeData* anime_data, int anime_num, int& handle);

	//�A�j���A�b�v�f�[�g
	void UpdateAnime();

	//�A�j���Đ�
	void PlayAnime(int anime_index);
	//�A�j���؂�ւ�
	void ChangeAnime(int anime_index);
	void ChangeAnime(int anime_index, int start_frame);
	void ChangeAnime(int anime_index, float frame_scal);
	//�A�j���ꎞ�X�g�b�v
	void StopAnime();
	//�A�j�����I�����Ă��邩
	bool IsEndAnime();

	//���[�v�A�j��
	void LoopAnime();
	void LoopAnime(int start_frame, int end_frame);
	void LoopStopAnime();

	//����̃t���[����
	bool IsSpecificFrame(float frame);

	//�Ԃ̃t���[����
	bool IsBetweenFrame(int min, int max);
	bool IsBetweenFrame(float min, float max);

	//����t���[���܂łɉ��񏈗����邩
	int GetProcessingNumToFrame(float frame);

	//�Đ��{���Z�b�g
	void SetReproductionSpeed(float reproduction_speed);
	//�Đ��{���I�[���Z�b�g
	void SetAllReproductionSpeed(float reproduction_speed);

	//�Đ��{�����Z�b�g
	void ResetReproductionSpeed();
	//�Đ��{���I�[�����Z�b�g
	void ResetAllReproductionSpeed();

private:
	//�A�j���f�^�b�`
	void DetachAnime();

public:
	//	�q�b�g�X�g�b�v�p�ÓI�ϐ����֐�=============================================================
	static bool		m_isAllStop;
	static int		m_AllstopCount;
	static int		m_AllstopMaxCount;

	static void		AllStopAnime(int stop_count);
	static void		AllStopAnimeStep();
	//	======================================================================================

};