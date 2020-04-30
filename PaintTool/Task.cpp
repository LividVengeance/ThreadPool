#include <iostream>
#include <thread>

#include "Task.h"
#include <complex>
#include "Utilities.h"

std::mutex CTask::mtx;


CTask::CTask(HDC hdc, int i)
	:m_ivalue(0)
{
	taskHDC = hdc;
	x = i;
}

CTask::CTask()
{

}

CTask::~CTask()
{

}

void CTask::operator()() const
{
	//Sleep to simulate work being done
	//std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 101));

	// Goes through every pixel
	for (int y = 0; y < Utils::SCR_HEIGHT; y++)
	{
		std::complex<float> point((float)x / Utils::SCR_WIDTH - 1.0, (float)y / Utils::SCR_HEIGHT - 0.2);

		std::complex<float> z(0, 0);
		int count = 0;
		while (abs(z) < 2 && count <= 34)
		{
			z = z * z + point;
			count++;
		}
		if (count < 34)
		{
			mtx.lock();
			SetPixel(taskHDC, x, y, RGB(0, 0, 0));
			mtx.unlock();
		}
		else
		{
			mtx.lock();
			SetPixel(taskHDC, x, y, RGB(255, 192, 203));
			mtx.unlock();
		}
	}
}

int CTask::getValue() const
{
	return m_ivalue;
}

