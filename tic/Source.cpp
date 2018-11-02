#include <future>
#include <thread>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "tictactoe.h"
#include "constants.h"

using namespace std;
int pow3(int n);
struct pass { // need topass array by value
	bool overflow;
	char board[boardsize][boardsize];
	
};

struct statecount {//a return value for the threads
	int ones;
	int twos;
	int threes;
	int totalendstates;
	int totalvalidstates;
	int nodecount[boardsize*boardsize+1];
	int nodeendcount[boardsize*boardsize+1];
};

sateinfo test(pass game,int fun);//function test if the endstate is valid
bool validator(pass form);
bool comparearray(char arr1[boardsize][boardsize], char arr2[boardsize][boardsize]);//sees if the board states are equal
char add(char n);//e->x x->o o->c  used to icrement the arrays
pass tonextvalid(pass inital, pass lim); //runs to find the next vaild array state
pass generatenextn(pass inital,int numbertoadd); //runs to find next array state no valid check
statecount runsequence(pass start, pass end);//the main threaded function





int main() {
	//testing area from check functions in tic tac toe
	while (false) {
		char board[boardsize][boardsize];
		for (int i = 0; i < boardsize; i++) {
			for (int j = 0; j < boardsize; j++) {
				cin >> board[i][j];
			}
		}
		int isvalid = valid(board, boardsize);
		if (isvalid) {
			sateinfo isend = endstate(board, boardsize);
			if (isend.gametype!=4) {
				cout << "valid" << endl;
				if (isend.gametype == 1) {
					cout << "winner" << endl;
				}
				else if (isend.gametype ==2) {
					cout << "stale mate" << endl;
				}
				else if (isend.gametype == 3) {
					cout << "unfinished" << endl;
				}
				cout << "nodes "<<isend.depth << endl;
			}
			else {
				cout << "illegal end" << endl;
			}
		}
		else {
			cout << "not valid" << endl;
		}
		system("pause");
	}


	char counter[boardsize][boardsize]; //board array used in intialization
	for (int i = 0; i < boardsize*boardsize; i++) {
		counter[i%boardsize][i/ boardsize] = 'e'; //fill with 'e'
	}
	
	bool done = true;
	int buffsize = pow3(buffertribits);//what is the size of the buffer 3^number of spaces
	cout << buffsize << endl;
	pass initalbuff[3*3*3*3*3*3*3*3*3+9]; // array that can handle a buff of 9 with a bit extra


	int totalendstates = 0;
	int totalvalidstates = 0;
	int ones=0;
	int twos=0;
	int threes=0;
	int nodecount[boardsize*boardsize+1];
	int nodeendcount[boardsize*boardsize+1];
	for (int n = 0; n < boardsize*boardsize+1; n++) {
		nodecount[n] = 0;
		nodeendcount[n] = 0;
	}
	future<statecount> threads[activethreads];

	//intialize the buffer

	for (int i = 0; i < buffsize; i++) {
		initalbuff[i].overflow = false;
		for (int ii = 0; ii < boardsize*boardsize; ii++) {
			initalbuff[i].board[ii % boardsize][ii / boardsize] = counter[ii % boardsize][ii / boardsize];
			
		}
		counter[(boardsize*boardsize -buffertribits)% boardsize][(boardsize*boardsize - buffertribits) / boardsize] = add(counter[(boardsize*boardsize - buffertribits) % boardsize][(boardsize*boardsize - buffertribits) / boardsize]);
		for (int ii = boardsize * boardsize - (buffertribits); ii < boardsize*boardsize; ii++) {
			if (counter[ii % boardsize][ii / boardsize] == 'c') {
				if (ii == boardsize * boardsize - 1) {
					counter[ii % boardsize][ii / boardsize] = 'e';
					cout << "overflow" << endl;
					break;
				}
				counter[ii % boardsize][ii / boardsize] = 'e';
				counter[(ii + 1) % boardsize][(ii + 1) / boardsize] = add(counter[(ii + 1) % boardsize][(ii + 1) / boardsize]);
				continue;
			}
			break;
		}
	}
	for (int ii = 0; ii < boardsize*boardsize; ii++) {
		initalbuff[buffsize+1].board[ii % boardsize][ii / boardsize] = counter[ii % boardsize][ii / boardsize];
	}

	//pinrt intalvalues
	for (int ii = 0; ii < buffsize; ii++) {
		for (int i = 0; i < boardsize*boardsize; i++) {
			cout << initalbuff[ii].board[i % boardsize][i / boardsize];
		}
		cout << endl;
	}
	
	//init done;


	system("pause");
	int head = 0;
	time_t starttime = time(0);
	time_t now = time(0);
	time_t elaps = (now - starttime);
	int timeleft = (double(elaps) / head * (buffsize - double(head)));
	int stepsize = activethreads;
	
	for (int i = 0; i < activethreads; i++) {
		threads[i] = async(&runsequence, initalbuff[head], initalbuff[head + 1]);
		head += 1;
	}


	now = time(0);
	elaps = (now - starttime);
	bool solving = true;
	while (solving) {
		for (int i = 0; i < activethreads; i++) {
			future_status status;
			if (threads[i].valid()) {
				status = threads[i].wait_for(0ms);
				if (status == future_status::ready) {//get info
					statecount temp = threads[i].get();
					totalendstates += temp.totalendstates;
					totalvalidstates += temp.totalvalidstates;
					ones += temp.ones;
					twos += temp.twos;
					threes += temp.threes;
					for (int n = 0; n < boardsize*boardsize+1; n++) {
						nodecount[n]+=temp.nodecount[n];
						nodeendcount[n] += temp.nodeendcount[n];
					}

					if (true) {//setup next set
						threads[i] = async(&runsequence, initalbuff[head], initalbuff[head + 1]);
						head += 1;
						if (head%activethreads == 0) {//update the progress
							now = time(0);
							elaps = (now - starttime);
							timeleft = (double(elaps) /head * (buffsize-double(head)));
							/*
							cout << fixed;
							cout << setprecision(3);
							cout << "\r" << "progress: " << double(head) * 100 / buffsize << "% ";
							cout << setprecision(6);
							cout << timeleft;
							cout << "    ";
							cout << flush;
							*/
						}
						if (head > buffsize -1) {
							solving = false;
							break;
						}
					}
				}
			}

		}
	}
	cout << endl;
	for (int i = 0; i < activethreads; i++) {
		if (threads[i].valid()) {
			threads[i].wait();
			statecount temp = threads[i].get();
			totalendstates += temp.totalendstates;
			totalvalidstates += temp.totalvalidstates;
			ones += temp.ones;
			twos += temp.twos;
			threes += temp.threes;
			for (int n = 0; n < boardsize*boardsize+1; n++) {
				nodecount[n] += temp.nodecount[n];
				nodeendcount[n] += temp.nodeendcount[n];
			}
			now = time(0);
			elaps = (now - starttime);
			timeleft = (double(elaps) / head * (buffsize - double(head)));
			/*
			cout << fixed;
			cout << setprecision(3);
			cout << "\r" << "progress: " << double(head) * 100 / buffsize << "% ";
			cout << setprecision(6);
			cout << timeleft;
			cout << "    ";
			cout << flush;
			*/
		}
	}

	cout << endl;
	now = time(0);
	elaps = (now - starttime);
	cout << endl;
	cout << "node list" << endl;
	cout << "Node depth" << "\t" << "Node count" << "\t" << "Terminal nodes" << endl;
	for (int n = 0; n < boardsize*boardsize+1; n++) {
		
		cout <<n <<"\t\t";
		cout << nodecount[n];
		cout << "\t\t"<<nodeendcount[n] << endl;
	}
	cout << "Victories: " << ones << "\tStalemates: " << twos << "\tIncomplete: " << threes << endl;
	cout << "Elapsed time: " << elaps << "sec" << endl;
	cout <<"Total end states: " << totalendstates << endl;
	cout << "Total valid states: " << totalvalidstates << endl;
	system("pause");
}

int pow3(int n)
{
	int end = 1;
	for (int i = 0; i < n; i++) {
		end *= 3;
	}
	return end;
}

sateinfo test(pass game, int fun)
{
	sateinfo btype = endstate(game.board, boardsize);
	return btype;
}

bool comparearray(char arr1[boardsize][boardsize], char arr2[boardsize][boardsize])
{
	bool end = true;
	for (int i = 0; i < boardsize;i++) {
		for (int ii = 0; ii < boardsize; ii++) {
			if (arr1[i][ii] != arr2[i][ii]) {
				end = false;
				break;
			}
		}
	}
	return end;
}

char add(char n)
{
	switch (n) {
	case 'e':
		return 'x';
	case 'x':
		return 'o';
	case 'o':
		return 'c';
	}
}

pass tonextvalid(pass inital, pass lim)
{
	do {
		inital = generatenextn(inital, 1);
		if (inital.overflow) {
			return lim;
		}
		if (comparearray(inital.board, lim.board)) {
			return lim;
		}
	} while (!validator(inital));
	return inital;
}

pass generatenextn(pass inital, int numbertoadd)
{
	for (int i = 0; i < numbertoadd; i++) {
		inital.board[0][0] = add(inital.board[0][0]);
		for (int i = 0; i < boardsize * boardsize; i++) {
			if (inital.board[i % boardsize][i / boardsize] == 'c') {
				if (i == boardsize * boardsize - 1) {
					inital.board[i % boardsize][i / boardsize] = 'e';
					inital.overflow = true;
					break;
				}
				inital.board[i % boardsize][i / boardsize] = 'e';
				inital.board[(i + 1) % boardsize][(i + 1) / boardsize] = add(inital.board[(i + 1) % boardsize][(i + 1) / boardsize]);
				continue;
			}
			break;
		}
	}
	return inital;
}

statecount runsequence(pass start, pass end)
{
	/* show inital starting point
	cout << "initalpass: ";
	for (int j = 0; j < boardsize*boardsize; j++) {
		cout << start.board[j % boardsize][j / boardsize];
	}
	*/
	int search = 0;
	int tempnum=0;
 
	bool ready = false;
	pass buffer = start;
	statecount final;
	final.totalendstates = 0;
	final.totalvalidstates = 0;
	final.ones =0;
	final.twos = 0;
	final.threes = 0;
	for (int n = 0; n < boardsize*boardsize+1; n++) {
		final.nodecount[n] =0;
		final.nodeendcount[n] =0;
	}
	future<sateinfo> solvethread[threadscount];
	future<pass> sequencefinder;
	
	if (validator(buffer)) {//first pos is valid
		//cout << "valid" << endl;
		sequencefinder = async(&tonextvalid, buffer, end);
		solvethread[0] = async(&test, buffer, boardsize);
	}
	else {
		//cout << "invalid" << endl;
		sequencefinder = async(&tonextvalid, buffer, end);
		sequencefinder.wait();
		buffer = sequencefinder.get();
		sequencefinder = async(&tonextvalid, buffer, end);
		solvethread[0] = async(&test, buffer, boardsize);
	}
	/*    diplay the result of the first test
	cout << "first test: ";
	for (int j = 0; j < boardsize*boardsize; j++) {
		cout << buffer.board[j % boardsize][j / boardsize];
	}
	int gametype = test(buffer, boardsize);
	if (gametype == 1) {//valid win
		cout << " valid win" << endl;
	}
	else if (gametype == 2) {//valid stale
		cout << " valid stale" << endl;
	}
	else if (gametype == 3) {//unfished but valid
		cout << " unfinished" << endl;
	}
	else {
		cout << "invalid" << endl;
	}
	
	*/

	ready = false;
	bool done = false;
	bool doneadd = false;
	while (!done) {
		if (ready == true) {
			if (validator(buffer)) {//check valid condition;

				ready = false;
				bool assigning = true;
				while (assigning) {//find a thread to take care of it
					for (int t = 0; t < threadscount; t++) {
						future_status status;
						if (solvethread[t].valid()) {
							
							status = solvethread[t].wait_for(0ms);
							if (status == future_status::ready) {
								sateinfo gametype = solvethread[t].get();
								if (gametype.gametype ==	1) {//valid win
									final.totalendstates += 1;
									final.totalvalidstates += 1;
									final.ones += 1;
									final.nodeendcount[gametype.depth] += 1;
									final.nodecount[gametype.depth] += 1;
								}
								else if (gametype.gametype == 2) {//valid stale
									final.totalendstates += 1;
									final.totalvalidstates += 1;
									final.twos += 1;
									final.nodeendcount[gametype.depth] += 1;
								}
								else if (gametype.gametype == 3) {//unfished but valid
									final.totalvalidstates += 1;
									final.threes += 1;
									final.nodecount[gametype.depth] += 1;

								}

								assigning = false;
								solvethread[t] = async(&test, buffer, boardsize);
								
								break;
							}
						}
					}
				}
			}
			else {
				ready = false;
				cout << "invalid in buffer" << endl;
			}
		}
		else if (ready == false&&doneadd==false) {//buffer is not ready so find a thread to increment
			future_status status;
			if (sequencefinder.valid()) {
				status = sequencefinder.wait_for(0ms);
				if (status == future_status::ready) {
					buffer = sequencefinder.get();
					
					/* print the data of a test
					cout << "a test: ";
					for (int j = 0; j < boardsize*boardsize; j++) {
						cout << buffer.board[j % boardsize][j / boardsize];
					}
					*/

					ready = true;
					if (comparearray(buffer.board, end.board)) {
						doneadd = true;
						done = true;
						//cout << " end?" << endl;
					}	
					else {
						/* print result of tests
						int gametype = test(buffer, boardsize);
						if (gametype == 1) {//valid win
							cout << " valid win" << endl;
						}
						else if (gametype == 2) {//valid stale
							cout << " valid stale" << endl;
						}
						else if (gametype == 3) {//unfished but valid
							cout << " unfinished" << endl;
						}
						else {
							cout << "invalid" << endl;
						}
						*/
						sequencefinder = async(&tonextvalid, buffer, end);
					}
				}
			}

		}
		else {
		}
	}

	for (int t = 0; t < threadscount; t++) {
		if (solvethread[t].valid()) {
			solvethread[t].wait();
			sateinfo gametype = solvethread[t].get();
			if (gametype.gametype < 2) {//valid win
				final.totalendstates += 1;
				final.totalvalidstates += 1;
				final.ones += 1;
				final.nodeendcount[gametype.depth] += 1;
				final.nodecount[gametype.depth] += 1;
			}
			else if (gametype.gametype == 2) {//valid stale
				final.totalendstates += 1;
				final.totalvalidstates += 1;
				final.twos += 1;
				final.nodeendcount[gametype.depth] += 1;
				final.nodecount[gametype.depth] += 1;
			}
			else if (gametype.gametype == 3) {//unfished but valid
				final.totalvalidstates += 1;
				final.threes += 1;

				final.nodecount[gametype.depth] += 1;
			}
		}
	}
	//cout << tempnum << endl;
	return final;
}

bool validator(pass form)
{
	bool end = false;
	end = valid(form.board, boardsize);
	return end;
}


