#include "kpCommon.h"
#include "kpEngine.h"

/*
 * Some info for the window creation
 */
static const LPCTSTR WIN_CLASS = "TomodachiMonsters";
static const LPCTSTR WIN_TITLE = "Tomodachi Monsters";

/*
 * The actual engine itself
 */
static struct kpEngine *engineInstance;

/*
 * For handling window events as well as input
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		engineInstance->running = false;
		break;

	case WM_KEYDOWN:
		engineInstance->keys[wParam] = true;
		break;

	case WM_KEYUP:
		engineInstance->keys[wParam] = false;
		break;

	case WM_CREATE:
	case WM_KILLFOCUS:
		ZeroMemory(engineInstance->keys, sizeof engineInstance->keys);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*
 * The main function
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/* Initialize the engine itself, check for errors */
	enum kpStatus engineError = KP_UNKNOWN_ERROR;
	engineInstance = kpCreateEngine(&engineError);

	if (engineError != KP_SUCCESS)
		kpLogError(kpParseError(engineError));
	else
		engineInstance->running = true;

	kpLogInfo("Engine initialized.");

	/* Setup Win32 window class */
	WNDCLASS wndClass = { 0 };
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wndClass.lpszClassName = WIN_CLASS;

	/* Suppose this fails, show an error */
	if (!RegisterClass(&wndClass))
		kpLogError(kpParseError(KP_WIN32_REGISTRATION_FAILURE));

	/* Define our window style, mustn't be resizable */
	DWORD dwStyle = WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);

	/* Adjust the window dimensions */
	RECT fitRect = { 0, 0, WIN_W, WIN_H };
	AdjustWindowRect(&fitRect, dwStyle, FALSE);

	/* Create window */
	HWND hWnd = CreateWindow(WIN_CLASS, WIN_TITLE, dwStyle,
							 (GetSystemMetrics(SM_CXSCREEN) - WIN_W) / 2,
							 (GetSystemMetrics(SM_CYSCREEN) - WIN_H) / 2,
							 fitRect.right - fitRect.left,
							 fitRect.bottom - fitRect.top,
							 NULL, NULL, hInstance, 0);

	/* Suppose this fails, show an error */
	if (!hWnd)
		kpLogError(kpParseError(KP_WIN32_CREATION_FAILURE));

	/* Define our bitmap info */
	BITMAPINFO bitMapInfo = { 0 };
	bitMapInfo.bmiHeader.biSize = sizeof(bitMapInfo.bmiHeader);
	bitMapInfo.bmiHeader.biWidth = BMP_W;
	bitMapInfo.bmiHeader.biHeight = -BMP_H;
	bitMapInfo.bmiHeader.biPlanes = 1;
	bitMapInfo.bmiHeader.biBitCount = 32;
	bitMapInfo.bmiHeader.biCompression = BI_RGB;

	/* Retrieve the device context */
	HDC devCon = GetDC(hWnd);

	/* Display window */
	ShowWindow(hWnd, nCmdShow);

	/* Game loop */
	MSG msg;
	DWORD next = GetTickCount();
	int loops;
	while (engineInstance->running)
	{
		/* A simple Win32 "message loop" */
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		/* Account for frame skips; Update the game */
		loops = 0;
		while (GetTickCount() > next && loops < 20)
		{
			kpEngineTick(engineInstance);

			next += 1000 / 60;
			loops++;
		}

		/* Render the game */
		kpEngineRender(engineInstance);

		/* Draw the buffer to the window */
		StretchDIBits(devCon, 0, 0, WIN_W, WIN_H, 0, 0, BMP_W, BMP_H, engineInstance->bmp->px, &bitMapInfo, DIB_RGB_COLORS, SRCCOPY);
	}

	/* Delete our device context */
	DeleteDC(devCon);

	/* Once everything is complete, clean up the engine */
	kpDestroyEngine(engineInstance);

	/* Close up the error/info log */
	kpLogInfo("All done, bye!");
	kpDestroyLogger();

	return EXIT_SUCCESS;
}
