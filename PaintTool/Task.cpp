#include <iostream>
#include <thread>

#include "Task.h"

CTask::CTask()
	:m_ivalue(0)
{

}

CTask::CTask(int _value)
	: m_ivalue(_value)
{

}

CTask::~CTask()
{

}

void CTask::operator()() const
{
	//Sleep to simulate work being done
	std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 101));
}

int CTask::getValue() const
{
	return m_ivalue;
}