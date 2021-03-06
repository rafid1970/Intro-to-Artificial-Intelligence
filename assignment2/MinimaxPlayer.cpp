/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 */
#include <iostream>
#include <assert.h>
#include "MinimaxPlayer.h"

using std::vector;

// A couple references for planning out various implementation methods:
// http://stackoverflow.com/questions/6887838/improving-minimax-algorithm
// http://mnemstudio.org/game-reversi-example-2.htm
// https://github.com/bekoeppel/Lynx-Reversi-Player/blob/master/miniMaxPlayer/MiniMaxPlayer.java

MinimaxPlayer::MinimaxPlayer(char symb) :
		Player(symb) {

}

MinimaxPlayer::~MinimaxPlayer() {

}

/**
 * Evaluates non terminal nodes, essentially check the value of a terminal state.
 * @param board   The current game state
 * @return        Utility
 */
int MinimaxPlayer::getUtility(OthelloBoard *board) {
	return board->count_score(board->get_p1_symbol()) - board->count_score(board->get_p2_symbol());
}

/**
 * Evaluate the possible moves remaining in the game.
 * @param playerSymbol   Player symbol character
 * @param board			 The current game state
 * @return 				 List of successor game states
 */
vector<OthelloBoard*> MinimaxPlayer::getSuccessors(char playerSymbol, OthelloBoard *board) {
	vector<OthelloBoard*> boardVector;
	int boardDimensions = 4;

	// Check every spot in the 2D array
	for (int i = 0; i < boardDimensions; i++) {
		for (int j = 0; j < boardDimensions; j++) {

			// Check the possible moves
			if (board->is_legal_move(i, j, playerSymbol)) {

				// Simulate all possible boards
				boardVector.push_back(new OthelloBoard(*board));
				boardVector.back()->play_move(i, j, symbol);

				// Set value on board
				boardVector.back()->setColumn(i);
				boardVector.back()->setRow(j);
			}
		}
	}

	return boardVector;
}

/**
 * Use successor function to generate all possible future states and return those with the best minimum value.
 * @param  row          The row
 * @param  column       The column
 * @param  playerSymbol Player piece symbol
 * @param  board        Game state
 * @return              Maximized value
 */
int MinimaxPlayer::minimumValue(int &row, int &column, char playerSymbol, OthelloBoard *board) {
	vector<OthelloBoard*> boardVector;
	int minimumRow    = 0;
	int minimumColumn = 0;
	int theMin        = 32767;

	// Check which player
	if (playerSymbol == 'X') {
		boardVector = getSuccessors('X', board);
	}

	if (playerSymbol == 'O') {
		boardVector = getSuccessors('O', board);
	}

	// Check for empty list, if so return value of the state
	if (boardVector.size() == 0) {
		return getUtility(board);
	}

	// Otherwise keep recursively going on until empty list of possible moves
	for (int i = 0; i < boardVector.size(); i++) {
		if (minimumValue(row, column, playerSymbol, boardVector[i]) > theMin) {
			minimumRow = boardVector[i]->getRow();
			minimumColumn = boardVector[i]->getColumn();
			theMin = minimumValue(row, column, playerSymbol, boardVector[i]);
		}
	}

	row = minimumRow;
	column = minimumRow;
	return theMin;
}

/**
 * Use successor function to generate all possible future states and return those with the best maximum value.
 * @param  row          The row
 * @param  column       The column
 * @param  playerSymbol Player piece symbol
 * @param  board        Game state
 * @return              Maximized value
 */
int MinimaxPlayer::maximumValue(int &row, int &column, char playerSymbol, OthelloBoard *board) {
	vector<OthelloBoard*> boardVector;
	int maximumRow    = 0;
	int maximumColumn = 0;
	int theMax        = -32767;

	// Check which player
	if (playerSymbol == 'X') {
		boardVector = getSuccessors('X', board);
	}

	if (playerSymbol == 'O') {
		boardVector = getSuccessors('O', board);
	}

	// Check for empty list, if so return value of the state
	if (boardVector.size() == 0) {
		return getUtility(board);
	}

	// Otherwise keep recursively going on until empty list of possible moves
	for (int i = 0; i < boardVector.size(); i++) {
		if (minimumValue(row, column, playerSymbol, boardVector[i]) > theMax) {
			maximumRow = boardVector[i]->getRow();
			maximumColumn = boardVector[i]->getColumn();
			theMax = minimumValue(row, column, playerSymbol, boardVector[i]);
		}
	}

	row = maximumRow;
	column = maximumColumn;
	return theMax;
}

/**
 * Determine player and initialize Minimax Algorithm
 * @param b   Game state
 * @param col The column
 * @param row The row
 */
void MinimaxPlayer::get_move(OthelloBoard *b, int &col, int &row) {

	// Check player and initialize minimax on appropriate player
	if (symbol == b->get_p1_symbol()) {
		maximumValue(row, col, 'X', b);
	} else if (symbol == b->get_p2_symbol()) {
		maximumValue(row, col, 'O', b);
	}
}

MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}
