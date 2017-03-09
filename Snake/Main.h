#pragma once

#include "resource.h"
#define WM_APP_TRAY				(WM_USER + 1)
#define WM_APP_ROTATE_DIAMOND   (WM_USER + 2)
#define WM_APP_MOVE_SNAKE		(WM_USER + 3)
extern HINSTANCE hInst;
extern NOTIFYICONDATA notifyIconData;
extern unsigned playerPoints;
void PrepareNotificationIconData(HWND hWnd);
void AddIconToSystemTray(HWND hWnd);
void RemoveIconFromSystemTray();
void HandleTray(HWND hWnd, WPARAM wParam, LPARAM lParam);
void ShowContextMenu(HWND hWnd, WPARAM wParam, LPARAM lParam);