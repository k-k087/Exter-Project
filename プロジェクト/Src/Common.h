
#ifndef __COMMON_H__
#define __COMMON_H__

#include "DxLib.h"

#include "Memory/MemoryManager.h"
#include "Debug/Debug.h"
#include "MyMath/MyMath.h"

//�E�B���h�E�̕��E����
const int WINDOW_HEIGHT = 720;
const int WINDOW_WIDTH = 1280;

//�ݒ�t���[�����[�g
const int FRAME_RATE = 60;
const int FRAME_TIME = (1000 / FRAME_RATE);

const float ZEROVALUE = 1.0000000e-010F;

//�O�x�N�^�[
#define ZERO_VECTOR  VGet(0.f,0.f,0.f)


//�p�x1�x
#define ONE_DEGREE	 DX_PI_F / 180.f

//�d��
#define GRAVITY 0.45f
#define MAX_GRAVITY 20.f

//void(*Func)()��typedef
typedef void (*VFunc)();

//VECTOR��int��
struct VECTOR_I
{
	int x, y, z;
};

#ifndef DEBUG_MODE

//new
#define _NEW( type )    new type
//Delete
#define _DELETE( ptr )  delete ptr

#else

// �f�o�b�O�pnew

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
#endif

//new
#define _NEW( type,... )  ( type* )( CMemoryManager::New<type>(new type,__LINE__,__FILE__))
//Delete
#define _DELETE( ptr )  CMemoryManager::Delete(ptr)


#endif

class CCommon
{
public:
	//�m��(�p�[�Z���g)
	static bool GetProbability(int value)
	{
		int rand = GetRand(99) + 1;
		if (rand <= value)
		{
			return true;
		}
		
		return false;
	}
	//VECTOR���[���ł��邩
	static bool CheckZeroVector(VECTOR v1)
	{
		if (v1.x == 0.f && v1.y == 0.f && v1.z == 0.f)
			return true;

		return false;
	}

	//-x~+x�܂ł̃����_���l(float)
	static float GetPMRandF(float f)
	{
		float result = static_cast<float>(GetRand(static_cast<int>(f) * 10000)) / 10000.f;

		if (GetRand(1) == 0) { result *= -1.f; }

		return result;
	}

	//-f<=x<=-add, +add<=x<=+f�̃����_���l(float)
	static float GetPMRandF(float f, float add)
	{
		float ram_f = f - add;

		float result = static_cast<float>(GetRand(static_cast<int>(ram_f) * 10000)) / 10000.f + add;

		if (GetRand(1) == 0) 
			result *= -1.f; 
			
		return result;
	}

	//-f<=x<=-add, +add<=x<=+f�̃����_���l(float)
	static VECTOR GetPMRandV(VECTOR p, float f, float add)
	{
		float ram_f = f - add;

		VECTOR m = { 
			static_cast<float>(GetRand(static_cast<int>(ram_f) * 10000)) / 10000.f + add,
			0.f,
			static_cast<float>(GetRand(static_cast<int>(ram_f) * 10000)) / 10000.f + add,
		};

		if (GetRand(1) == 0)
			m = CMyMath::VecScale(m, -1.f);

		VECTOR result = CMyMath::VecAdd(p, m);

		return result;
	}

	//���̒l���߂��l��
	static bool IsNearF(float check_num, float ans_num, float ans_min_max)
	{
		float min = ans_num - ans_min_max;
		float max = ans_num + ans_min_max;

		if (min < check_num && check_num < max)
		{
			return true;
		}

		return false;
	}
};
#endif