// A functor class

#ifndef __CTASK_H__
#define __CTASK_H__

#include <Windows.h>

class CTask
{
public:
	CTask();
	CTask(int _value);
	~CTask();
	void operator()() const;
	int getValue() const;
private:
	int m_ivalue;
};

#endif



