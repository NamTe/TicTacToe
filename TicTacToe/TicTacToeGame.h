#pragma once
class TicTacToeGame
{
public:
	TicTacToeGame();
	TicTacToeGame(HINSTANCE hInst);
	~TicTacToeGame();
	const static int CELL_SIZE = 100;						// default size of board
	BOOL GetGameBoardRect(HWND hWnd, RECT* rect);
	void DrawLine(HDC hdc, int x1, int y1, int x2, int y2);
	int GetCellNumberFromPoint(HWND hWnd, int xPos, int yPos);
	BOOL GetCellRect(HWND hWnd, int index, RECT *pRect);
	int GetWinner(int wins[3]);
	void ShowTurn(HWND hWnd, HDC hdc);
	BOOL StartNewGame(HWND hWnd);
	void DrawIconCentered(HDC hdc, RECT *pRect, HICON hIcon);
	void ShowWinner(HWND hWnd, HDC hdc);
	BOOL IsContinue();
	void PlayGame(int index, HWND hWnd, HDC hdc);
	void DrawGame(HDC hdc, RECT rc, HWND hWnd);
private:

	int intPlayerTurn = 1;
	int arrayGameBoard[9];
	HICON hiPlayerOne, hiPlayerTwo;
	int winner = 0;
	int wins[3];
};

