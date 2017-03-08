#pragma once

#include "resource.h"
#define WM_TRAY   (WM_USER + 1)
extern HINSTANCE hInst;
extern NOTIFYICONDATA notifyIconData;
extern unsigned playerPoints;
void PrepareNotificationIconData(HWND hWnd);
void AddIconToSystemTray(HWND hWnd);
void RemoveIconFromSystemTray();
void HandleTray(HWND hWnd, WPARAM wParam, LPARAM lParam);
void ShowContextMenu(HWND hWnd, WPARAM wParam, LPARAM lParam);