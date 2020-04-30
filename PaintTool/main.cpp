/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2019 Media Design School

  File Name   :   main.cpp
  Description :   Creates a Mandelbrot Fractal Set using a multiple threaded 
  Author      :   Andrew Barnes
  Mail        :   andrew.bar8456@mediadesign.school.nz
********************/

#include <iostream>
#include <windows.h>   // Include all the windows headers.
#include <windowsx.h>  // Include useful macros.
#include <math.h>
#include <time.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN

#include "resource.h"
#include "ThreadPool.h"
#include "WorkQueue.h"
#include "Task.h"
#include "Utilities.h"
#include <complex>

#define WINDOW_CLASS_NAME L"WINCLASS1"

//Global variables
HINSTANCE g_hInstance;
HMENU g_hMenu;

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void GameLoop()
{
	//One frame of game logic occurs here...
}

int mainFunc(HDC hdc, HWND _hwnd)
{
	//auto start = std::chrono::high_resolution_clock::now();
	typedef std::chrono::high_resolution_clock Time;
	typedef std::chrono::milliseconds ms;
	typedef std::chrono::duration<float> fsec;
	auto timeStart = Time::now();


	srand((unsigned int)time(0));
	const int kiTOTALITEMS = Utils::SCR_WIDTH;
	//Create a ThreadPool Object capable of holding as many threads as the number of cores
	ThreadPool& threadPool = ThreadPool::GetInstance();
	//Initialize the pool
	threadPool.Initialize(hdc);
	threadPool.Start();
	// The main thread writes items to the WorkQueue
	for (int i = 0; i < kiTOTALITEMS; i++)
	{
		threadPool.Submit(CTask(hdc, i));
		std::cout << "Main Thread wrote item " << i << " to the Work Queue " << std::endl;
	}
	while (threadPool.getItemsProcessed() != kiTOTALITEMS)
	{
	
	}
	if (threadPool.getItemsProcessed() == kiTOTALITEMS)
	{
		threadPool.Stop();
	}
	
	threadPool.DestroyInstance();

	auto timeEnd = Time::now();
	fsec timeFinish = timeEnd - timeStart;
	ms d = std::chrono::duration_cast<ms>(timeFinish);
	float test = timeFinish.count();
	
	std::string timeTakeStr = "Time taken: " + std::to_string(test) + "ms";

	std::wstring timeMsg = s2ws(timeTakeStr);
	LPCWSTR timeResult = timeMsg.c_str();

	MessageBox(_hwnd, timeResult, L"Time Taken to Complete", MB_OK);
	return(0);
}

LRESULT CALLBACK WindowProc(HWND _hwnd,
	UINT _msg,
	WPARAM _wparam,
	LPARAM _lparam)
{
	// This is the main message handler of the system.
	PAINTSTRUCT ps;
	HDC hdc;        // Handle to a device context.

	switch (_msg)
	{
	case WM_CREATE:
	{
		// Return Success.
		return (0);
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(_wparam))
		{
		case ID_FILE_EXIT:
		{
			PostQuitMessage(0);
			break;
		}
		case ID_HELP_ABOUT:
		{
			MessageBox(_hwnd, L"This thread pool mandelbrot fractal set program was developed by Andrew Barnes", L"Author Information", MB_OK | MB_ICONINFORMATION);
			break;
		}
		default:
			break;
		}
		return(0);
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(_hwnd, &ps);
		mainFunc(hdc, _hwnd);
		break;
	}
	case WM_DESTROY:
	{
		// Kill the application, this sends a WM_QUIT message.
		PostQuitMessage(0);
		// Return success.
		return (0);
	}
	break;

		default:break;
	} // End switch.

	  // Process any messages that we did not take care of...
	return (DefWindowProc(_hwnd, _msg, _wparam, _lparam));
}

int WINAPI WinMain(HINSTANCE _hInstance,
	HINSTANCE _hPrevInstance,
	LPSTR _lpCmdLine,
	int _nCmdShow)
{
	WNDCLASSEX winclass; // This will hold the class we create.
	HWND hwnd;           // Generic window handle.
	MSG msg;             // Generic message.

	g_hInstance = _hInstance;
	// First fill in the window class structure.
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = _hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground =
		static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// register the window class
	if (!RegisterClassEx(&winclass))
	{
		return (0);
	}

	g_hMenu = LoadMenu(_hInstance, MAKEINTRESOURCE(IDR_MENU1));
	// create the window
	hwnd = CreateWindowEx(NULL, // Extended style.
		WINDOW_CLASS_NAME,      // Class.
		L"Thread Pool Window",   // Title.
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100,                    // Initial x,y.
		Utils::SCR_WIDTH, Utils::SCR_HEIGHT,                // Initial width, height.
		NULL,                   // Handle to parent.
		g_hMenu,                   // Handle to menu.
		_hInstance,             // Instance of this application.
		NULL);                  // Extra creation parameters.

	if (!(hwnd))
	{
		return (0);
	}

	// Enter main event loop
	while (true)
	{
		// Test if there is a message in queue, if so get it.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Test if this is a quit.
			if (msg.message == WM_QUIT)
			{
				break;
			}

			// Translate any accelerator keys.
			TranslateMessage(&msg);
			// Send the message to the window proc.
			DispatchMessage(&msg);
		}

		// Main game processing goes here.
		GameLoop(); //One frame of game logic occurs here...
	}

	// Return to Windows like this...
	return (static_cast<int>(msg.wParam));
}