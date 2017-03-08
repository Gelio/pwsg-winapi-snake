#include "stdafx.h"
#include "Snake.h"
#include "Main.h"

snakePart *head, *tail;
WCHAR szSnakeTitle[MAX_LOADSTRING];
WCHAR szSnakeWindowClass[MAX_LOADSTRING];
int snakeSegments = 0;
// 0 - up, 1 - right, 2 - down, 3 - left
Direction snakeDirection = DIRECTION_RIGHT;

ATOM RegisterSnakeClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = SnakeWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(163, 73, 164));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szSnakeWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

bool AddNewSnakeSegment(int x, int y)
{
	if (snakeSegments >= MAX_SNAKE_SEGMENTS)
		return false;

	snakePart *previousHead = head;
	head = new snakePart;
	if (!tail)
		tail = head;
	if (previousHead)
		previousHead->next = head;
	head->hWnd = CreateWindowW(szSnakeWindowClass, szSnakeTitle, WS_POPUP,
		x, y, SNAKE_WIDTH, SNAKE_HEIGHT, nullptr, nullptr, hInst, nullptr);

	if (!head->hWnd)
		return false;

	snakeSegments++;
	long style = GetWindowLong(head->hWnd, GWL_EXSTYLE);
	style |= WS_EX_LAYERED;
	style |= WS_EX_TOOLWINDOW;
	style |= WS_EX_TOPMOST;

	SetWindowLong(head->hWnd, GWL_EXSTYLE, style);

	SetLayeredWindowAttributes(head->hWnd, 0, SNAKE_TRANSPARENCY * 255, LWA_ALPHA);
	SetWindowPos(head->hWnd, HWND_TOPMOST, x, y, SNAKE_WIDTH, SNAKE_HEIGHT, SWP_SHOWWINDOW);

	HRGN region = CreateRoundRectRgn(0, 0, SNAKE_WIDTH, SNAKE_HEIGHT, SNAKE_ROUNDED_CORNER, SNAKE_ROUNDED_CORNER);
	SetWindowRgn(head->hWnd, region, true);

	ShowWindow(head->hWnd, SW_SHOW);
	UpdateWindow(head->hWnd);
	return true;
}

POINT GetNextSnakeHeadPosition()
{
	snakePart *previousHead = head;
	RECT previousHeadRect;
	GetWindowRect(previousHead->hWnd, &previousHeadRect);

	int screenWidth = GetSystemMetrics(SM_CXSCREEN),
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Select position for the next segment
	POINT nextHeadTopLeftPoint;
	if (snakeDirection == DIRECTION_UP)
	{
		nextHeadTopLeftPoint.x = previousHeadRect.left;
		nextHeadTopLeftPoint.y = previousHeadRect.top - SNAKE_HEIGHT;
	}
	else if (snakeDirection == DIRECTION_RIGHT)
	{
		nextHeadTopLeftPoint.x = previousHeadRect.right;
		nextHeadTopLeftPoint.y = previousHeadRect.top;
	}
	else if (snakeDirection == DIRECTION_DOWN)
	{
		nextHeadTopLeftPoint.x = previousHeadRect.left;
		nextHeadTopLeftPoint.y = previousHeadRect.bottom;
	}
	else if (snakeDirection == DIRECTION_LEFT)
	{
		nextHeadTopLeftPoint.x = previousHeadRect.left - SNAKE_WIDTH;
		nextHeadTopLeftPoint.y = previousHeadRect.top;
	}

	// Check if the head should be outside the screen and adjust position
	if (nextHeadTopLeftPoint.x < 0)
		nextHeadTopLeftPoint.x = screenWidth - SNAKE_WIDTH;
	if (nextHeadTopLeftPoint.x > screenWidth - SNAKE_WIDTH)
		nextHeadTopLeftPoint.x = 0;

	if (nextHeadTopLeftPoint.y < 0)
		nextHeadTopLeftPoint.y = screenHeight - SNAKE_HEIGHT;
	if (nextHeadTopLeftPoint.y > screenHeight - SNAKE_HEIGHT)
		nextHeadTopLeftPoint.y = 0;

	return nextHeadTopLeftPoint;
}

bool AddNewHeadSegment()
{
	POINT nextHeadPoint = GetNextSnakeHeadPosition();
	return AddNewSnakeSegment(nextHeadPoint.x, nextHeadPoint.y);
}

void SnakeMakeMove()
{
	POINT nextHeadPosition = GetNextSnakeHeadPosition();
	RECT nextHeadRect;
	nextHeadRect.left = nextHeadPosition.x;
	nextHeadRect.right = nextHeadPosition.x + SNAKE_WIDTH;
	nextHeadRect.top = nextHeadPosition.y;
	nextHeadRect.bottom = nextHeadPosition.y + SNAKE_HEIGHT;

	RECT ballRect;
	ballRect.left = ballPosition.x;
	ballRect.right = ballPosition.x + BALL_WIDTH;
	ballRect.top = ballPosition.y;
	ballRect.bottom = ballPosition.y + BALL_HEIGHT;

	RECT a;
	BOOL ballCaught = IntersectRect(&a, &nextHeadRect, &ballRect);
	if (ballCaught)
	{
		AddNewHeadSegment();
		PlaceBallInRandomPosition();
	}
	else
		MoveLastSnakeSegmentToFront(nextHeadPosition);
}

void MoveLastSnakeSegmentToFront(POINT nextHeadPosition)
{
	snakePart *prevTail = tail;
	tail = tail->next;
	MoveWindow(prevTail->hWnd, nextHeadPosition.x, nextHeadPosition.y, SNAKE_WIDTH, SNAKE_HEIGHT, TRUE);
	head->next = prevTail;
	head = head->next;
}

LRESULT CALLBACK SnakeWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN:
		HandleSnakeDirectionChange(wParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool InitSnakeInstance(HINSTANCE hInstance, int nCmdShow)
{
	LoadStringW(hInstance, IDS_SNAKE_TITLE, szSnakeTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SNAKE, szSnakeWindowClass, MAX_LOADSTRING);

	RegisterSnakeClass(hInstance);
	head = tail = nullptr;

	bool snakeSegmentAdded;
	for (int i = 0; i < STARTING_SNAKE_SEGMENTS; i++)
	{
		snakeSegmentAdded = AddNewSnakeSegment(STARTING_SNAKE_X + i * SNAKE_WIDTH, STARTING_SNAKE_Y);
		if (!snakeSegmentAdded)
			return false;
	}

	return true;
}

void HandleSnakeDirectionChange(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_UP:
		snakeDirection = DIRECTION_UP;
		break;

	case VK_RIGHT:
		snakeDirection = DIRECTION_RIGHT;
		break;

	case VK_DOWN:
		snakeDirection = DIRECTION_DOWN;
		break;

	case VK_LEFT:
		snakeDirection = DIRECTION_LEFT;
		break;
	}
}