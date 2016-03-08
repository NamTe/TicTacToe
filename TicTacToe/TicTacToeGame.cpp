#include "stdafx.h"
#include "TicTacToeGame.h"
#include "resource.h"

TicTacToeGame::TicTacToeGame()
{

}


TicTacToeGame::TicTacToeGame(HINSTANCE hInst)
{
	hiPlayerOne = LoadIcon(hInst, MAKEINTRESOURCE(IDI_X));
	hiPlayerTwo = LoadIcon(hInst, MAKEINTRESOURCE(IDI_O));
	ZeroMemory(arrayGameBoard, sizeof(arrayGameBoard));
}


TicTacToeGame::~TicTacToeGame()
{
	DestroyIcon(hiPlayerOne);
	DestroyIcon(hiPlayerTwo);
}



BOOL TicTacToeGame::GetGameBoardRect(HWND hWnd, RECT* rect)
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


void TicTacToeGame::DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

int TicTacToeGame::GetCellNumberFromPoint(HWND hWnd, int xPos, int yPos)
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

BOOL TicTacToeGame::GetCellRect(HWND hWnd, int index, RECT *pRect)
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


int TicTacToeGame::GetWinner(int wins[3])
{
	int cells[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 3, 6, 1, 4, 7, 2, 5, 8, 0, 4, 8, 2, 4, 6 };

	for (int i = 0; i < ARRAYSIZE(cells); i += 3)
	{
		//winner
		if ((arrayGameBoard[cells[i]] != 0) && arrayGameBoard[cells[i]] == arrayGameBoard[cells[i + 1]] &&
			arrayGameBoard[cells[i + 1]] == arrayGameBoard[cells[i + 2]]) {
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


void TicTacToeGame::ShowTurn(HWND hWnd, HDC hdc)
{
	static const LPWSTR szPlayerOneTurn = L"Turn: Player 1";
	static const LPWSTR szPlayerTwoTurn = L"Turn: Player 2";
	LPWSTR Turn = intPlayerTurn == 1 ? szPlayerOneTurn : szPlayerTwoTurn;
	switch (winner)
	{

	case 0:
		Turn = intPlayerTurn == 1 ? szPlayerOneTurn : szPlayerTwoTurn;
		break;
	case 1:
		Turn = L"Player 1 is the winner";
		break;
	case 2:
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

BOOL TicTacToeGame::StartNewGame(HWND hWnd)
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

void TicTacToeGame::DrawIconCentered(HDC hdc, RECT *pRect, HICON hIcon)
{
	const int ICON_SIZE = 32;

	if (NULL != pRect)
	{
		int left = pRect->left + ((pRect->right - pRect->left) - ICON_SIZE) / 2;
		int top = pRect->top + ((pRect->bottom - pRect->top) - ICON_SIZE) / 2;
		DrawIcon(hdc, left, top, hIcon);
	}
}

void TicTacToeGame::ShowWinner(HWND hWnd, HDC hdc)
{
	RECT rcClient;
	for (int i = 0; i < ARRAYSIZE(wins); i++)
	{
		if (GetCellRect(hWnd, wins[i], &rcClient)) {
			FillRect(hdc, &rcClient, CreateSolidBrush(RGB(255, 0, 0)));
			DrawIconCentered(hdc, &rcClient, (winner == 1) ? hiPlayerOne : hiPlayerTwo);
		}
	}

}

BOOL TicTacToeGame::IsContinue() 
{
	return intPlayerTurn != 0;
}

void TicTacToeGame::PlayGame(int index, HWND hWnd, HDC hdc)
{
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
				ShowWinner(hWnd, hdc);
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
}

void TicTacToeGame::DrawGame(HDC hdc, RECT rc, HWND hWnd)
{
	for (int i = 0; i < 4; i++)
	{
		DrawLine(hdc, rc.left + TicTacToeGame::CELL_SIZE * i, rc.top, rc.left + TicTacToeGame::CELL_SIZE * i, rc.bottom);

		DrawLine(hdc, rc.left, rc.top + TicTacToeGame::CELL_SIZE * i, rc.right, rc.top + TicTacToeGame::CELL_SIZE * i);
	}

	RECT rcCell;
	for (int i = 0; i < ARRAYSIZE(arrayGameBoard); i++)
	{
		if ((arrayGameBoard[i] != 0) && GetCellRect(hWnd, i, &rcCell))
		{
			DrawIconCentered(hdc, &rcCell, (arrayGameBoard[i] == 1) ? hiPlayerOne : hiPlayerTwo);
		}
	}
	if (winner != 0 && winner != 3)
	{
		ShowWinner(hWnd, hdc);
	}
	ShowTurn(hWnd, hdc);
}