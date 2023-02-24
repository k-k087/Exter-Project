#include "DxLib.h"

#include <cassert>
#include <Windows.h>

#include "MemoryManager.h"
#include "../Common.h"
#include "../Debug/Debug.h"


CMemoryManager* CMemoryManager::m_memoryManager = NULL;//インスタンス

//マネージャー生成、取得、削除
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
		//その前に中身をすべて消す
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
		//見つけたら消す
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

//初期化
void CMemoryManager::Init()
{

}

//Delete
void CMemoryManager::Delete(void* ptr)
{
	//削除するポインタを探す
	MemoryInfo* memory_info = m_memoryManager->GetMemoryInfo();
	for (int memory_index = 0; memory_index < MEMORY_COUNT_MAX; ++memory_index, ++memory_info)
	{
		//見つけたら消す
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

	//最後までまわったら二重開放！！
	//もしくは登録されていないかも？
#ifdef DEBUG_MODE
	assert(0);
#endif
}

//きれいに消せてるかのCheck
void CMemoryManager::CheckComplete()
{
	//デバッグウィンドウに出力
	char frame[] = "\n\n============================================================================================================================================\n\n";

	char str[512];

	//区切り文字出力
	OutputDebugString(frame);

	//全部消せてるかのCheck
	MemoryInfo* memory_info = m_memoryInfo;
	int count = 0;

	for (int memory_index = 0; memory_index < MEMORY_COUNT_MAX;
		++memory_index, ++memory_info)
	{
		if (!memory_info->m_pointer) { continue; }

		//出力用バッファ初期化
		memset(str, 0, sizeof(str));

		//出力文字作成
		sprintf_s(str, "%s  (%d行目)  %d番目に確保された型名　%s \n\n",
			memory_info->m_fileName.c_str(),memory_info->m_line, 
			memory_info->m_count, memory_info->m_typeName.c_str());

		//出力
		OutputDebugString(str);

		++count;
	}

	//出力用バッファ初期化
	memset(str, 0, sizeof(str));
	sprintf_s(str, "解放忘れ数　：　%d", count);

	// 解放忘れ数出力
	OutputDebugString(str);

	// 区切り文字列出力
	OutputDebugString(frame);
	

}