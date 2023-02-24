#pragma once

#include <iostream>
#include <typeinfo>
#include <cassert>

#include "../Debug/Debug.h"

using namespace std;

//�������Ǘ��N���X
class CMemoryManager
{
public:
	static const int MEMORY_COUNT_MAX = 2048;	//�m�ۍő吔

	//�������̈���\����
	struct MemoryInfo
	{
		int		m_count;		//���ԖڂɊm�ۂ��ꂽ��
		int		m_line;			//���s�ڂ�
		void*	m_pointer;		//�������|�C���^
		string	m_fileName;		//�t�@�C����
		string	m_typeName;		//�m�ی^��

		//�R���X�g���N�^
		MemoryInfo() :
			m_count(0),
			m_line(0),
			m_pointer(NULL)
		{
			m_typeName.clear();
			m_fileName.clear();
		}
	};

private:

	MemoryInfo m_memoryInfo[MEMORY_COUNT_MAX];	//���������
	int m_count;			//�������m�ۉ�
	int m_workingCount;		//���݂̊m�ې�

	static CMemoryManager* m_memoryManager;//�C���X�^���X

	//�R���X�g���N�^
	CMemoryManager() :
		m_count(0),
		m_workingCount(0)
	{}

public:

	//�}�l�[�W���[�����A�擾�A�폜
	static CMemoryManager* CreateManager();
	static CMemoryManager* GetManager();
	void DeleteManager();

	//���������擾
	MemoryInfo* GetMemoryInfo()
	{
		return m_memoryInfo;
	}

	//������
	void Init();

	//New
	template<class Type>
	static void* New(void* ptr, const int line, const char* file)
	{
		//�󂢂Ă���ꏊ��T��
		MemoryInfo* memory_info = m_memoryManager->GetMemoryInfo();
		int memory_index = 0;
		for (; memory_index < MEMORY_COUNT_MAX; ++memory_index, ++memory_info)
		{
			if (!memory_info->m_pointer) { break; }
		}
#ifdef DEBUG_MODE
		//�󂫂��Ȃ�������~�߂�
		assert(memory_index < MEMORY_COUNT_MAX);
#endif
		//�K�v�f�[�^������
		memory_info->m_count = m_memoryManager->GetAddCount();
		memory_info->m_pointer = ptr;
		memory_info->m_typeName = typeid(Type).name();
		memory_info->m_line = line;
		memory_info->m_fileName = file;

		//���݂̃������m�ې����Z
		m_memoryManager->AddWorkingCount();

		return ptr;
	}

	//Delete
	static void Delete(void* ptr);

	void DeleteAllMemoryInfo();

	//���ꂢ�ɏ����Ă邩��Check
	void CheckComplete();

	int GetAddCount() { return ++m_count; }
	void AddWorkingCount() { ++m_workingCount; }
	
};