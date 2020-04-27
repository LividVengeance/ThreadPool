/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2019 Media Design School

  File Name   :   [Filename]
  Description :   [One line description of what is the file is for]
  Author      :   [Your name]
  Mail        :   [your.name]@mediadesign.school.nz
********************/

#include <iostream>
#include <windows.h>   // Include all the windows headers.
#include <windowsx.h>  // Include useful macros.
#include <math.h>
#include <time.h>

#define WIN32_LEAN_AND_MEAN

#include "resource.h"
#include "ThreadPool.h"
#include "WorkQueue.h"
#include "Task.h"

#define WINDOW_CLASS_NAME L"WINCLASS1"

//Global variables
HINSTANCE g_hInstance;
HMENU g_hMenu;

void GameLoop()
{
	//One frame of game logic occurs here...
}

LRESULT CALLBACK WindowProc(HWND _hwnd,
	UINT _msg,
	WPARAM _wparam,
	LPARAM _lparam)
{
	// This is the main message handler of the system.
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
			MessageBox(_hwnd, L"This paint tool was developed by Andrew Barnes", L"Author Information", MB_OK | MB_ICONINFORMATION);
			break;
		}
		default:
			break;
		}
		return(0);
	}
	break;
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
		1500, 800,                // Initial width, height.
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

int mainFunc()
{
	srand((unsigned int)time(0));
	const int kiTOTALITEMS = 20;
	//Create a ThreadPool Object capable of holding as many threads as the number of cores
	ThreadPool& threadPool = ThreadPool::GetInstance();
	//Initialize the pool
	threadPool.Initialize();
	threadPool.Start();
	// The main thread writes items to the WorkQueue
	for (int i = 0; i < kiTOTALITEMS; i++)
	{
		threadPool.Submit(CTask(i));
		std::cout << "Main Thread wrote item " << i << " to the Work Queue " << std::endl;
		//Sleep for some random time to simulate delay in arrival of work items
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1001));
	}
	if (threadPool.getItemsProcessed() == kiTOTALITEMS)
	{
		threadPool.Stop();
	}

	threadPool.DestroyInstance();

	int iTemp;
	std::cin >> iTemp;

	return 0;
}
