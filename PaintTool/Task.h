// A functor class
#include <mutex>



#ifndef __CTASK_H__
#define __CTASK_H__

#include <Windows.h>

class CTask
{
public:
	int x;
	
	CTask(HDC hdc, int i);
	CTask();
	~CTask();
	void operator()() const;
	int getValue() const;
	HDC taskHDC;
private:
	static std::mutex mtx;
	int m_ivalue;
};

#endif



