#include "DxLib.h"

#include "Field.h"

#include "../Draw/DrawManager.h"
#include "FieldManager.h"
#include "../Common.h"

static const char* FIELD_PATH = "Data/Model/Stage/STAGE1.x";

static const VECTOR FIELD_SCALE = { 1.7f, 1.2f, 1.7f };

static const float FIELD_SPEED = 10.f;

static const float MAX_FIELD_Z = 4500.f;

//����
CField* CField::Create()
{
	CField* field = _NEW(CField);

	//DrawList�ɒǉ�
	CDrawManager::AddData(field);

	return field;
}

//������
void CField::Init()
{
	m_radius = 800;
}

//���[�h
void CField::Load()
{
	m_handle = MV1LoadModel(FIELD_PATH);

	MV1SetScale(m_handle, FIELD_SCALE);
}

//�X�e�b�v
void CField::Step()
{

}

//�A�b�v�f�[�g
void CField::Update()
{
	//���W�Z�b�g
	MV1SetPosition(m_handle, m_pos);
}

//�`��
void CField::Draw()
{

	//�Ō�ɕ`��
	MV1DrawModel(m_handle);
}

void CField::Fin()
{

}