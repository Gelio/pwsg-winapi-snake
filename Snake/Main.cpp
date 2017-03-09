#include "stdafx.h"
#include "Main.h"
#include "Ball.h"
#include "Snake.h"

HINSTANCE hInst;
NOTIFYICONDATA notifyIconData;
unsigned playerPoints = 0;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	hInst = hInstance;
	srand(time(NULL));
	if (!InitBallInstance(hInstance, nCmdShow))
		return FALSE;

	if (!InitSnakeInstance(hInstance, nCmdShow))
		return FALSE;

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

void PrepareNotificationIconData(HWND hWnd)
{
	memset(&notifyIconData, 0, sizeof(NOTIFYICONDATA));
	notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	notifyIconData.hWnd = hWnd;
	notifyIconData.uFlags = NIF_ICON | NIF_TIP | NIF_SHOWTIP | NIF_MESSAGE;
	notifyIconData.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SNAKE));
	notifyIconData.uID = 1;
	notifyIconData.uVersion = NOTIFYICON_VERSION_4;
	StringCchCopy(notifyIconData.szTip, ARRAYSIZE(notifyIconData.szTip), L"Snake");
	notifyIconData.uCallbackMessage = WM_APP_TRAY;
}

void AddIconToSystemTray(HWND hWnd)
{
	PrepareNotificationIconData(hWnd);
	Shell_NotifyIcon(NIM_ADD, &notifyIconData);
	Shell_NotifyIcon(NIM_SETVERSION, &notifyIconData);
}

void RemoveIconFromSystemTray()
{
	Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
}

void HandleTray(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(lParam))
	{
	case WM_RBUTTONDOWN:
	case WM_CONTEXTMENU:
		ShowContextMenu(hWnd, wParam, lParam);
		break;
	}
}

void ShowContextMenu(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HMENU menu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
	HMENU menuTrackPopup = GetSubMenu(menu, 0);
	
	WCHAR szScore[MAX_LOADSTRING];
	swprintf(szScore, MAX_LOADSTRING, L"Score: %d", playerPoints);
	ModifyMenu(menuTrackPopup, 0, MF_BYPOSITION | MF_STRING | MF_GRAYED | MF_DISABLED, 0, szScore);

	int mouseX = GET_X_LPARAM(wParam),
		mouseY = GET_Y_LPARAM(wParam);

	TrackPopupMenu(menuTrackPopup, 
		TPM_BOTTOMALIGN | TPM_RIGHTALIGN,
		mouseX, mouseY, 0, hWnd, NULL);
}


