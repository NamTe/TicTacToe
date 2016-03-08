// TicTacToe.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TicTacToe.h"
#include<windowsx.h>


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
const int CELL_SIZE = 100;						// default size of board
int intPlayerTurn = 1;
int arrayGameBoard[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
HICON hiPlayerOne, hiPlayerTwo;
int winner = 0;
int wins[3];

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TICTACTOE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TICTACTOE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TICTACTOE));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TICTACTOE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_TICTACTOE));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


BOOL GetGameBoardRect(HWND hWnd, RECT* rect)
{
	RECT rc;

	if (GetClientRect(hWnd, &rc))
	{
		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;

		rect->left = (width - CELL_SIZE * 3) / 2;
		rect->top = (height - CELL_SIZE * 3) / 2;

		rect->right = rect->left + CELL_SIZE * 3;
		rect->bottom = rect->top + CELL_SIZE * 3;

		return TRUE;
	}

	SetRectEmpty(rect);
	return FALSE;

}


void DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

int GetCellNumberFromPoint(HWND hWnd, int xPos, int yPos)
{
	POINT pt = { xPos, yPos };
	RECT rc;

	if (GetGameBoardRect(hWnd, &rc))
	{
		if (PtInRect(&rc, pt))
		{
			xPos = pt.x - rc.left;
			yPos = pt.y - rc.top;

			int column = xPos / CELL_SIZE;
			int row = yPos / CELL_SIZE;

			return column + row * 3;
		}
	}

	return -1; //outside gameboard or failure
}

BOOL GetCellRect(HWND hWnd, int index, RECT *pRect)
{
	RECT rcBoard;
	SetRectEmpty(pRect);

	if (index < 0 || index > 8)
	{
		return FALSE;
	}

	if (GetGameBoardRect(hWnd, &rcBoard))
	{
		int column = index % 3;
		int row = index / 3;

		pRect->left = rcBoard.left + column * CELL_SIZE + 1;
		pRect->top = rcBoard.top + row * CELL_SIZE + 1;
		pRect->right = pRect->left + CELL_SIZE - 1;
		pRect->bottom = pRect->top + CELL_SIZE - 1;

		return TRUE;
	}

	return FALSE;
}


int GetWinner(int wins[3])
{
	int cells[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 3, 6, 1, 4, 7, 2, 5, 8, 0, 4, 8, 2, 4, 6 };

	for (int i = 0; i < ARRAYSIZE(cells); i += 3)
	{
		//winner
		if ((arrayGameBoard[cells[i]] != 0) && arrayGameBoard[cells[i]] == arrayGameBoard[cells[i + 1]] && arrayGameBoard[cells[i + 1]] == arrayGameBoard[cells[i + 2]]) {
			wins[0] = cells[i];
			wins[1] = cells[i + 1];
			wins[2] = cells[i + 2];

			return arrayGameBoard[cells[i]];
		}
	}

	for (int i = 0; i < ARRAYSIZE(arrayGameBoard); i++)
	{
		//continue
		if (0 == arrayGameBoard[i])
			return 0;
	}

	//draw
	return 3;
}


void ShowTurn(HWND hWnd, HDC hdc)
{
	static const LPWSTR szPlayerOneTurn = L"Turn: Player 1";
	static const LPWSTR szPlayerTwoTurn = L"Turn: Player 2";
	LPWSTR Turn = intPlayerTurn == 1 ? szPlayerOneTurn : szPlayerTwoTurn;
	switch (winner)
	{

	case 0:
		Turn = intPlayerTurn == 1 ? szPlayerOneTurn : szPlayerTwoTurn;
		break;
	case 1 :
		Turn = L"Player 1 is the winner";
		break;
	case 2 : 
		Turn = L"Player 2 is the winner";
		break;
	default:
		Turn = L"it's a draw";
		break;
	}
	RECT rcClient;
	if (GetClientRect(hWnd, &rcClient))
	{
		rcClient.top = rcClient.bottom - 48;
		FillRect(hdc, &rcClient, (HBRUSH)GetStockObject(GRAY_BRUSH));
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 0));
		DrawText(hdc, Turn, lstrlen(Turn), &rcClient, DT_CENTER);
	}
}

BOOL StartNewGame(HWND hWnd)
{
	int id = MessageBox(hWnd, L"Are you sure to start new game?", L"New Game?", MB_YESNO);
	if (id == IDYES)
	{
		intPlayerTurn = winner == 3 ? 1 : winner;
		ZeroMemory(arrayGameBoard, sizeof(arrayGameBoard));
		ZeroMemory(wins, sizeof(wins));
		winner = 0;
		ShowTurn(hWnd, GetDC(hWnd));
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
		return TRUE;
	}
	return FALSE;
}

void DrawIconCentered(HDC hdc, RECT *pRect, HICON hIcon)
{
	const int ICON_SIZE = 32;

	if (NULL != pRect)
	{
		int left = pRect->left + ((pRect->right - pRect->left) - ICON_SIZE) / 2;
		int top = pRect->top + ((pRect->bottom - pRect->top) - ICON_SIZE) / 2;
		DrawIcon(hdc, left, top, hIcon);
	}
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	{

		hiPlayerOne = LoadIcon(hInst, MAKEINTRESOURCE(IDI_X));
		hiPlayerTwo = LoadIcon(hInst, MAKEINTRESOURCE(IDI_O));
	}
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_NEWGAME:
		{
			StartNewGame(hWnd);
		}
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_LBUTTONDOWN:
	{
		if (intPlayerTurn == 0)
		{
			if (!StartNewGame(hWnd))
			{
				break;
			}
		}

		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		int index = GetCellNumberFromPoint(hWnd, xPos, yPos);

		HDC hdc = GetDC(hWnd);
		if (NULL != hdc)
		{
			WCHAR temp[100];

			if (index != -1)
			{
				RECT rc;
				if ((arrayGameBoard[index] == 0) && GetCellRect(hWnd, index, &rc))
				{
					//FillRect(hdc, &rc, (intPlayerTurn == 1) ? hbPlayerOne : hbPlayerTwo);
					DrawIconCentered(hdc, &rc, (intPlayerTurn == 1) ? hiPlayerOne : hiPlayerTwo);
					arrayGameBoard[index] = intPlayerTurn;
					winner = GetWinner(wins);
					if (winner == 1 || winner == 2)
					{
						MessageBox(hWnd, (winner == 1) ? L"Player 1 is the winner" : L"Player 2 is the winner",
							L"You Win", MB_OK | MB_ICONINFORMATION);
						intPlayerTurn = 0;
					}
					else if (winner == 3)
					{
						MessageBox(hWnd, L"it's a draw",
							L"Draw", MB_OK | MB_ICONINFORMATION);
						intPlayerTurn = 0;
					}
					else {
						intPlayerTurn = (intPlayerTurn == 1) ? 2 : 1;
						
					}
					ShowTurn(hWnd, hdc);
				}
			}
			ReleaseDC(hWnd, hdc);
		}
	}
		break;
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* pMin = (MINMAXINFO*)lParam;
		pMin->ptMinTrackSize.x = CELL_SIZE * 5;
		pMin->ptMinTrackSize.y = CELL_SIZE * 5;
	}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		RECT rc;
		if (GetGameBoardRect(hWnd, &rc))
		{
			RECT rcClient;
			if (GetClientRect(hWnd, &rcClient))
			{
				LPWSTR szPlayerOne = L"Player 1";
				LPWSTR szPlayerTwo = L"Player 2";
				SetBkMode(hdc, TRANSPARENT);
				SetTextColor(hdc, RGB(0, 0, 0));
				TextOut(hdc, 10, 10, szPlayerOne, lstrlen(szPlayerOne));
				SetTextColor(hdc, RGB(255, 255, 255));
				TextOut(hdc, rcClient.right - 65, 10, szPlayerTwo, lstrlen(szPlayerTwo));
			}
			FillRect(hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
			//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
		}

		for (int i = 0; i < 4; i++)
		{
			DrawLine(hdc, rc.left + CELL_SIZE * i, rc.top, rc.left + CELL_SIZE * i, rc.bottom);

			DrawLine(hdc, rc.left, rc.top + CELL_SIZE * i, rc.right, rc.top + CELL_SIZE * i);
		}

		RECT rcCell;
		for (int i = 0; i < ARRAYSIZE(arrayGameBoard); i++)
		{
			if ((arrayGameBoard[i] != 0) && GetCellRect(hWnd, i, &rcCell))
			{
				DrawIconCentered(hdc, &rcCell, (arrayGameBoard[i] == 1) ? hiPlayerOne : hiPlayerTwo);
			}
		}
		ShowTurn(hWnd, hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:

		DestroyIcon(hiPlayerOne);
		DestroyIcon(hiPlayerTwo);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
