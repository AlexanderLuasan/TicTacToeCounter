#pragma once
#include "constants.h"
#include <iostream>
using namespace std;
struct sateinfo {
	int gametype;
	int depth;
};
bool valid(char board[boardsize][boardsize], int size);
sateinfo endstate(char board[boardsize][boardsize], int size);
int count(char board[boardsize][boardsize], char side);