#pragma once

#include "Constants.h"
#include "Snake.h"

#define BALL_WIDTH 20
#define BALL_HEIGHT 20

extern HWND ball;
extern POINT ballPosition;
extern WCHAR szBallWindowClass[MAX_LOADSTRING];
extern WCHAR szBallTitle[MAX_LOADSTRING];

ATOM                RegisterBallClass(HINSTANCE hInstance);
LRESULT CALLBACK    BallWndProc(HWND, UINT, WPARAM, LPARAM);
bool InitBallInstance(HINSTANCE, int);
POINT GetRandomBallPosition();
void PlaceBallInRandomPosition();
