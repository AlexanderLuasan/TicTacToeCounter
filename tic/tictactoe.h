#pragma once
#include "constants.h"
#include <iostream>
using namespace std;
struct sateinfo {//the data of results
	int gametype;
	int depth;
};
bool valid(char board[boardsize][boardsize], int size);//check the board for x and o count
sateinfo endstate(char board[boardsize][boardsize], int size);//if this is a valid states
int count(char board[boardsize][boardsize], char side);//total count of symbols