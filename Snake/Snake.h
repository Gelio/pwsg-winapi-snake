#pragma once

#include "Constants.h"
#include "Ball.h"

#define SNAKE_WIDTH 20
#define SNAKE_HEIGHT 20
#define SNAKE_ROUNDED_CORNER 5

#define STARTING_SNAKE_X 100
#define STARTING_SNAKE_Y 100
#define STARTING_SNAKE_SEGMENTS 3
#define MAX_SNAKE_SEGMENTS 100
#define SNAKE_TRANSPARENCY 0.4
#define MOVE_INTERVAL 300

struct snakePart
{
	HWND hWnd;
	snakePart *next;
};
extern snakePart *head, *tail;
enum Direction
{
	DIRECTION_UP = 0,
	DIRECTION_RIGHT = 1,
	DIRECTION_DOWN = 2,
	DIRECTION_LEFT = 3
};

extern WCHAR szSnakeTitle[MAX_LOADSTRING];
extern WCHAR szSnakeWindowClass[MAX_LOADSTRING];
extern int snakeSegments;
extern Direction snakeDirection;

ATOM                RegisterSnakeClass(HINSTANCE hInstance);
LRESULT CALLBACK    SnakeWndProc(HWND, UINT, WPARAM, LPARAM);


bool AddNewSnakeSegment(int x, int y);
POINT GetNextSnakeHeadPosition();
bool AddNewHeadSegment();
void SnakeMakeMove();
bool InitSnakeInstance(HINSTANCE hInstance, int);
void HandleSnakeDirectionChange(WPARAM wParam);
void MoveLastSnakeSegmentToFront(POINT nextHeadPosition);