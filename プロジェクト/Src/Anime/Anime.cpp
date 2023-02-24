#include "../Common.h"

#include "Anime.h"

//	�ÓI�ϐ��錾====================================================
bool CAnimeInfo::m_isAllStop = false;
int	CAnimeInfo::m_AllstopCount = 0;
int CAnimeInfo::m_AllstopMaxCount = 0;
//	==============================================================


//	�Z�b�g�A�j���f�[�^
void CAnimeInfo::SetAnimeData(const CPlayAnimeData* anime_data, int anime_num, int& handle)
{
	//	�f�[�^���Ȃ���ΐ擪�|�C���^�ɃZ�b�g
	if (!m_animeData)
		m_animeData = const_cast<CPlayAnimeData*>(anime_data);

	//	�A�j�����Z�b�g
	m_animeNum = anime_num;

	//	�n���h�����|�C���^�ŊǗ�
	if (!m_handle)
		m_handle = &handle;
}

//	�A�j���A�b�v�f�[�g
void CAnimeInfo::UpdateAnime()
{
	if (m_attachIndex == -1)
		return;

	//	�X�g�b�v�Ȃ�return
	if (m_stopFlg)
		return;

	//	�L��
	m_oldFrame = m_currentFrame;

	//	�J�E���g���Z
	if (!m_isAllStop)
		m_currentFrame += (m_speed * m_reproductionSpeed * m_allReproductionSpeed);

	//	��ɓ��ʂȃ��[�v�Đ��̔���
	if (m_loopFlg)
	{
		if (m_currentFrame >= m_loopEndFrame)
		{
			m_currentFrame = m_loopStartFrame;
		}
	}
	
	//	�����ɕ��ʂ̔���
	if (m_currentFrame >= m_frameTime)
	{
		//	�Ō�܂ł������ނɂ���ď�����ς���
		switch(m_type)
		{
			//	���[�v
		case ANIME_TYPE_LOOP:
			m_currentFrame = 0.f;
			break;

			//	�I��
		case ANIME_TYPE_END:
			m_currentFrame = m_frameTime;
			break;
		}
	}
	
}

//	�A�j���Đ�
void CAnimeInfo::PlayAnime(int anime_index)
{
	//	�f�^�b�`
	DetachAnime();

	//	�K�v�������A�Đ�
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

//	�A�j���؂�ւ�
void CAnimeInfo::ChangeAnime(int anime_index)
{
	//	�����A�j�����Đ����悤�Ƃ���Ȃ�return
	if (m_animeID == m_animeData[anime_index].m_animeID)
		return;

	//	�f�^�b�`
	DetachAnime();

	int play_anime_id = m_animeData[anime_index].m_animeID;
	int play_handle = m_animeData[anime_index].m_handleIndex;
	bool play_name_check = m_animeData[anime_index].m_nameCheck;
	float play_speed = m_animeData[anime_index].m_speed;

	//�Đ����A�K�v��������
	m_attachIndex = MV1AttachAnim(*m_handle, play_anime_id, play_handle, play_name_check);
	m_frameTime = MV1GetAnimTotalTime(*m_handle, play_anime_id);
	m_speed = play_speed;
	m_type = m_animeData[anime_index].m_type;
	m_currentFrame = m_oldFrame = 0.f;
	m_reproductionSpeed = 1.f;
	m_animeID = play_anime_id;
}
//	�A�j���؂�ւ��i�X�^�[�g�Đ����Ԃ����߂��j
void CAnimeInfo::ChangeAnime(int anime_index, int start_frame)
{
	//	�����A�j�����Đ����悤�Ƃ���Ȃ�return
	if (m_animeID == m_animeData[anime_index].m_animeID)
		return;

	//	�f�^�b�`
	DetachAnime();

	int play_anime_id = m_animeData[anime_index].m_animeID;
	int play_handle = m_animeData[anime_index].m_handleIndex;
	bool play_name_check = m_animeData[anime_index].m_nameCheck;
	float play_speed = m_animeData[anime_index].m_speed;

	//�Đ����A�K�v��������
	m_attachIndex = MV1AttachAnim(*m_handle, play_anime_id, play_handle, play_name_check);
	m_frameTime = MV1GetAnimTotalTime(*m_handle, play_anime_id);
	m_speed = play_speed;
	m_type = m_animeData[anime_index].m_type;
	m_currentFrame = m_oldFrame = static_cast<float>(start_frame);
	m_reproductionSpeed = 1.f;
	m_animeID = play_anime_id;
}
//	�A�j���؂�ւ��i�X�^�[�g�Đ����Ԃ������Ō��߂��j
void CAnimeInfo::ChangeAnime(int anime_index, float frame_scal)
{
	//	�����A�j�����Đ����悤�Ƃ���Ȃ�return
	if (m_animeID == m_animeData[anime_index].m_animeID)
		return;

	//�f�^�b�`
	DetachAnime();

	int play_anime_id = m_animeData[anime_index].m_animeID;
	int play_handle = m_animeData[anime_index].m_handleIndex;
	bool play_name_check = m_animeData[anime_index].m_nameCheck;
	float play_speed = m_animeData[anime_index].m_speed;

	//	�Đ����A�K�v��������
	m_attachIndex = MV1AttachAnim(*m_handle, play_anime_id, play_handle, play_name_check);
	m_frameTime = MV1GetAnimTotalTime(*m_handle, play_anime_id);
	m_speed = play_speed;
	m_type = m_animeData[anime_index].m_type;
	m_currentFrame = m_oldFrame = (frame_scal * m_frameTime);
	m_reproductionSpeed = 1.f;
	m_animeID = play_anime_id;
}
//	�A�j���ꎞ�X�g�b�v
void CAnimeInfo::StopAnime()
{
	m_stopFlg = true;
}
//	�A�j�����I�����Ă��邩
bool CAnimeInfo::IsEndAnime()
{
	if (m_currentFrame == m_frameTime)
		return true;
	
	return false;
}

//	���[�v�A�j��
void CAnimeInfo::LoopAnime()
{
	m_loopFlg = true;
	m_loopStartFrame = 0.f;
	m_loopEndFrame = MV1GetAnimTotalTime(*m_handle, m_animeID);
}
//	���[�v�A�j���i�X�^�[�g�ƃG���h�����߂�j
void CAnimeInfo::LoopAnime(int start_frame, int end_frame)
{
	float fin_frame = MV1GetAnimTotalTime(*m_handle, m_animeID);
	if (fin_frame < start_frame || fin_frame < end_frame)
		return;
	
	m_loopFlg = true;
	m_loopStartFrame = static_cast<float>(start_frame);
	m_loopEndFrame = static_cast<float>(end_frame);
}
//	���ʃ��[�v�X�g�b�v
void CAnimeInfo::LoopStopAnime()
{
	m_loopFlg = false;
	m_loopStartFrame = m_loopEndFrame = 0.f;
}


//����̃t���[����
bool CAnimeInfo::IsSpecificFrame(float frame)
{
	//�O�t�������̒l�ȉ��A���t�������̒l�ȏ�
	if (m_oldFrame < frame && frame <= m_currentFrame)
		return true;

	return false;
}

//�Ԃ̃t���[����
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

//����t���[���܂łɉ��񏈗����邩
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

//�A�j���f�^�b�`
void CAnimeInfo::DetachAnime()
{
	if (m_attachIndex != -1)
	{
		MV1DetachAnim(*m_handle, m_attachIndex);
		m_attachIndex = -1;
		m_currentFrame = 0.f;
	}
}

//�Đ��{���Z�b�g
void CAnimeInfo::SetReproductionSpeed(float reproduction_speed)
{
	m_reproductionSpeed = reproduction_speed;
}
//�Đ��{���I�[���Z�b�g
void CAnimeInfo::SetAllReproductionSpeed(float reproduction_speed)
{
	m_allReproductionSpeed = reproduction_speed;
	m_reproductionFlg = true;
}

//�Đ��{�����Z�b�g
void CAnimeInfo::ResetReproductionSpeed()
{
	m_reproductionSpeed = 1.f;
}
//�Đ��{�����Z�b�g(�P�ɂ���)
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

	//�~�܂��Ă����
	m_AllstopCount += FRAME_TIME;

	if (m_AllstopCount >= m_AllstopMaxCount)
	{
		m_isAllStop = false;
		m_AllstopCount = m_AllstopMaxCount = 0;
	}
	
}