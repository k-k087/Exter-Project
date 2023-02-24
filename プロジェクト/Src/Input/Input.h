
#ifndef __INPUT_H__
#define __INPUT_H__

//インプットクラス
class CInput
{
public:
	virtual void Init() = 0;
	virtual void Step() = 0;
	
	virtual bool IsPush(int code) = 0;
	virtual bool IsKeep(int code) = 0;
	virtual bool IsRelease(int code) = 0;
	virtual bool IsDown(int code) = 0;
};

#endif

