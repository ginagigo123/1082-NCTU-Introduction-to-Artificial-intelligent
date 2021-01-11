#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<algorithm>

using namespace std;


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
	dir[3].setX(0);	dir[3].setY(-1);
	dir[4].setX(0);	dir[4].setY(1);
	dir[5].setX(1);	dir[5].setY(-1);
	dir[6].setX(1);	dir[6].setY(0);
	dir[7].setX(1);	dir[7].setY(1);
}

bool isValid(int x,int y){
	if( (x == 0 || x == 7) && (y == 0 || y == 7) )
		return false;
	return ((x >= 0 && x < 8 ) && (y >= 0 && y < 8));
}


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
	
	vector<Position> getPossiblePositions(int player, int opponent) {
		vector<Position> emptyPositions;
		//cout<<"get position function start"<<endl;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if( (i == 0 || i == 7) && (j == 0 || j == 7) )
					continue;
				if (boardValues[i][j] == 0) {
					if (i < 7 && i>0 && j < 7 && j>0)//如果是中間的都可以填 
					{
						Position p;
						p.setX(i);
						p.setY(j);
						emptyPositions.push_back(p);
					}
					else if (i == 0 && j != 0 && j != 7) {
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
	
	


int main()
{

	return 0;
}

