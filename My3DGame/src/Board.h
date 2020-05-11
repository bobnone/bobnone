//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <vector>

class BoardState
{
public:
	enum SquareState
	{
		Empty,
		Red,
		Yellow
	};
	BoardState();
	std::vector<BoardState*> getPossibleMoves(SquareState player) const;
	bool isTerminal() const;
	float getScore() const;
	SquareState board_[6][7];
protected:
	bool isFull() const;
	int getFourInARow() const;
	float calculateHeuristic() const;
};
//Try to place the player's piece
bool tryPlayerMove(class BoardState* state, int column);
//Make the next CPU move
void CPUMove(class BoardState* state);