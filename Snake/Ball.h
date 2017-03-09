#pragma once

#include "Constants.h"
#include "Snake.h"

#define BALL_WIDTH 32
#define BALL_HEIGHT 32
#define DIAMOND_COUNT 6
#define DIAMOND_ANIMATION_SPRITES 8
#define DIAMOND_WIDTH 32
#define DIAMOND_HEIGHT 32
#define DIAMOND_ROTATE_INTERVAL 200
#define DIAMOND_BACKGROUND_COLOR RGB(51, 51, 51)

extern HWND ball;
extern POINT ballPosition;
extern WCHAR szBallWindowClass[MAX_LOADSTRING];
extern WCHAR szBallTitle[MAX_LOADSTRING];
extern int diamondType;
extern int diamondAnimationSprite;

extern HBITMAP diamondsBitmap;
extern HDC diamondsHdc;

ATOM                RegisterBallClass(HINSTANCE hInstance);
LRESULT CALLBACK    BallWndProc(HWND, UINT, WPARAM, LPARAM);
bool InitBallInstance(HINSTANCE, int);
POINT GetRandomBallPosition();
void CreateNewDiamond();
int GetNextDiamondType();
void PaintDiamond(HWND hWnd);
void ForceDiamondUpdate();
void LoadDiamondBitmap(HDC hdc);