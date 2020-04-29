#include <iostream>
#include <thread>

#include "Task.h"
#include <complex>
#include "Utilities.h"


void value(HDC hdc, int x, int y)
{
	std::complex<float> point((float)x / Utils::SCR_WIDTH - 1.5, (float)y / Utils::SCR_HEIGHT - 0.5);

	std::complex<float> z(0, 0);
	int count = 0;
	while (abs(z) < 2 && count <= 34)
	{
		z = z * z + point;
		count++;
	}
	if (count < 34)
	{
		SetPixel(hdc, x, y, RGB(0, 0, 255));
	}
	else
	{
		SetPixel(hdc, x, y, RGB(0, 0, 0));
	}
}

CTask::CTask(HDC hdc)
	//:m_ivalue(0)
{
	// Goes through every pixel
	for (int i = 0; i < Utils::SCR_WIDTH; i++)
	{
		for (int j = 0; j < Utils::SCR_HEIGHT; j++)
		{
			value(hdc, i, j);
		}
	}
}

//CTask::CTask(int _value)
//	: m_ivalue(_value)
//{
//
//}

CTask::~CTask()
{

}

void CTask::operator()() const
{
	//Sleep to simulate work being done
	std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 101));

	// Goes through every pixel
	//for (int i = 0; i < Utils::SCR_WIDTH; i++)
	//{
	//	for (int j = 0; j < Utils::SCR_HEIGHT; j++)
	//	{
	//		value(hdc, i, j);
	//	}
	//}
}

int CTask::getValue() const
{
	return m_ivalue;
}

