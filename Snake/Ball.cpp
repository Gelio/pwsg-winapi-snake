#include "stdafx.h"
#include "Ball.h"
#include "Main.h"

HWND ball;
POINT ballPosition;
WCHAR szBallWindowClass[MAX_LOADSTRING];
WCHAR szBallTitle[MAX_LOADSTRING];
int diamondType;
int diamondAnimationSprite;

ATOM RegisterBallClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = BallWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szBallWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

bool InitBallInstance(HINSTANCE hInstance, int nCmdShow)
{
	// Load strings from the string table and register ball class
	LoadStringW(hInstance, IDS_BALL_TITLE, szBallTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_BALL, szBallWindowClass, MAX_LOADSTRING);
	RegisterBallClass(hInstance);

	diamondType = GetNextDiamondType();
	diamondAnimationSprite = 1;

	// Create ball at a random position
	ballPosition = GetRandomBallPosition();
	ball = CreateWindowW(szBallWindowClass, szBallTitle, WS_POPUP | WS_SYSMENU,
		ballPosition.x, ballPosition.y, BALL_WIDTH, BALL_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (!ball)
		return false;

	// Make ball a circle
	/*HRGN region = CreateEllipticRgn(0, 0, BALL_WIDTH, BALL_HEIGHT);
	SetWindowRgn(ball, region, true);*/

	// Set as top-most window
	long style = GetWindowLong(ball, GWL_EXSTYLE);
	style |= WS_EX_TOPMOST;
	style |= WS_EX_TOOLWINDOW;
	SetWindowLong(ball, GWL_EXSTYLE, style);
	SetWindowPos(ball, HWND_TOPMOST, ballPosition.x, ballPosition.y, BALL_WIDTH, BALL_HEIGHT, SWP_SHOWWINDOW);

	UpdateWindow(ball);
	return true;
}

POINT GetRandomBallPosition()
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN),
		screenHeight = GetSystemMetrics(SM_CYSCREEN);
	POINT p;
	p.x = rand() % (screenWidth - BALL_WIDTH);
	p.y = rand() % (screenHeight - BALL_HEIGHT);
	return p;
}

void CreateNewDiamond()
{
	diamondAnimationSprite = 0;
	diamondType = GetNextDiamondType();
	ballPosition = GetRandomBallPosition();
	MoveWindow(ball, ballPosition.x, ballPosition.y, BALL_WIDTH, BALL_HEIGHT, TRUE);
}

int GetNextDiamondType()
{
	return rand() % DIAMOND_COUNT;
}

void PaintDiamond(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	HBITMAP diamonds = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DIAMONDS));

	HDC diamondsHdc = CreateCompatibleDC(hdc);
	SelectObject(diamondsHdc, diamonds);

	int diamondTopLeftX = diamondAnimationSprite * DIAMOND_WIDTH;
	int diamondTopLeftY = diamondType * DIAMOND_HEIGHT;
	BitBlt(hdc, 0, 0, DIAMOND_WIDTH, DIAMOND_HEIGHT, diamondsHdc, diamondTopLeftX, diamondTopLeftY, SRCCOPY);
	diamondAnimationSprite++;
	diamondAnimationSprite %= DIAMOND_ANIMATION_SPRITES;

	DeleteDC(diamondsHdc);
	DeleteObject(diamonds);
	DeleteDC(hdc);
	

	EndPaint(hWnd, &ps);
}

void ForceDiamondUpdate()
{
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.bottom = BALL_HEIGHT;
	rect.right = BALL_WIDTH;
	InvalidateRect(ball, &rect, TRUE);
}

LRESULT CALLBACK BallWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		RegisterHotKey(hWnd, 100, NULL, VK_SPACE);
		SetTimer(hWnd, WM_APP_MOVE_SNAKE, MOVE_INTERVAL, NULL);
		SetTimer(hWnd, WM_APP_ROTATE_DIAMOND, DIAMOND_ROTATE_INTERVAL, NULL);
		AddIconToSystemTray(hWnd);
		break;
	case WM_APP_TRAY:
		HandleTray(hWnd, wParam, lParam);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case WM_APP_MOVE_SNAKE:
			SnakeMakeMove();
			break;
		case WM_APP_ROTATE_DIAMOND:
			ForceDiamondUpdate();
			break;
		}
		break;

	case WM_HOTKEY:
		if (HIWORD(lParam) == VK_SPACE)
			AddNewHeadSegment();
		break;

	case WM_KEYDOWN:
		HandleSnakeDirectionChange(wParam);
		break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_SCORE_EXIT:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
		PaintDiamond(hWnd);
		break;
	case WM_DESTROY:
		RemoveIconFromSystemTray();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

