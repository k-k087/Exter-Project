#include "DxLib.h"

#include <cassert>
#include <Windows.h>

#include "MemoryManager.h"
#include "../Common.h"
#include "../Debug/Debug.h"


CMemoryManager* CMemoryManager::m_memoryManager = NULL;//�C���X�^���X

//�}�l�[�W���[�����A�擾�A�폜
CMemoryManager* CMemoryManager::CreateManager()
{

	m_memoryManager = new CMemoryManager;

	return m_memoryManager;
}
CMemoryManager* CMemoryManager::GetManager()
{
	
	return m_memoryManager;
}
void CMemoryManager::DeleteManager()
{
	if (m_memoryManager)
	{
		//���̑O�ɒ��g�����ׂď���
		DeleteAllMemoryInfo();

		delete m_memoryManager;

		m_memoryManager = nullptr;
	}
}

void CMemoryManager::DeleteAllMemoryInfo()
{
	MemoryInfo* memory_info = m_memoryManager->GetMemoryInfo();
	for (int memory_index = 0; memory_index < MEMORY_COUNT_MAX; ++memory_index, ++memory_info)
	{
		//�����������
		if (memory_info->m_pointer)
		{
			delete memory_info->m_pointer;

			memory_info->m_pointer = nullptr;
			memory_info->m_count = 0;
			memory_info->m_typeName.clear();
			memory_info->m_fileName.clear();
			memory_info->m_line = 0;
		}
	}
}

//������
void CMemoryManager::Init()
{

}

//Delete
void CMemoryManager::Delete(void* ptr)
{
	//�폜����|�C���^��T��
	MemoryInfo* memory_info = m_memoryManager->GetMemoryInfo();
	for (int memory_index = 0; memory_index < MEMORY_COUNT_MAX; ++memory_index, ++memory_info)
	{
		//�����������
		if (ptr == memory_info->m_pointer)
		{
			delete ptr;

			memory_info->m_pointer = nullptr;
			memory_info->m_count = 0;
			memory_info->m_typeName.clear();
			memory_info->m_fileName.clear();
			memory_info->m_line = 0;

			return;
		}
	}

	//�Ō�܂ł܂�������d�J���I�I
	//�������͓o�^����Ă��Ȃ������H
#ifdef DEBUG_MODE
	assert(0);
#endif
}

//���ꂢ�ɏ����Ă邩��Check
void CMemoryManager::CheckComplete()
{
	//�f�o�b�O�E�B���h�E�ɏo��
	char frame[] = "\n\n============================================================================================================================================\n\n";

	char str[512];

	//��؂蕶���o��
	OutputDebugString(frame);

	//�S�������Ă邩��Check
	MemoryInfo* memory_info = m_memoryInfo;
	int count = 0;

	for (int memory_index = 0; memory_index < MEMORY_COUNT_MAX;
		++memory_index, ++memory_info)
	{
		if (!memory_info->m_pointer) { continue; }

		//�o�͗p�o�b�t�@������
		memset(str, 0, sizeof(str));

		//�o�͕����쐬
		sprintf_s(str, "%s  (%d�s��)  %d�ԖڂɊm�ۂ��ꂽ�^���@%s \n\n",
			memory_info->m_fileName.c_str(),memory_info->m_line, 
			memory_info->m_count, memory_info->m_typeName.c_str());

		//�o��
		OutputDebugString(str);

		++count;
	}

	//�o�͗p�o�b�t�@������
	memset(str, 0, sizeof(str));
	sprintf_s(str, "����Y�ꐔ�@�F�@%d", count);

	// ����Y�ꐔ�o��
	OutputDebugString(str);

	// ��؂蕶����o��
	OutputDebugString(frame);
	

}