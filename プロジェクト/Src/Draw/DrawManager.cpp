#include "DxLib.h"

#include <list>

#include "DrawManager.h"

#include "../Object/Object.h"
#include "../Common.h"


CDrawManager* CDrawManager::m_drawManager = NULL;
int CDrawManager::m_drawCount;

//�R���X�g���N�^
CDrawManager::CDrawManager() :
	m_shadowHandle(-1)
{
	m_drawCount = 0;
}

//������
void CDrawManager::Init()
{
	m_drawCount = 0;
	m_shadowHandle = MakeShadowMap(4096, 4096);
	SetShadowMapLightDirection(m_shadowHandle, VGet(1.f, -1.f, 0.3f));

	//	�e�͈͂��Z�b�g
	SetShadowMapDrawArea(m_shadowHandle, VGet(-1000.f, -1.f, -1000.f),
		VGet(1000.f, 200.f, 1000.f));
}

void CDrawManager::DrawAllObject()
{
	m_drawManager->Draw();
}
void CDrawManager::Draw()
{
	//�܂�Uptate
	for (const auto& draw_vec : m_objectVec)
	{
		draw_vec->Update();
	}

	//�e������ɕ`��
	if (m_shadowHandle != -1)
	{
		ShadowMap_DrawSetup(m_shadowHandle);

		//Draw�����Ԃɉ�
		for (const auto& draw_vec : m_objectVec)
		{
			draw_vec->Draw();
		}

		ShadowMap_DrawEnd();
	}

	//�ʏ�Draw�����Ԃɉ�
	SetUseShadowMap(0, m_shadowHandle);

	for (const auto& draw_vec : m_objectVec)
	{
		draw_vec->Draw();
	}

	SetUseShadowMap(0, -1);
}

CDrawManager* CDrawManager::CreateManager()
{
	//�Ȃ��Ȃ���
	if (!m_drawManager)
	{
		m_drawManager = _NEW(CDrawManager);
	}

	return m_drawManager;
}
CDrawManager* CDrawManager::GetManager()
{
	if (!m_drawManager) { return nullptr; }

	//����Ȃ�n��
	return m_drawManager;
}
void CDrawManager::DeleteManager()
{
	if (!m_drawManager) { return; }
	
	//���X�g�̒��g�N���A
	m_objectVec.clear();

	DeleteShadowMap(m_shadowHandle);

	//�폜
	_DELETE(m_drawManager);

	m_drawManager = nullptr;
	
}

//�f�[�^�ǉ�
int CDrawManager::AddData(CObject* object)
{
	return m_drawManager->Add(object);
}
int CDrawManager::Add(CObject* object)
{
	//ID�p�ϐ���ێ�
	int draw_id = m_drawCount;
	
#if 0
	//��ގ擾
	CObject::ObjectKind kind = object->GetKind();


	//�擪�Ȃ炻�̂܂ܒǉ�
	if (m_drawCount == 0)
	{
		m_objectVec.push_back(object);
	}
	//�擪������Ȃ�D��x���ɒǉ�
	else
	{
		//���ꂽ���ꏊ�܂ŉ��
		auto draw_vec = m_objectVec.begin();
		auto vec_end = m_objectVec.end();
		for (; draw_vec != vec_end && (*draw_vec)->GetKind() < kind; draw_vec++) {}

		//�f�[�^�̒ǉ�
		m_objectVec.insert(draw_vec, object);
	}
#endif

	//���Ƀf�[�^������
	m_objectVec.push_back(object);

	//�J�E���g���Z
	++m_drawCount;

	return draw_id;
}

//�f�[�^�폜
void CDrawManager::DeleteData(int draw_id)
{
	m_drawManager->Delete(draw_id);
}
void CDrawManager::Delete(int draw_id)
{
	////���������f�[�^�܂ŉ��
	//for (auto draw_vec = m_)
}