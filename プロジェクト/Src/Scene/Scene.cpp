#include "Scene.h"

#include "../Input/Mouse/Mouse.h"

static const char* EFFECT_PATH = "Data/Graph/Effect/TitleEffect.png";
static const char* CUR_PATH = "Data/Graph/Cur/Cur.png";

static const int EFFECT_INDEX_COUNT = 40;
static const int EFFECT_COUNT = 34;

static const float EFFECT_ALPHA = DX_PI_F / EFFECT_INDEX_COUNT;

void CScene::InitEffect()
{
	memset(m_effectInfo, 0, sizeof(m_effectInfo));
	for (int effect_index = 0; effect_index < EFFECT_NUM; effect_index++)
	{
		m_effectInfo[effect_index].alpha = 140.f;
	}
	m_curHandle = -1;
}
void CScene::LoadEffect()
{
	LoadDivGraph(EFFECT_PATH, 4, 2, 2, 32, 32, m_effectHandle);
	m_curHandle = LoadGraph(CUR_PATH);
}
void CScene::FinEffect()
{
	for (int effect_index = 0; effect_index < 4; effect_index++)
	{
		DeleteGraph(m_effectHandle[effect_index]);
		m_effectHandle[effect_index] = -1;
	}

	DeleteGraph(m_curHandle); m_curHandle = -1;
}

void CScene::OccurEffect(float mouse_x, float mouse_y)
{
	EffectInfo* effect_info = m_effectInfo;
	for (int effect_index = 0; effect_index < EFFECT_NUM; effect_index++, effect_info++)
	{
		if (effect_info->isUse)continue;

		effect_info->isUse = true;
		effect_info->pos.x = mouse_x + CCommon::GetPMRandF(5.f);
		effect_info->pos.y = mouse_y + CCommon::GetPMRandF(5.f);
		effect_info->count = EFFECT_INDEX_COUNT;
		effect_info->index = 0;
		effect_info->scale = 0.8f;
		effect_info->rot = CCommon::GetPMRandF(DX_PI_F);

		break;
	}
}
void CScene::StepEffect()
{
	EffectInfo* effect_info = m_effectInfo;
	for (int effect_index = 0; effect_index < EFFECT_NUM; effect_index++, effect_info++)
	{
		if (!effect_info->isUse)continue;

		effect_info->count -= FRAME_TIME;
		effect_info->alpha += EFFECT_ALPHA;
		if (effect_info->count <= 0)
		{
			effect_info->index++;
			effect_info->scale += 0.2f;
			if (effect_info->index >= 4)
			{
				effect_info->isUse = false;
			}
			else
			{
				effect_info->count = EFFECT_INDEX_COUNT;
				effect_info->alpha = 0.f;
			}
		}
	}

	if (CMouse::GetMouse()->IsMoveMousePos())
	{
		int mouse_x, mouse_y;
		GetMousePoint(&mouse_x, &mouse_y);

		m_isCur = true;

		m_effectCount -= FRAME_TIME;
		if (m_effectCount <= 0)
		{
			m_effectCount = EFFECT_COUNT;
			OccurEffect((float)mouse_x, (float)mouse_y);
		}
	}
}

void CScene::DrawEffect()
{
	EffectInfo* effect_info = m_effectInfo;

	for (int effect_index = 0; effect_index < EFFECT_NUM; effect_index++, effect_info++)
	{
		if (!effect_info->isUse)continue;

		int effect_alpha = 140 + static_cast<int>(sinf(effect_info->alpha) * 155.f);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, effect_alpha);

		DrawRotaGraph(static_cast<int>(effect_info->pos.x), static_cast<int>(effect_info->pos.y),
			effect_info->scale, effect_info->rot, m_effectHandle[effect_info->index], true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (m_isCur)
	{
		int mouse_x, mouse_y;
		GetMousePoint(&mouse_x, &mouse_y);

		DrawGraph(mouse_x, mouse_y, m_curHandle, true);
	}
}