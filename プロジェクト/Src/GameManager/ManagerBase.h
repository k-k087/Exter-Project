#pragma once

#include "DxLib.h"

class CManagerBase
{
public:
	virtual void Step() = 0;

	//�}�l�[�W���[���
	//(�D��x���ɕ���ł���)
	enum ManagerKind
	{
		MG_KIND_NONE = -1,

		MG_KIND_CHARACTER,	//�L�����N�^�[
		MG_KIND_MAGIC,		//���@
		MG_KIND_ITEM,		//�A�C�e��
		MG_KIND_BEAM,		//�r�[��
		MG_KIND_FIELD,		//�t�B�[���h
		MG_KIND_SKY,		//�V��
		MG_KIND_CAMERA,		//�J����
		MG_KIND_UI,			//���[�U�C���^�t�F�[�X
		MG_KIND_COLLISION,	//�����蔻��`�F�b�N

		MG_KIND_NUM,
	};

protected:
	ManagerKind m_kind; //�}�l�[�W���[�̎��

public:
	//Getter&Setter

	ManagerKind GetKind()
	{
		return m_kind;
	}
};