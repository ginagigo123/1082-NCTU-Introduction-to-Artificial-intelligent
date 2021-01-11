#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h> 
#include <time.h>
#include "STcpClient.h"
using namespace std;

double probability[8][8] = { 0   ,1.2,1   ,1.1 ,1.1 ,1   ,1.2,0   ,
							  1.2,0.8,0.9,0.9,0.9,0.9,0.8,1.2,
							  1   ,0.9,1   ,1   ,1   ,1   ,0.9,1   ,
							  1.1 ,0.9,1   ,1   ,1   ,1   ,0.9,1   ,
							  1.1 ,0.9,1   ,1   ,1   ,1   ,0.9,1   ,
							  1   ,0.9,1   ,1   ,1   ,1   ,0.9,1   ,
							  1.2,0.8,0.9,0.9,0.9,0.9,0.8,1.2,
							  0   ,1.2,1   ,1.1 ,1.1 ,1   ,1.2,0 };

class Position
{
	int x;
	int y;
public:
	int getX() { return x; }
	int getY() { return y; }
	void setX(int i) { x = i; }
	void setY(int j) { y = j; }
};


class Board {
	int boardValues[8][8];// [0: unoccupied]; [1: occupied by Black]; [2: occupied by White]; [-1: the four corners]
	int totalMoves;
public:
	int DEFAULT_BOARD_SIZE;
	int IN_PROGRESS;
	int DRAW;
	int Black;
	int White;

	Board()
	{
		DEFAULT_BOARD_SIZE = 8;
		IN_PROGRESS = -1;
		DRAW = 0;
		Black = 1;
		White = 2;
		totalMoves = 0;
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				boardValues[i][j] = 0;
		boardValues[0][0] = boardValues[7][0] = boardValues[0][7] = boardValues[7][7] = -1;
	}
	// getters and setters
	int getBoardValues(int x, int y) { return boardValues[x][y]; }
	int getTotalMoves() { return totalMoves; }

	void setBoardValues(int i, int j, int v) { boardValues[i][j] = v; }//for testing code
	void printboard() {
		for (int i = 0; i < 8; i++)
		{
			cout << "| ";
			for (int j = 0; j < 8; j++)
			{
				cout << boardValues[i][j] << "\t| ";
			}
			cout << endl;
		}
	}
	void performMove(int player, int opponent, Position p) {
		totalMoves++;
		int x = p.getX();
		int y = p.getY();
		boardValues[x][y] = player;
		//cout<<"player:"<<player<<endl;
		if (x - 1 > 0 && y - 1 > 0)//check左上角斜線翻轉 
		{
			bool flag = false;
			int i = x - 2;
			int j = y - 2;
			if (boardValues[x - 1][y - 1] == opponent)
			{
				while (i >= 0 && j >= 0)
				{
					if (boardValues[i][j] == player)
					{
						flag = true;
						break;
					}
					else if (boardValues[i][j] == -1 || boardValues[i][j] == 0)
						break;
					i--;
					j--;
				}
			}
			if (flag)
			{
				i++;
				j++;
				while (i != x && j != y)
				{
					boardValues[i][j] = player;
					i++;
					j++;
				}
			}
		}
		if (x - 1 > 0)//check上方翻轉 
		{
			bool flag = false;
			int i = x - 2;
			if (boardValues[x - 1][y] == opponent)
			{
				while (i >= 0)
				{
					if (boardValues[i][y] == player)
					{
						flag = true;
						break;
					}
					else if (boardValues[i][y] == -1 || boardValues[i][y] == 0)
						break;
					i--;
				}
			}
			if (flag)
			{
				i++;
				while (i != x)
				{
					boardValues[i][y] = player;
					i++;
				}
			}
		}
		if (x - 1 > 0 && y + 1 < 7)//check右上角斜線翻轉 
		{
			bool flag = false;
			int i = x - 2;
			int j = y + 2;
			if (boardValues[x - 1][y + 1] == opponent)
			{
				while (i >= 0 && j < 8)
				{
					if (boardValues[i][j] == player)
					{
						flag = true;
						break;
					}
					else if (boardValues[i][j] == -1 || boardValues[i][j] == 0)
						break;
					i--;
					j++;
				}
			}
			if (flag)
			{
				i++;
				j--;
				while (i != x && j != y)
				{
					boardValues[i][j] = player;
					i++;
					j--;
				}
			}
		}
		if (y - 1 > 0)//check左方翻轉 
		{
			bool flag = false;
			int j = y - 2;
			if (boardValues[x][y - 1] == opponent)
			{
				while (j >= 0)
				{
					if (boardValues[x][j] == player)
					{
						flag = true;
						break;
					}
					else if (boardValues[x][j] == -1 || boardValues[x][j] == 0)
						break;
					j--;
				}
			}
			if (flag)
			{
				j++;
				while (j != y)
				{
					boardValues[x][j] = player;
					j++;
				}
			}
		}
		if (y + 1 < 7)//check右方翻轉 
		{
			bool flag = false;
			int j = y + 2;
			if (boardValues[x][y + 1] == opponent)
			{
				while (j < 8)
				{
					if (boardValues[x][j] == player)
					{
						flag = true;
						break;
					}
					else if (boardValues[x][j] == -1 || boardValues[x][j] == 0)
						break;
					j++;
				}
			}
			if (flag)
			{
				j--;
				while (j != y)
				{
					boardValues[x][j] = player;
					j--;
				}
			}
		}
		if (x + 1 < 7 && y - 1 > 0)//check左下角斜線翻轉 
		{
			bool flag = false;
			int i = x + 2;
			int j = y - 2;
			if (boardValues[x + 1][y - 1] == opponent)
			{
				while (i < 8 && j >= 0)
				{
					if (boardValues[i][j] == player)
					{
						flag = true;
						break;
					}
					else if (boardValues[i][j] == -1 || boardValues[i][j] == 0)
						break;
					i++;
					j--;
				}
			}
			if (flag)
			{
				i--;
				j++;
				while (i != x && j != y)
				{
					boardValues[i][j] = player;
					i--;
					j++;
				}
			}
		}
		if (x + 1 < 7)//check下方翻轉 
		{
			bool flag = false;
			int i = x + 2;
			if (boardValues[x + 1][y] == opponent)
			{
				while (i < 8)
				{
					if (boardValues[i][y] == player)
					{
						flag = true;
						break;
					}
					else if (boardValues[i][y] == -1 || boardValues[i][y] == 0)
						break;
					i++;
				}
			}
			if (flag)
			{
				i--;
				while (i != x)
				{
					boardValues[i][y] = player;
					i--;
				}
			}
		}
		if (x + 1 < 7 && y + 1 < 7)//check右下角斜線翻轉 
		{
			bool flag = false;
			int i = x + 2;
			int j = y + 2;
			if (boardValues[x + 1][y + 1] == opponent)
			{
				while (i < 8 && j < 8)
				{
					if (boardValues[i][j] == player)
					{
						flag = true;
						break;
					}
					else if (boardValues[i][j] == -1 || boardValues[i][j] == 0)
						break;
					i++;
					j++;
				}
			}
			if (flag)
			{
				i--;
				j--;
				while (i != x && j != y)
				{
					boardValues[i][j] = player;
					i--;
					j--;
				}
			}
		}
	}

	int checkStatus() {
		/* Evaluate whether the game is won and return winner.
		   If it is draw return 0 else return -1 */
		int B = 0;
		int W = 0;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)//算棋子 
			{
				if (boardValues[i][j] == 0)//還有空位 
				{
					vector<Position> tmp1, tmp2;
					tmp1 = this->getPossiblePositions(1, 2);
					tmp2 = this->getPossiblePositions(2, 1);
					if (tmp1.size() != 0 || tmp2.size() != 0)
					{
						return IN_PROGRESS;
					}
				}
				if (boardValues[i][j] == Black)
					B++;
				else if (boardValues[i][j] == White)
					W++;
			}
		}
		if (B > W)
			return Black;
		else if (B == W)
			return DRAW;
		else
			return White;
	}
	
	vector<Position> getPossiblePositions(int player, int opponent) {
		vector<Position> emptyPositions;
		//cout<<"get position function start"<<endl;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (boardValues[i][j] == 0)
				{
					if (i < 7 && i>0 && j < 7 && j>0)//如果是中間的都可以填 
					{
						Position p;
						p.setX(i);
						p.setY(j);
						emptyPositions.push_back(p);
					}
					else if (i == 0 && j != 0 && j != 7)//如果是上面第一排 
					{
						bool flag = false;
						if (boardValues[i][j - 1] == opponent)//左線
						{
							int k = j - 2;
							int v;
							while (k >= 0)
							{
								v = boardValues[i][k];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								k--;
							}
						}
						if (!flag && boardValues[i + 1][j - 1] == opponent)//左斜
						{
							int x = i + 2;
							int y = j - 2;
							int v;
							while (x < 8 && y >= 0)
							{
								v = boardValues[x][y];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								x++;
								y--;
							}
						}
						if (!flag && boardValues[i + 1][j] == opponent)//下方
						{
							int x = i + 2;
							int v;
							while (x < 8)
							{
								v = boardValues[x][j];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								x++;
							}
						}
						if (!flag && boardValues[i + 1][j + 1] == opponent)//右斜
						{
							int x = i + 2;
							int y = j + 2;
							int v;
							while (x < 8 && y < 8)
							{
								v = boardValues[x][y];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								x++;
								y++;
							}
						}
						if (!flag && boardValues[i][j + 1] == opponent)//右方
						{
							int y = j + 2;
							int v;
							while (y < 8)
							{
								v = boardValues[i][y];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								y++;
							}
						}
					}
					else if (i == 7 && j != 0 && j != 7)//如果是下面最後一排 
					{
						bool flag = false;
						if (boardValues[i][j - 1] == opponent)//左方
						{
							int k = j - 2;
							int v;
							while (k >= 0)
							{
								v = boardValues[i][k];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								k--;
							}
						}
						if (!flag && boardValues[i - 1][j - 1] == opponent)//左斜上
						{
							//cout << "check 左斜上" << endl;
							int x = i - 2;
							int y = j - 2;
							int v;
							while (x >= 0 && y >= 0)
							{
								v = boardValues[x][y];
								if (v == player)
								{
									//cout << "put!!!" << endl;
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								x--;
								y--;
							}
						}
						if (!flag && boardValues[i - 1][j] == opponent)
						{
							int x = i - 2;
							int v;
							while (x >= 0)
							{
								v = boardValues[x][j];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								x--;
							}
						}
						if (!flag && boardValues[i - 1][j + 1] == opponent)
						{
							int x = i - 2;
							int y = j + 2;
							int v;
							while (x >= 0 && y < 8)
							{
								v = boardValues[x][y];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								x--;
								y++;
							}
						}
						if (!flag && boardValues[i][j + 1] == opponent)
						{
							int y = j + 2;
							int v;
							while (y < 8)
							{
								v = boardValues[i][y];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								y++;
							}
						}
					}
					else if (j == 0 && i != 0 && i != 7)//如果是左邊第一列 
					{
						bool flag = false;
						if (boardValues[i][j + 1] == opponent)
						{
							int k = j + 2;
							int v;
							while (k < 8)
							{
								v = boardValues[i][k];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								k++;
							}
						}
						if (!flag && boardValues[i - 1][j + 1] == opponent)
						{
							int x = i - 2;
							int y = j + 2;
							int v;
							while (x >= 0 && y < 8)
							{
								v = boardValues[x][y];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								x--;
								y++;
							}
						}
						if (!flag && boardValues[i - 1][j] == opponent)
						{
							int x = i - 2;
							int v;
							while (x >= 0)
							{
								v = boardValues[x][j];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								x--;
							}
						}
						if (!flag && boardValues[i + 1][j + 1] == opponent)
						{
							int x = i + 2;
							int y = j + 2;
							int v;
							while (x < 8 && y < 8)
							{
								v = boardValues[x][y];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								x++;
								y++;
							}
						}
						if (!flag && boardValues[i + 1][j] == opponent)
						{
							int x = i + 2;
							int v;
							while (x < 8)
							{
								v = boardValues[x][j];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								x++;
							}
						}
					}
					else if (j == 7 && i != 0 && i != 7)//如果是右邊最後一列 
					{
						bool flag = false;
						if (boardValues[i][j - 1] == opponent)
						{
							int k = j - 2;
							int v;
							while (k >= 0)
							{
								v = boardValues[i][k];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								k--;
							}
						}
						if (!flag && boardValues[i - 1][j - 1] == opponent)
						{
							int x = i - 2;
							int y = j - 2;
							int v;
							while (x >= 0 && y >= 0)
							{
								v = boardValues[x][y];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								x--;
								y--;
							}
						}
						if (!flag && boardValues[i - 1][j] == opponent)
						{
							int x = i - 2;
							int v;
							while (x >= 0)
							{
								v = boardValues[x][j];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								x--;
							}
						}
						if (!flag && boardValues[i + 1][j - 1] == opponent)
						{
							int x = i + 2;
							int y = j - 2;
							int v;
							while (x < 8 && y >= 0)
							{
								v = boardValues[x][y];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								x++;
								y--;
							}
						}
						if (!flag && boardValues[i + 1][j] == opponent)
						{
							int x = i + 2;
							int v;
							while (x < 8)
							{
								v = boardValues[x][j];
								if (v == player)
								{
									Position p;
									p.setX(i);
									p.setY(j);
									emptyPositions.push_back(p);
									flag = true;
									break;
								}
								else if (v == 0 || v == -1)
								{
									break;
								}
								x++;
							}
						}
					}
				}
			}
		}
		//cout << "get position function end:" << emptyPositions.size() << endl;
		return emptyPositions;
	}

};


class State {
	Board board;
	int playerNo;//1or 2
	Position move;

	// copy constructor, getters, and setters
public:
	void setStateboard(Board b) { board = b; }
	void setStatePlayerNo(int p) { playerNo = p; }
	void setMove(Position p) { move.setX(p.getX()); move.setY(p.getY()); }

	Board	getStateboard() { return board; }
	int		getStatePlayerNo() { return playerNo; }
	Position getMove() { return move; }

	int	getStateOpponent() {
		if (playerNo == 1)
			return 2;
		else
			return 1;
	}
	vector<State> getAllPossibleState()
	{
		// constructs a list of all possible step from current state
		vector<State> possibleState;
		vector<Position> possiblePosition;
		int opponent;
		if (playerNo == 1)//black
		{
			opponent = 2;
		}
		else
		{
			opponent = 1;
		}
		//cout << "player:" << playerNo << endl;
		possiblePosition = board.getPossiblePositions(opponent, playerNo);
		//cout << "possible position:" << possiblePosition.size() << endl;
		for (int i = 0; i < possiblePosition.size(); i++)
		{
			State tmp;
			tmp.setStateboard(board);
			tmp.board.performMove(opponent, playerNo, possiblePosition[i]);
			//cout<<"move:("<<possiblePosition[i].getX()<<","<<possiblePosition[i].getY()<<")"<<endl;
			//tmp.getStateboard().printboard();
			tmp.setMove(possiblePosition[i]);
			tmp.setStatePlayerNo(opponent);
			possibleState.push_back(tmp);

		}
		return possibleState;

	}
	State randomPlay(int level)//for simultation
	{
		/* get a list of all possible positions on the board and
		   play a random move */
		   //cout << "random play:" << endl;
		int opponent;
		if (playerNo == 1)//black
		{
			opponent = 2;
		}
		else
		{
			opponent = 1;
		}
		vector<Position> possiblePosition = board.getPossiblePositions(playerNo, opponent);
		srand(time(NULL));
		if (possiblePosition.size() > 0)
		{
			//int i = rand() % possiblePosition.size();
			Position po;
			double p=0;
			for (int i = 0; i < possiblePosition.size(); i++)
			{
				if (probability[possiblePosition[i].getX()][possiblePosition[i].getY()] > p)
					po = possiblePosition[i];
			}
			State tmp;
			tmp.setStateboard(board);
			tmp.board.performMove(playerNo, opponent, po);
			tmp.setMove(po);
			tmp.setStatePlayerNo(opponent);
			return tmp;
		}
		else
		{
			//cout << "no place to use" << endl;
			State tmp;
			tmp.setStateboard(board);
			tmp.setStatePlayerNo(opponent);
			return tmp;
		}

	}
};

class Node {
	State state;
	Node* parent;
	vector<Node> childArray;
	int visitCount;
	double winScore;
	bool root;
	// setters and getters
public:
	Node() { root = false; visitCount = 0; winScore = 0; }
	void setNodeState(State s) { state = s; }
	void setNodeParent(Node* p) { parent = p; }
	void setNodeChild(Node c) { childArray.push_back(c); }
	void addNodeVisitCount() { visitCount++; }
	void addNodeWinScore() { winScore++; }
	void setNodeWinScore(double s) { winScore = s; }
	void setNoderoot() { root = true; }

	State	getNodeState() { return state; }
	Node* getNodeParent() { return parent; }
	Node* getNodeChild(int i) { return &childArray[i]; }
	int		getNodeChildNum() { return childArray.size(); }
	int		getNodeVisitCount() { return visitCount; }
	double	getNodeWinScore() { return winScore; }
	bool	getNoderoot() { return root; }
	Node* getRandomChildNode()
	{
		//cout << "get randome chlid" << endl;
		int i = rand() % childArray.size();
		/*int child;
		if (this->getNodeChildNum() > 0)
		{
			//int i = rand() % possiblePosition.size();
			double p = 0;
			for (int i = 0; i < this->getNodeChildNum(); i++)
			{
				if (probability[this->getNodeChild(i)->getNodeState().getMove().getX()][this->getNodeChild(i)->getNodeState().getMove().getY()] > p)
					child = i;
			}
		}*/
		Node* tmp = &childArray[i];
		return(tmp);
	};//get a random child to explore
	double	getChildScore(int i) { return (childArray[i]).getNodeWinScore(); }
	Node* getChildWithMaxScore()
	{
		if (childArray.size() > 0)
		{
			double max = (childArray[0]).getNodeWinScore();//勝率
			Node* Max = &childArray[0];
			for (int i = 1; i < childArray.size(); i++)
			{
				if (((childArray[i]).getNodeWinScore()) > max)
				{
					max = ((childArray[i]).getNodeWinScore());
					Max = &childArray[i];
				}
			}
			return Max;
		}
		else
		{
			Node* tmp = this;//get node parent and parent's move
			return tmp;
		}
	}
};

class Tree {
	Node root;
public:
	Tree() {
		root.setNoderoot();
	}
	Node getRoot() { return root; }
	void setRoot(Node r) { root = r; }
};

class UCT {

public:
	static double uctValue(int parentVisit, double nodeWinScore, int nodeVisit)
	{
		if (nodeVisit == 0) {
			return 99999;
		}
		return (((double)nodeWinScore / (double)nodeVisit) + 1.6 * sqrt(log(parentVisit) / (double)nodeVisit));
	}

	double Valueprobabilities(int player, Position p, double sum, Board B)
	{
		int i = p.getX(), j = p.getY();
		if ((i == 0 || i == 7) && (j == 1 || j == 6))
			return 1.15;
		else if ((i == 1 || i == 6) && (j == 0 || j == 7))
			return 1.15;
		else if ((i == 3 || i == 4) && (j == 0 || j == 7))
			return 1.1;
		else if (((i == 0 || i == 7) && (j == 3 || j == 4)))
			return 1.1;
		// bad
		else if (((i == 1 || i == 6) && (j == 1 || j == 6))) {
			if (i == 1 && j == 1)
				if (B.getBoardValues(1, 0) != player || B.getBoardValues(0, 1) != player)
					return 0.85;
			if (i == 1 && j == 6)
				if (B.getBoardValues(1, 7) != player || B.getBoardValues(0, 6) != player)
					return 0.85;
			if (i == 6 && j == 1)
				if (B.getBoardValues(6, 0) != player || B.getBoardValues(7, 1) != player)
					return 0.85;
			if (i == 6 && j == 6)
				if (B.getBoardValues(6, 7) != player || B.getBoardValues(7, 6) != player)
					return 0.85;
		}
		else if (((i == 2 || i == 5) && (j == 0 || j == 7))) {
			if (i == 2 && j == 0)
				if (B.getBoardValues(1, 0) != player)
					return 0.95;
			if (i == 2 && j == 7)
				if (B.getBoardValues(1, 7) != player)
					return 0.95;
			if (i == 5 && j == 0)
				if (B.getBoardValues(6, 0) != player)
					return 0.95;
			if (i == 5 && j == 7)
				if (B.getBoardValues(6, 7) != player)
					return 0.95;
		}
		else if (((i == 0 || i == 7) && (j == 2 || j == 5))) {
			if (i == 0 && j == 2)
				if (B.getBoardValues(0, 1) != player)
					return 0.95;
			if (i == 0 && j == 5)
				if (B.getBoardValues(0, 6) != player)
					return 0.95;
			if (i == 7 && j == 2)
				if (B.getBoardValues(7, 1) != player)
					return 0.95;
			if (i == 7 && j == 5)
				if (B.getBoardValues(7, 6) != player)
					return 0.95;
		}
	}

	static Node* findBestNodeWithUCT(Node* node,int level)
	{
		double tmp;
		int parentVisit = (*node).getNodeVisitCount();
		Node* child = (*node).getNodeChild(0);
		double max;
		int playerno = child->getNodeState().getStatePlayerNo();
		if (level < 54)
		{	
			double result = uctValue(parentVisit, (*node).getChildScore(0), (*child).getNodeVisitCount());
			Position x = child->getNodeState().getMove();
			//int y = child->getNodeState().getMove()
			Board b = child->getNodeState().getStateboard();

			//result *= Valueprobabilities (playerno, x, result, b);
			//max = probability[][child->getNodeState().getMove().getY()]*;
			max = result;
		}
			
		else
			max= uctValue(parentVisit, (*node).getChildScore(0), (*child).getNodeVisitCount());
		//cout<<"uct:" << max << endl;
		Node* Max = child;
		for (int i = 1; i < (*node).getNodeChildNum(); i++)
		{
			child = (*node).getNodeChild(i);
			if(level<54)
				tmp= probability[child->getNodeState().getMove().getX()][child->getNodeState().getMove().getY()] * uctValue(parentVisit, (*node).getChildScore(i), (*child).getNodeVisitCount());
			else
				tmp = uctValue(parentVisit, (*node).getChildScore(i), (*child).getNodeVisitCount());
			//cout << "uct:" << tmp << endl;
			if (tmp > max)//&&(*child).getNodeState().getAllPossibleState().size()!=0
			{
				Max = child;
				max = tmp;
			}
		}
		return Max;
	}
};

class MonteCarloTreeSearch {
	double WIN_SCORE;
	int opponent;
	double t;
	int level;

	Node* selectPromisingNode(Node* rootNode)
	{
		Node* node = rootNode;

		while ((*node).getNodeChildNum() != 0) {
			//cout << "select with UCT" << endl;

			//(*node).getNodeState().getStateboard().boardvalue((*rootNode).getNodeState().getStatePlayerNo());
			UCT u;
			node = u.findBestNodeWithUCT(node,level);
		}
		return node;
	}

	void expandNode(Node* node) {
		vector<State> possibleStates = (*node).getNodeState().getAllPossibleState();
		//cout << "expanding function: possibleState size"<<possibleStates.size() << endl;
		for (int i = 0; i < possibleStates.size(); i++)
		{
			Node newNode;
			newNode.setNodeState(possibleStates[i]);
			newNode.setNodeParent(node);
			newNode.getNodeState().setStatePlayerNo((*node).getNodeState().getStateOpponent());
			(*node).setNodeChild(newNode);
			//newNode.getNodeState().getStateboard().printboard();
		}
	}

	void backPropogation(Node* nodeToExplore, bool playerwin) {

		Node* tempNode = nodeToExplore;
		//cout << "back propogation start:" << endl;
		while (!(*tempNode).getNoderoot()) {
			(*tempNode).addNodeVisitCount();
			//cout << "add VISITCOUNT" << endl;
			if (playerwin)
			{
				(*tempNode).addNodeWinScore();
			}
			tempNode = (*tempNode).getNodeParent();
		}
		(*tempNode).addNodeVisitCount();
		if (playerwin)
		{
			(*tempNode).addNodeWinScore();
		}
	}

	int simulateRandomPlayout(Node* node) {
		//cout << "sinulation play start:" << endl;
		//cout << "parent address:" << (*node).getNodeParent() << endl;
		//cout << "my address:" << node << endl;
		State tempState = (*node).getNodeState();
		//cout << "parent address:" << (*node).getNodeParent() << endl;
		//cout << "my address:" << node << endl;
		int boardStatus = tempState.getStateboard().checkStatus();
		if (boardStatus != -1) {//game end
			return boardStatus;
		}
		while (boardStatus == -1) {//game is continueing
			tempState = tempState.randomPlay(level);
			boardStatus = tempState.getStateboard().checkStatus();
		}
		//tempState.getStateboard().printboard();
		return boardStatus;
	}



public:

	MonteCarloTreeSearch() { WIN_SCORE = 1; t = clock(); }
	Position findNextMove(Board board, int playerNo,int levels) {
		// define an end time which will act as a terminating condition
		level = levels;
		if (playerNo == 1)//player is black
			opponent = 2;
		else//player is white
			opponent = 1;
		Tree tree;
		Node rootNode = tree.getRoot();
		State tmp;
		tmp.setStateboard(board);//目前的狀態
		tmp.setStatePlayerNo(opponent);//對方剛下棋 
		rootNode.setNodeState(tmp);
		//rootNode.getNodeState().getStateboard().printboard();
		//cout<<"root player:"<<rootNode.getNodeState().getStatePlayerNo()<<endl;
		int count = 1;
		while ((clock() - t) / CLOCKS_PER_SEC < 5 /*count>0*/) {
			//selection
			//cout << "select" << endl;
			Node* promisingNode = selectPromisingNode(&rootNode);
			//(*promisingNode).getNodeState().getStateboard().printboard();
			//cout << "parent address:" << (*promisingNode).getNodeParent() << endl;
			//cout << "my address:" << promisingNode << endl;

			//expand
			if ((*promisingNode).getNodeState().getStateboard().checkStatus() == -1)//如果還沒到底
			{
				//cout << "expand:" << endl;
				expandNode(promisingNode);
			}
			//simulation
			//cout << "simulation" << endl;
			Node* nodeToExplore;
			if ((*promisingNode).getNodeChildNum() > 0) {
				nodeToExplore = (*promisingNode).getRandomChildNode();
				//(*nodeToExplore).getNodeState().getStateboard().printboard();
				//cout << "parent address:" << (*nodeToExplore).getNodeParent() << endl;
				//cout << "my address:" << nodeToExplore << endl;
				int playoutResult = simulateRandomPlayout(nodeToExplore);
				//cout << "result" << playoutResult << endl;
				if (playerNo == playoutResult)
				{
					backPropogation(nodeToExplore, true);
					//cout << "win" << endl;
				}
				else
					backPropogation(nodeToExplore, false);
				//cout << rootNode.getNodeVisitCount() << "," << rootNode.getNodeWinScore();
				//cout << "end a while" << endl;

			}
			else
			{
				//cout << "no expand node!" << endl;
				break;
			}
		}
		//cout << "time up!" << endl;
		Node* winnerNode = rootNode.getChildWithMaxScore();
		//(*winnerNode).getNodeState().getStateboard().printboard();
		tree.setRoot(*winnerNode);
		return (*winnerNode).getNodeState().getMove();
	}


};



/*int main()
{
	Board board;
	int test1[8][8] = {-1,1,2,2,2,2,2,-1,
						2,1,1,2,2,1,1,1,
						1,1,2,2,2,2,1,1,
						1,1,1,1,1,1,1,1,
						2,2,2,2,1,2,1,2,
						2,1,2,1,2,1,2,2,
						2,2,2,2,2,2,2,2,
					   -1,1,2,2,2,0,0,-1};
	int test2[8][8] = {-1,2,1,1,1,2,2,-1,
						1,2,1,1,1,2,2,2,
						1,2,1,1,2,1,1,2,
						2,2,2,2,1,2,1,2,
						2,2,2,2,2,1,2,2,
						0,2,1,1,1,2,2,2,
						2,2,2,2,2,2,2,0,
						-1,0,2,2,0,0,0,-1};
	Position step;
	int playerno = 1;//black
	int count = 2;
	int i, j, v;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board.setBoardValues(i, j, test2[i][j]);
		}
	}
	board.printboard();
	MonteCarloTreeSearch mcts;
	step = mcts.findNextMove(board, playerno);
	cout << "next step: (" << step.getX() << "," << step.getY() << ")" << endl;
	return 0;
}*/



std::vector<int> GetStep(std::vector<std::vector<int>>& board, bool is_black) {

	MonteCarloTreeSearch mcts;
	Board boards;
	Position step;
	vector<int> steps;
	int playerno;
	int level=0;
	if (is_black)
		playerno = 1;
	else
		playerno = 2;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			boards.setBoardValues(i, j, board[i][j]);
			if (board[i][j] != 0)
				level+=1;
		}
	}
	//boards.printboard();
	step = mcts.findNextMove(boards, playerno,level);
	steps.resize(2);
	steps[0] = step.getX();
	steps[1] = step.getY();
	return steps;
}

int main() {
	int id_package;
	std::vector<std::vector<int>> board;
	std::vector<int> step;

	bool is_black;
	while (true) {
		if (GetBoard(id_package, board, is_black))
			break;

		step = GetStep(board, is_black);
		SendStep(id_package, step);
	}
}




