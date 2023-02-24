#pragma once

#include <iostream>
#include <typeinfo>
#include <cassert>

#include "../Debug/Debug.h"

using namespace std;

//メモリ管理クラス
class CMemoryManager
{
public:
	static const int MEMORY_COUNT_MAX = 2048;	//確保最大数

	//メモリ領域情報構造体
	struct MemoryInfo
	{
		int		m_count;		//何番目に確保されたか
		int		m_line;			//何行目か
		void*	m_pointer;		//メモリポインタ
		string	m_fileName;		//ファイル名
		string	m_typeName;		//確保型名

		//コンストラクタ
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

	MemoryInfo m_memoryInfo[MEMORY_COUNT_MAX];	//メモリ情報
	int m_count;			//メモリ確保回数
	int m_workingCount;		//現在の確保数

	static CMemoryManager* m_memoryManager;//インスタンス

	//コンストラクタ
	CMemoryManager() :
		m_count(0),
		m_workingCount(0)
	{}

public:

	//マネージャー生成、取得、削除
	static CMemoryManager* CreateManager();
	static CMemoryManager* GetManager();
	void DeleteManager();

	//メモリ情報取得
	MemoryInfo* GetMemoryInfo()
	{
		return m_memoryInfo;
	}

	//初期化
	void Init();

	//New
	template<class Type>
	static void* New(void* ptr, const int line, const char* file)
	{
		//空いている場所を探す
		MemoryInfo* memory_info = m_memoryManager->GetMemoryInfo();
		int memory_index = 0;
		for (; memory_index < MEMORY_COUNT_MAX; ++memory_index, ++memory_info)
		{
			if (!memory_info->m_pointer) { break; }
		}
#ifdef DEBUG_MODE
		//空きがなかったら止める
		assert(memory_index < MEMORY_COUNT_MAX);
#endif
		//必要データを入れる
		memory_info->m_count = m_memoryManager->GetAddCount();
		memory_info->m_pointer = ptr;
		memory_info->m_typeName = typeid(Type).name();
		memory_info->m_line = line;
		memory_info->m_fileName = file;

		//現在のメモリ確保数加算
		m_memoryManager->AddWorkingCount();

		return ptr;
	}

	//Delete
	static void Delete(void* ptr);

	void DeleteAllMemoryInfo();

	//きれいに消せてるかのCheck
	void CheckComplete();

	int GetAddCount() { return ++m_count; }
	void AddWorkingCount() { ++m_workingCount; }
	
};