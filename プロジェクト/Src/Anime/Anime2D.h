#pragma once

class CAnime2DData
{
public:
	struct Anime2DDetail
	{
		int* m_handle;
		int m_frameNum;
		int m_frameTime;
		bool m_isLoop;
	};

private:
	Anime2DDetail* m_detail;
	int m_animeNum;
};
