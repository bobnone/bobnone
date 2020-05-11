//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Board.h"
#include "Random.h"

BoardState::BoardState()
{
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 7; j++)
		{
			board_[i][j] = Empty;
		}
	}
}
std::vector<BoardState*> BoardState::getPossibleMoves(SquareState player) const
{
	std::vector<BoardState*> retVal;
	//For each column, find if a move is possible
	for(int col = 0; col < 7; col++)
	{
		for(int row = 5; row >= 0; row--)
		{
			if(board_[row][col] == BoardState::Empty)
			{
				retVal.emplace_back(new BoardState(*this));
				retVal.back()->board_[row][col] = player;
				break;
			}
		}
	}
	return retVal;
}
bool BoardState::isTerminal() const
{
	//Is the board full?
	if(isFull())
	{
		return true;
	}
	//Is there a four-in-a-row?
	int fourInRow = getFourInARow();
	if(fourInRow != 0)
	{
		return true;
	}
	return false;
}
float BoardState::getScore() const
{
	//If the board is full, the score is 0
	if(isFull())
	{
		return 0.0f;
	}
	//Is there a four-in-a-row?
	int fourInRow = getFourInARow();
	if(fourInRow != 0)
	{
		return static_cast<float>(fourInRow);
	}
	return calculateHeuristic();
}
bool BoardState::isFull() const
{
	bool full = true;
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 7; j++)
		{
			if(board_[i][j] == Empty)
			{
				full = false;
			}
		}
	}
	return full;
}
int BoardState::getFourInARow() const
{
	//Returns -1 if yellow wins, 1 if red wins, 0 otherwise
	//Check if there's a row with four in a row
	for(int row = 0; row < 6; row++)
	{
		for(int col = 0; col < 4; col++)
		{
			if(board_[row][col] == board_[row][col + 1] && board_[row][col] == board_[row][col + 2] && board_[row][col] == board_[row][col + 3])
			{
				if(board_[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if(board_[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}
	// Check if there's a column with four in a row
	for(int col = 0; col < 7; col++)
	{
		for(int row = 0; row < 3; row++)
		{
			if(board_[row][col] == board_[row + 1][col] && board_[row][col] == board_[row + 2][col] && board_[row][col] == board_[row + 3][col])
			{
				if(board_[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if(board_[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}
	// Check if there's a right diagonal four in a row
	for(int col = 0; col < 4; col++)
	{
		for(int row = 0; row < 3; row++)
		{
			if(board_[row][col] == board_[row + 1][col + 1] && board_[row][col] == board_[row + 2][col + 2] && board_[row][col] == board_[row + 3][col + 3])
			{
				if(board_[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if(board_[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}
	// Check if there's a left diagonal for in a row
	for(int col = 0; col < 4; col++)
	{
		for(int row = 3; row < 6; row++)
		{
			if(board_[row][col] == board_[row - 1][col + 1] && board_[row][col] == board_[row - 2][col + 2] && board_[row][col] == board_[row - 3][col + 3])
			{
				if(board_[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if(board_[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}
float BoardState::calculateHeuristic() const
{
	// TODO: You could change this to calculate an actual heuristic
	return 0.0f;
}
bool tryPlayerMove(BoardState* state, int column)
{
	//Find the first row in that column that's available (if any)
	for(int row = 5; row >= 0; row--)
	{
		if(state->board_[row][column] == BoardState::Empty)
		{
			state->board_[row][column] = BoardState::Yellow;
			return true;
		}
	}
	return false;
}
void CPUMove(BoardState* state)
{
	//For now, this just randomly picks one of the possible moves
	std::vector<BoardState*> moves = state->getPossibleMoves(BoardState::Red);
	int index = Random::getIntRange(0, moves.size() - 1);
	*state = *moves[index];
	//Clear up memory from possible moves
	for(auto state : moves)
	{
		delete state;
	}
}