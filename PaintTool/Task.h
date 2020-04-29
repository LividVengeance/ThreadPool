// A functor class

#ifndef __CTASK_H__
#define __CTASK_H__

#include <Windows.h>

class CTask
{
public:
	CTask(HDC hdc);
	//CTask(int _value);
	~CTask();
	void operator()() const;
	int getValue() const;
	HDC* hdc;
private:
	int m_ivalue;
};

#endif



