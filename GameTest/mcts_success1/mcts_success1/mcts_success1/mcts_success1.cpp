#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h> 
#include <time.h>
//#include "STcpClient.h"
using namespace std;

double probability[8][8] = {0   ,1.20,1   ,1.1 ,1.1 ,1   ,1.20,0   ,
					       1.20,0.80,0.95,0.95,0.95,0.95,0.80,1.15,
					       1   ,0.95,1   ,1   ,1   ,1   ,0.95,1   ,
					       1.1 ,0.95,1   ,1   ,1   ,1   ,0.95,1   ,
					       1.1 ,0.95,1   ,1   ,1   ,1   ,0.95,1   , 
					       1   ,0.95,1   ,1   ,1   ,1   ,0.95,1   , 
					       1.20,0.80,0.95,0.95,0.95,0.95,0.80,1.15, 
					       0   ,1.20,1   ,1.1 ,1.1 ,1   ,1.20,0   };

class Position
{
	int x;
	int y;
public:
	int getX() { return x; }
	int getY() { return y; }
	void setX(int i) { x = i; }
	void setY(int j) { y = j; }
}dir[8];

void ini(){
	dir[0].setX(-1);	dir[0].setY(-1);
	dir[1].setX(-1);	dir[1].setY(0);
	dir[2].setX(-1);	dir[2].setY(1);
	dir[3].setX(0);		dir[3].setY(-1);
	dir[4].setX(0);		dir[4].setY(1);
	dir[5].setX(1);		dir[5].setY(-1);
	dir[6].setX(1);		dir[6].setY(0);
	dir[7].setX(1);		dir[7].setY(1);
}

bool isValid(int x,int y){
	if( (x == 0 || x == 7) && (y == 0 || y == 7) )
		return false;
	return ((x >= 0 && x < 8 ) && (y >= 0 && y < 8));
}

class Board {
	int boardValues[8][8];// [0: unoccupied]; [1: occupied by Black]; [2: occupied by White]; [-1: the four corners]
	int totalMoves;
public:
	int DEFAULT_BOARD_SIZE;
	int IN_PROGRESS;
	int DRAW;
	int Black;
	int White;
	
	// constructor
	Board() {
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
		for (int i = 0; i < 8; i++) {
			cout << "| ";
			for (int j = 0; j < 8; j++) {
				cout << boardValues[i][j] << "\t| ";
			}
			cout << endl;
		}
	}
	// After Move , Same position will be filp over.
	void performMove(int player, int opponent, Position p) {
		totalMoves++;
		int x = p.getX();
		int y = p.getY();
		boardValues[x][y] = player;
		//cout<<"player:"<<player<<endl;
		for(int i = 0 ; i < 8 ; i ++){
			int cx = x + dir[i].getX();
			int cy = y + dir[i].getY();
			if(!isValid(cx,cy))	continue;
			if (boardValues[cx][cy] == 0) 
				continue;
			while (boardValues[cx][cy] == opponent){ 
				cx += dir[i].getX();
				cy += dir[i].getY();
				if (!isValid(cx,cy))	break;
				if(boardValues[cx][cy] == 0)
					break;
			}
			if (!isValid(cx,cy))	continue;
			if(boardValues[cx][cy] == 0) 
				continue;
			if(boardValues[cx][cy] == player){
				while (true) {
					cx -= dir[i].getX();
					cy -= dir[i].getY();
					if( cx == x && cy == y)
						break;
					boardValues[cx][cy] = player; 
				}
			}
		}
	}
	// look whether the Game is over
	int checkStatus() {
		/* Evaluate whether the game is won and return winner.
		   If it is draw return 0 else return -1 */
		int flag = 0;
		int B = 0;
		int W = 0;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++)//算棋子 
			{
				/*
				if (boardValues[i][j] == 0)//還有空位 
				{
					vector<Position> tmp1,tmp2;
					tmp1 = this->getPossiblePositions(1, 2);
					tmp2 = this->getPossiblePositions(2, 1);
					if (tmp1.size() != 0 || tmp2.size() != 0)
					{
						return IN_PROGRESS;
					}
				}*/
				if (boardValues[i][j] == Black)
					B++;
				else if (boardValues[i][j] == White)
					W++;
				else if(boardValues[i][j] == 0)
					flag ++;
				
			}
		}
		if(flag == 24)
			return DRAW;
		if(flag != 0 ) {
			cout<<"flag:"<<flag<<endl;
			return -1;
		}
		if(W == 0)	return Black;
		if (B > W || W == 0)
			return Black;
		else if (B == W)
			return DRAW;
		else if( W > B || B == 0)
			return White;
	}
	// get All the possible position
	vector<Position> getPossiblePositions(int player, int opponent) {
		vector<Position> emptyPositions;
		cout<<"get position function start"<<endl;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				//if( (i == 0 || i == 7) && (j == 0 || j == 7) )
					//continue;
				if (boardValues[i][j] == 0) {
					if (i < 7 && i > 0 && j < 7 && j>  0)//如果是中間的都可以填 
					{
						Position p;
						p.setX(i);
						p.setY(j);
						emptyPositions.push_back(p);
						cout<<"(i,j):"<<i<<","<<j<<endl;
					}
					else{
						for (int k = 0 ; k < 8 ; k ++) {
							int cx = i + dir[k].getX();
							int cy = j + dir[k].getY();
							if(!isValid(cx,cy))	continue;
							if (boardValues[cx][cy] == 0)
								continue;
							while (boardValues[cx][cy] == opponent){ 
								cx += dir[k].getX();
								cy += dir[k].getY();
								if (!isValid(cx,cy))	break;
								if(boardValues[cx][cy] == 0)
									break;
							}
							if (!isValid(cx,cy))	continue;
							if (boardValues[cx][cy] == 0) continue;
							if (boardValues[cx][cy] == player) {
								Position p;	p.setX(i);	p.setY(j);
								emptyPositions.push_back(p);
								cout<<"(i,j):"<<i<<","<<j<<endl;
								break;
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
	int playerNo; //1or 2
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
	
	// get all the possible state
	vector<State> getAllPossibleState() {
		// constructs a list of all possible step from current state
		vector<State> possibleState;
		vector<Position> possiblePosition;
		int opponent;
		if (playerNo == 1)//black
			opponent = 2;
		else
			opponent = 1;
		//cout << "player:" << playerNo << endl;
		possiblePosition = board.getPossiblePositions( opponent,playerNo);
		//cout << "possible position:" << possiblePosition.size() << endl;
		for (int i = 0; i < possiblePosition.size(); i++) {
			State tmp;
			tmp.setStateboard(board);
			tmp.board.performMove( opponent, playerNo,possiblePosition[i]);
			//cout<<"move:("<<possiblePosition[i].getX()<<","<<possiblePosition[i].getY()<<")"<<endl;
			//tmp.getStateboard().printboard();
			tmp.setMove(possiblePosition[i]);
			tmp.setStatePlayerNo(opponent);
			possibleState.push_back(tmp);
		}
		return possibleState;

	}
	State randomPlay() //for simultation
	{
		/* get a list of all possible positions on the board and
		   play a random move */
		//cout << "random play:" << endl;
		int opponent;
		if (playerNo == 1)//black
			opponent = 2;
		else
			opponent = 1;
		cout<<"������player:"<<playerNo<<endl;
		// gain opponent's move 
		vector<Position> possiblePosition = board.getPossiblePositions(playerNo, opponent);
		srand(time(NULL));
		if (possiblePosition.size() > 0) {
			int i = rand() % possiblePosition.size();
			State tmp;
			tmp.setStateboard(board);
			tmp.board.performMove(playerNo, opponent, possiblePosition[i]);
			tmp.setMove(possiblePosition[i]);
			tmp.setStatePlayerNo(opponent);
			return tmp;
		}
		else { 
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
	//get a random child to explore
	Node* getRandomChildNode() {
		//cout << "get randome chlid" << endl;
		int i = rand() % childArray.size();
		Node* tmp = &childArray[i];
		return (tmp);
	};
	double	getChildScore(int i) { return (childArray[i]).getNodeWinScore(); }
	Node* getChildWithMaxScore() {
		if (childArray.size() > 0) {
			double max = (childArray[0]).getNodeWinScore();
			Node* Max = &childArray[0];
			for (int i = 1; i < childArray.size(); i++) {
				if ((childArray[i]).getNodeWinScore() > max) {
					max = (childArray[i]).getNodeWinScore();
					Max = &childArray[i];
				}
			}
			return Max;
		}
		else
		{
			Node* tmp = this;//get node parent and parent's move
			return tmp ;
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
			return 1.5;
		}
		// double sum = this->score*1.0 / this->visit + this->C * sqrt(2 * log(this->father->visit / this->visit));	
		return (((double)nodeWinScore / (double)nodeVisit) + 1.41 * sqrt(log(parentVisit) / (double)nodeVisit));
	}

	static Node* findBestNodeWithUCT(Node* node)
	{
		double tmp;
		int parentVisit = (*node).getNodeVisitCount();
		Node* child = (*node).getNodeChild(0);
		double max = uctValue(parentVisit, (*node).getChildScore(0), (*child).getNodeVisitCount());
		Node* Max = (*node).getNodeChild(0);
		for (int i = 1; i < (*node).getNodeChildNum(); i++) {
			child = (*node).getNodeChild(i);
			tmp = uctValue(parentVisit, (*node).getChildScore(i), (*child).getNodeVisitCount());
			if (tmp > max)//&&(*child).getNodeState().getAllPossibleState().size()!=0
			{
				Max = (*node).getNodeChild(i);
				max = tmp;
			}
			cout<<"(x,y):"<< (*child).getNodeState().getMove().getX()<<","<< (*child).getNodeState().getMove().getX();
			cout<<" u(x,y):"<<tmp<<endl;
		}
		return Max;
	}
};

class MonteCarloTreeSearch {
	double WIN_SCORE;
	int opponent;
	double t;

	// find the best node from the tree, using utility : UCT
	Node* selectPromisingNode(Node* rootNode) {
		Node* node = rootNode;
		// if child num != 0 means it isn't a leaf
		while ((*node).getNodeChildNum() != 0) {
			//cout << "--------------select with UCT-------------" << endl;
			UCT u;
			node = u.findBestNodeWithUCT(node);
			cout<<"Best:"<<(*node).getNodeState().getMove().getX()<<","<<(*node).getNodeState().getMove().getY()<<endl; 
		}
		return node;
	}

	void expandNode(Node* node) {
		cout<<"--------------goAllpossible-------------"<<endl;
		vector<State> possibleStates =  (*node).getNodeState().getAllPossibleState();
		cout << "*expanding-possibleState size:" << possibleStates.size() << endl;
		for (int i = 0; i < possibleStates.size(); i++) {
			Node newNode;
			newNode.setNodeState(possibleStates[i]);
			newNode.setNodeParent(node);
			// opponent in turn
			newNode.getNodeState().setStatePlayerNo((*node).getNodeState().getStateOpponent());
			cout<<"expand��node�������G"<<(*node).getNodeState().getStateOpponent()<<endl; 
			(*node).setNodeChild(newNode);
			//newNode.getNodeState().getStateboard().printboard();
		}
	}

	void backPropogation(Node* nodeToExplore, bool playerwin) {
		Node* tempNode = nodeToExplore;
		cout << "*back propogation start" << endl;
		while (!(*tempNode).getNoderoot()) {
			(*tempNode).addNodeVisitCount();
			//cout << "add VISITCOUNT" << endl;
			if (playerwin) {
				(*tempNode).addNodeWinScore();
			}
			cout<<"(x,y): "<<(*tempNode).getNodeState().getMove().getX()<<","<< (*tempNode).getNodeState().getMove().getY()<<" score:" <<(*tempNode).getNodeWinScore()<<endl;
			tempNode = (*tempNode).getNodeParent();
		}
		(*tempNode).addNodeVisitCount();
		if (playerwin) {
			(*tempNode).addNodeWinScore();
		}
		cout<<"(x,y): "<<(*tempNode).getNodeState().getMove().getX()<<","<< (*tempNode).getNodeState().getMove().getY()<<" score:" <<(*tempNode).getNodeWinScore()<<endl;
	}

	int simulateRandomPlayout(Node* node) {
		cout << "*sinulation play start" << endl;
		State tempState = (*node).getNodeState();
		//temp.getNodeState().set;
		//tempState.setStatePlayerNo(tempState.getStateOpponent());
		
		int boardStatus = tempState.getStateboard().checkStatus();
		if (boardStatus != -1) {//game end
			return boardStatus;
		}
		while (boardStatus == -1) {//game is continueing
			tempState = tempState.randomPlay();
			boardStatus = tempState.getStateboard().checkStatus();
			tempState.getStateboard().printboard();
		}
		tempState.getStateboard().printboard();
		return boardStatus;
	}



public:

	MonteCarloTreeSearch() { WIN_SCORE = 1; t = clock(); }
	Position findNextMove(Board board, int playerNo) {
		// define an end time which will act as a terminating condition
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
		cout<<"root:"<<opponent<<endl;
		//rootNode.getNodeState().getStateboard().printboard();
		//cout<<"root player:"<<rootNode.getNodeState().getStatePlayerNo()<<endl;
		int count = 1;
		while ((clock() - t) / CLOCKS_PER_SEC < 5 /*count>0*/) {
			//selection
			//cout << "----------select---------" << endl;
			Node* promisingNode = selectPromisingNode(&rootNode);
			//(*promisingNode).getNodeState().getStateboard().printboard();
			//cout << "parent address:" << (*promisingNode).getNodeParent() << endl;
			//cout << "my address:" << promisingNode << endl;

			//expand
			if ((*promisingNode).getNodeState().getStateboard().checkStatus() == -1)// the game is not over
			{
				//cout << "expand" << endl;
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
				if (playerNo == playoutResult){
					backPropogation(nodeToExplore, true);
					//cout << "win" << endl;
				}
				else
					backPropogation(nodeToExplore, false);
				//cout << rootNode.getNodeVisitCount() << "," << rootNode.getNodeWinScore();
				//cout << "end a while" << endl;
			}
			else{
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



int main()
{
	Board board;
	int test1[8][8]={0};
	test1[0][0] = -1;
	test1[0][7] = -1;
	test1[7][0] = -1;
	test1[7][7] = -1;
	/*
	int test1[8][8] = {0,1,2,2,2,2,2,-1,
						,1,1,2,2,1,1,1,
						1,1,2,2,2,2,1,1,
						1,1,1,1,1,1,1,1,
						2,2,2,2,1,2,1,2,
						2,1,2,1,2,1,2,2,
						2,2,2,2,2,2,2,2,
					   -1,1,2,2,2,0,0,-1};*/
	int test2[8][8] = {-1,2,1,1,1,2,2,-1,
						1,2,1,1,1,2,2,2,
						1,2,1,1,2,1,1,2,
						2,2,2,2,1,2,1,2,
						2,2,2,2,2,1,2,2,
						0,2,1,1,1,2,2,2,
						2,2,2,2,2,2,2,0,
						-1,0,2,2,0,0,0,-1};
	Position step;
	int playerno = 2;//black
	int count = 2;
	int i, j, v;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board.setBoardValues(i, j, test1[i][j]);
		}
	}
	board.printboard();
	MonteCarloTreeSearch mcts;
	step = mcts.findNextMove(board, playerno);
	cout << "next step: (" << step.getX() << "," << step.getY() << ")" << endl;
	return 0;
}


/*
std::vector<int> GetStep(std::vector<std::vector<int>>& board, bool is_black) {
	
	MonteCarloTreeSearch mcts;
	Board boards;
	Position step;
	vector<int> steps;
	int playerno;
	if (is_black)
		playerno = 1;
	else
		playerno = 2;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			boards.setBoardValues(i, j, board[i][j]);
		}
	}
	//boards.printboard();
	step = mcts.findNextMove(boards, playerno);
	steps.resize(2);
	steps[0] = step.getX();
	steps[1] = step.getY();
	return steps;
}

int main() {
	int id_package;
	std::vector<std::vector<int>> board;
	std::vector<int> step;
	ini();
	bool is_black;
	while (true) {
		if (GetBoard(id_package, board, is_black))
			break;

		step = GetStep(board, is_black);
		SendStep(id_package, step);
	}
}


*/

