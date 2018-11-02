#include "tictactoe.h"

bool valid(char board[boardsize][boardsize], int size) {
	int numofx = 0, numofo = 0;
	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {
			if (board[r][c] == 'x') {
				numofx++;
			}
			else if (board[r][c] == 'o') {
				numofo++;
			}
		}
	}
	if (numofx == numofo || ((numofx - numofo)) == 1) {
		return true;
	}
	else return false;
}

sateinfo endstate(char board[boardsize][boardsize], int size) {
	int wincol = 0, winrow = 0, winld = 0, winrd = 0;
	bool victory;
	char initial;
	bool full=true;

	int pos = 0;
	char wins[9];

	for (int r = 0; r < size; r++) {
		for (int c = 0; c < size; c++) {
			if (board[r][c] == 'e') {
				full = false;
				break;
			}
		}
	}
	for (int c = 0; c < size; c++) {
		if (board[0][c] != 'e') {
			initial = board[0][c];
			victory = true;
			for (int r = 0; r < size; r++) {
				if (board[r][c] != initial) {
					victory = false;
				}
			}
			if (victory) {
				wins[pos] = initial;
				pos += 1;
				wincol += 1;
			}
		}
	}
	for (int r = 0; r < size; r++) {
		if (board[r][0] != 'e') {
			initial = board[r][0];
			victory = true;
			for (int c = 0; c < size; c++) {
				if (board[r][c] != initial) {
					victory = false;
				}
			}
			if (victory) {
				wins[pos] = initial;
				pos += 1;
				winrow += 1;
			}
		}
	}

	if (board[0][0] != 'e') {
		initial = board[0][0];
		victory = true;
		for (int r = 0; r < size; r++) {
			if (board[r][r] != initial) {
				victory = false;
				break;
			}
		}
		if (victory) {
			wins[pos] = initial;
			pos += 1;
			winld += 1;
		}
	}
	if (board[0][size - 1] != 'e') {
		initial = board[0][size - 1];
		victory = true;
		for (int r = 0; r < size; r++) {
			
			if (board[r][size - 1 - r] != initial) {
				victory = false;
				break;
			}
		}
		if (victory) {
			wins[pos] = initial;
			pos += 1;
			winrd += 1;
		}
	}
	sateinfo end;
	
	if (((winrd < 2) && (winld < 2) && (wincol < 2)&&(winrow<2))) {
		if ((winrd + winld + wincol + winrow)==1) {
			if (wins[0]=='x') {
				if (count(board, 'x') == count(board, 'o')+1) {//is os turn x made last move
					end.gametype = 1;
				}
				else {
					end.gametype = 4; //invalid
				}
			}
			else if (wins[0] == 'o') {
				if (count(board, 'o') == count(board, 'x')) {//is x's turn o made last move
					end.gametype = 1;
				}
				else {
					end.gametype = 4; //invalid
				}
			}
		}
		else if((winrd + winld + wincol + winrow) == 2){
			if (boardsize % 2 == 0) {
				if (winrd == 1 && winld == 1) {
					end.gametype = 4;
				}
				else {
					end.gametype = 1;
				}
			}
			else {//three p
				end.gametype = 1;
			}

		}
		else if ((winrd + winld + wincol + winrow) == 3) {
			cout << "help" << endl;
			system("pause");
			end.gametype = 4;
		}
		else if (full) {//game is stale
			end.gametype = 2;
		}
		else {
			end.gametype = 3;//game is not yet stale or victory
		}
	}
	else {
		end.gametype = 4;//not possible to reach more than 1 wins non overlaping
	}
	if (end.gametype != 4) {
		end.depth = count(board, 'o');
		end.depth += count(board, 'x');
	}
	return end;
}

int count(char board[boardsize][boardsize], char side)
{
	int end = 0;
	for (int c = 0; c < boardsize; c++) {
		for (int r = 0; r < boardsize; r++) {
			if (side == board[r][c]) {
				end += 1;
			}
		}
	}
	return end;
}
