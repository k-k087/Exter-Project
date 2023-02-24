#include "../Common.h"

#include "Anime.h"

//	静的変数宣言====================================================
bool CAnimeInfo::m_isAllStop = false;
int	CAnimeInfo::m_AllstopCount = 0;
int CAnimeInfo::m_AllstopMaxCount = 0;
//	==============================================================


//	セットアニメデータ
void CAnimeInfo::SetAnimeData(const CPlayAnimeData* anime_data, int anime_num, int& handle)
{
	//	データがなければ先頭ポインタにセット
	if (!m_animeData)
		m_animeData = const_cast<CPlayAnimeData*>(anime_data);

	//	アニメ数セット
	m_animeNum = anime_num;

	//	ハンドルをポインタで管理
	if (!m_handle)
		m_handle = &handle;
}

//	アニメアップデート
void CAnimeInfo::UpdateAnime()
{
	if (m_attachIndex == -1)
		return;

	//	ストップならreturn
	if (m_stopFlg)
		return;

	//	記憶
	m_oldFrame = m_currentFrame;

	//	カウント加算
	if (!m_isAllStop)
		m_currentFrame += (m_speed * m_reproductionSpeed * m_allReproductionSpeed);

	//	先に特別なループ再生の判定
	if (m_loopFlg)
	{
		if (m_currentFrame >= m_loopEndFrame)
		{
			m_currentFrame = m_loopStartFrame;
		}
	}
	
	//	次ぎに普通の判定
	if (m_currentFrame >= m_frameTime)
	{
		//	最後まできたら種類によって処理を変える
		switch(m_type)
		{
			//	ループ
		case ANIME_TYPE_LOOP:
			m_currentFrame = 0.f;
			break;

			//	終了
		case ANIME_TYPE_END:
			m_currentFrame = m_frameTime;
			break;
		}
	}
	
}

//	アニメ再生
void CAnimeInfo::PlayAnime(int anime_index)
{
	//	デタッチ
	DetachAnime();

	//	必要情報を入れ、再生
	int play_anime_id = m_animeData[anime_index].m_animeID;
	int play_handle = m_animeData[anime_index].m_handleIndex;
	bool play_name_check = m_animeData[anime_index].m_nameCheck;
	float play_speed = m_animeData[anime_index].m_speed;

	m_attachIndex = MV1AttachAnim(*m_handle, play_anime_id, play_handle, play_name_check);
	m_frameTime = MV1GetAnimTotalTime(*m_handle, play_anime_id);
	m_speed = play_speed;
	m_type = m_animeData[anime_index].m_type;
	m_currentFrame = 0.f;
	m_oldFrame = 0.f;
	m_reproductionSpeed = 1.f;
	m_animeID = play_anime_id;
}

//	アニメ切り替え
void CAnimeInfo::ChangeAnime(int anime_index)
{
	//	同じアニメを再生しようとするならreturn
	if (m_animeID == m_animeData[anime_index].m_animeID)
		return;

	//	デタッチ
	DetachAnime();

	int play_anime_id = m_animeData[anime_index].m_animeID;
	int play_handle = m_animeData[anime_index].m_handleIndex;
	bool play_name_check = m_animeData[anime_index].m_nameCheck;
	float play_speed = m_animeData[anime_index].m_speed;

	//再生し、必要情報を入れる
	m_attachIndex = MV1AttachAnim(*m_handle, play_anime_id, play_handle, play_name_check);
	m_frameTime = MV1GetAnimTotalTime(*m_handle, play_anime_id);
	m_speed = play_speed;
	m_type = m_animeData[anime_index].m_type;
	m_currentFrame = m_oldFrame = 0.f;
	m_reproductionSpeed = 1.f;
	m_animeID = play_anime_id;
}
//	アニメ切り替え（スタート再生時間を決めれる）
void CAnimeInfo::ChangeAnime(int anime_index, int start_frame)
{
	//	同じアニメを再生しようとするならreturn
	if (m_animeID == m_animeData[anime_index].m_animeID)
		return;

	//	デタッチ
	DetachAnime();

	int play_anime_id = m_animeData[anime_index].m_animeID;
	int play_handle = m_animeData[anime_index].m_handleIndex;
	bool play_name_check = m_animeData[anime_index].m_nameCheck;
	float play_speed = m_animeData[anime_index].m_speed;

	//再生し、必要情報を入れる
	m_attachIndex = MV1AttachAnim(*m_handle, play_anime_id, play_handle, play_name_check);
	m_frameTime = MV1GetAnimTotalTime(*m_handle, play_anime_id);
	m_speed = play_speed;
	m_type = m_animeData[anime_index].m_type;
	m_currentFrame = m_oldFrame = static_cast<float>(start_frame);
	m_reproductionSpeed = 1.f;
	m_animeID = play_anime_id;
}
//	アニメ切り替え（スタート再生時間を割合で決めれる）
void CAnimeInfo::ChangeAnime(int anime_index, float frame_scal)
{
	//	同じアニメを再生しようとするならreturn
	if (m_animeID == m_animeData[anime_index].m_animeID)
		return;

	//デタッチ
	DetachAnime();

	int play_anime_id = m_animeData[anime_index].m_animeID;
	int play_handle = m_animeData[anime_index].m_handleIndex;
	bool play_name_check = m_animeData[anime_index].m_nameCheck;
	float play_speed = m_animeData[anime_index].m_speed;

	//	再生し、必要情報を入れる
	m_attachIndex = MV1AttachAnim(*m_handle, play_anime_id, play_handle, play_name_check);
	m_frameTime = MV1GetAnimTotalTime(*m_handle, play_anime_id);
	m_speed = play_speed;
	m_type = m_animeData[anime_index].m_type;
	m_currentFrame = m_oldFrame = (frame_scal * m_frameTime);
	m_reproductionSpeed = 1.f;
	m_animeID = play_anime_id;
}
//	アニメ一時ストップ
void CAnimeInfo::StopAnime()
{
	m_stopFlg = true;
}
//	アニメが終了しているか
bool CAnimeInfo::IsEndAnime()
{
	if (m_currentFrame == m_frameTime)
		return true;
	
	return false;
}

//	ループアニメ
void CAnimeInfo::LoopAnime()
{
	m_loopFlg = true;
	m_loopStartFrame = 0.f;
	m_loopEndFrame = MV1GetAnimTotalTime(*m_handle, m_animeID);
}
//	ループアニメ（スタートとエンドを決める）
void CAnimeInfo::LoopAnime(int start_frame, int end_frame)
{
	float fin_frame = MV1GetAnimTotalTime(*m_handle, m_animeID);
	if (fin_frame < start_frame || fin_frame < end_frame)
		return;
	
	m_loopFlg = true;
	m_loopStartFrame = static_cast<float>(start_frame);
	m_loopEndFrame = static_cast<float>(end_frame);
}
//	特別ループストップ
void CAnimeInfo::LoopStopAnime()
{
	m_loopFlg = false;
	m_loopStartFrame = m_loopEndFrame = 0.f;
}


//特定のフレームか
bool CAnimeInfo::IsSpecificFrame(float frame)
{
	//前フレがその値以下、現フレがその値以上
	if (m_oldFrame < frame && frame <= m_currentFrame)
		return true;

	return false;
}

//間のフレームか
bool CAnimeInfo::IsBetweenFrame(int min, int max)
{
	if (static_cast<float>(min) <= m_currentFrame && m_currentFrame <= static_cast<float>(max))
		return true;
	
	return false;
}
bool CAnimeInfo::IsBetweenFrame(float min, float max)
{
	if (min <= m_currentFrame && m_currentFrame <= max)
		return true;

	return false;
}

//特定フレームまでに何回処理するか
int CAnimeInfo::GetProcessingNumToFrame(float frame)
{
	float current_frame = m_currentFrame;
	float frame_time = (m_speed * m_reproductionSpeed * m_allReproductionSpeed);
	int count = 0;

	while (frame > current_frame)
	{
		current_frame += frame_time;
		count++;
	}

	return count;
}

//アニメデタッチ
void CAnimeInfo::DetachAnime()
{
	if (m_attachIndex != -1)
	{
		MV1DetachAnim(*m_handle, m_attachIndex);
		m_attachIndex = -1;
		m_currentFrame = 0.f;
	}
}

//再生倍速セット
void CAnimeInfo::SetReproductionSpeed(float reproduction_speed)
{
	m_reproductionSpeed = reproduction_speed;
}
//再生倍速オールセット
void CAnimeInfo::SetAllReproductionSpeed(float reproduction_speed)
{
	m_allReproductionSpeed = reproduction_speed;
	m_reproductionFlg = true;
}

//再生倍速リセット
void CAnimeInfo::ResetReproductionSpeed()
{
	m_reproductionSpeed = 1.f;
}
//再生倍速リセット(１にする)
void CAnimeInfo::ResetAllReproductionSpeed()
{
	m_allReproductionSpeed = 1.f;
	m_reproductionFlg = false;
}


void CAnimeInfo::AllStopAnime(int stop_count)
{
	m_isAllStop = true;
	m_AllstopMaxCount = stop_count;
	m_AllstopCount = 0;
}

void CAnimeInfo::AllStopAnimeStep()
{
	if (!m_isAllStop) { return; }

	//止まっている間
	m_AllstopCount += FRAME_TIME;

	if (m_AllstopCount >= m_AllstopMaxCount)
	{
		m_isAllStop = false;
		m_AllstopCount = m_AllstopMaxCount = 0;
	}
	
}