#pragma once

#include <list>

#include "../GameManager/ManagerBase.h"

#include "../Object/Object.h"

using namespace std;

class CDrawManager
{
public:

private:
	int	m_shadowHandle;					//�e�n���h��
	list<CObject*> m_objectVec;			//�`�悷����̂̃��X�g
	static CDrawManager* m_drawManager;	//�C���X�^���X
	static int m_drawCount;				//�`�搔

	//�R���X�g���N�^
	CDrawManager();

public:
	//������
	void Init();

	//���ׂĂ�Draw
	static void DrawAllObject();
	void Draw();

	//�����A�擾�A�폜
	static CDrawManager* CreateManager();
	static CDrawManager* GetManager();
	void DeleteManager();

	//�f�[�^�ǉ�
	static int AddData(CObject* object);
	int Add(CObject* object);
	//�f�[�^�폜
	static void DeleteData(int draw_id);
	void Delete(int draw_id);

	/*
		Getter&Setter
	*/

	list<CObject*>* GetObjectVec()
	{
		return &m_objectVec;
	}

};