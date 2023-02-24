#include "DxLib.h"

#include "Sky.h"

#include "../Draw/DrawManager.h"
#include "../Character/Player/PlayerManager.h"
#include "../Character/Player/Player.h"
#include "../Common.h"


//���f���p�X
static const char* SKY_PATH = "Data/Model/Sky/Sky2.x";

//����
CSky* CSky::Create()
{
	CSky* sky = _NEW(CSky);

	//DrawList�ɒǉ�
	CDrawManager::AddData(sky);

	return sky;
}

//������
void CSky::Init()
{
	m_pos = VGet(0.f, 10.f, 0.f);
	m_handle = -1;
	m_rot = VGet(0.f, 0.f, 0.f);
}

//���[�h
void CSky::Load()
{
	if (m_handle == -1)
	{
		m_handle = MV1LoadModel(SKY_PATH);
	}

	//�傫���Z�b�g
	MV1SetScale(m_handle, VGet(50.f, 50.f, 50.f));
	m_rot.x = -DX_PI_F / 180.f * 2.f;
}

//�X�e�b�v
void CSky::Step()
{
	m_rot.y += 0.0004f;
	m_rot.z += 0.0002f;
}

//�A�b�v�f�[�g
void CSky::Update()
{
	MV1SetPosition(m_handle, m_pos);
	MV1SetRotationXYZ(m_handle, m_rot);
}

//�`��
void CSky::Draw()
{
	MV1DrawModel(m_handle);
}

void CSky::Fin()
{

}