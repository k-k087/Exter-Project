#include "DxLib.h"

#include "UI.h"
#include "UIManager.h"

#include "PlayUI/PlayUI.h"
#include "TitleUI/TitleUI.h"

#include "../Common.h"


CUIManager* CUIManager::m_UIManager = nullptr;

void CUIManager::Init()
{
	m_UI->Init();
}
void CUIManager::Load()
{
	m_UI->Load();
}
void CUIManager::Step()
{
	m_UI->Step();
}
void CUIManager::Draw()
{
	m_UI->Update();
	m_UI->Draw();
}
void CUIManager::Fin()
{
	m_UI->Fin();
}

//�}�l�[�W���[����
CUIManager* CUIManager::CreateManager()
{
	if (!m_UIManager)
		m_UIManager = _NEW(CUIManager);

	return m_UIManager;
}
//�}�l�[�W���[�Q�b�g
CUIManager* CUIManager::GetManager()
{
	return m_UIManager;
}
//�}�l�[�W���[�폜
void CUIManager::DeleteManager()
{
	if (m_UIManager)
	{
		if (m_UI)
		{
			m_UI->Fin();
			_DELETE(m_UI);
		}

		m_UI = nullptr;

		_DELETE(m_UIManager);
		m_UIManager = nullptr;
	}
}


//�����UI����
void CUIManager::CreateUI(UIKind kind)
{
	//����Ȃ炯��
	if (m_UI)
	{
		m_UI->Fin();
		_DELETE(m_UI);
	}

	switch (kind)
	{
	case UI_KIND_TITLE:
		break;

	case UI_KIND_PLAY:
		m_UI = CPlayUI::CreatePlayUI(); break;
	}
}
CUI* CUIManager::GetUI()
{
	return m_UI;
}