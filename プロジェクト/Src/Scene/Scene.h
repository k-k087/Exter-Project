#pragma once

#include "../Common.h"

//���݂̃V�[��
enum SceneID
{
	SCENE_ID_NONE = -1,

	SCENE_ID_TITLE,		//�^�C�g��
	SCENE_ID_SELECT,	//�Z���N�g���
	SCENE_ID_PLAY,		//�v���C
	SCENE_ID_GAMEOVER,	//�Q�[���I�[�o�[
	SCENE_ID_CLEAR,		//�N���A

	SCENE_ID_NUM,
};

//���݂̏���
enum SceneFuncID
{
	SCENE_FUNC_ID_NONE = -1,

	SCENE_FUNC_ID_INIT,	//������
	SCENE_FUNC_ID_LOAD,	//���[�h
	SCENE_FUNC_ID_LOOP,	//���[�v
	SCENE_FUNC_ID_FIN,	//�㏈��

	SCENE_FUNC_ID_NUM,
};

//�V�[���N���X
class CScene
{
public:
	struct EffectInfo
	{
		VECTOR pos;
		int index;
		int count;
		float scale;
		float rot;
		float alpha;

		bool isUse;
	};

	static const int EFFECT_NUM = 10;


	//������
	virtual void Init() = 0;
	//���[�h
	virtual void Load() = 0;
	//�ʏ폈��
	virtual void Step() = 0;
	//�`��
	virtual void Draw() = 0;
	//�㏈��
	virtual void Fin() = 0;
	//���[�v����
	virtual void Loop()
	{
		Step();
		Draw();
	}

protected:
	EffectInfo m_effectInfo[EFFECT_NUM];
	int m_effectHandle[4];
	int m_curHandle;
	bool m_isCur;
	int	m_effectCount;

public:
	CScene():
		m_effectInfo{},
		m_effectHandle{-1},
		m_curHandle(-1),
		m_isCur(),
		m_effectCount()
	{};
	virtual ~CScene() = default;

public:
	void InitEffect();
	void LoadEffect();
	void FinEffect();

	void OccurEffect(float mouse_x, float mouse_y);
	void StepEffect();
	void DrawEffect();
};