#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include<vector>
#include<stack>
#include<queue>
#include<time.h>
#include<iomanip>
#define N 6
using namespace std;

/*
	author : 蔡怡君
	content : Minesweeper using Backtrack Search 
*/

// mine map
int mine[N][N],row,column;


struct node
{
	int assigned_node;
	int TNT;
	int domain[N][N]; // domain's number {0,1} = 2
	int value[N][N]; // value : 0 or 1 (have mines)
}first;


struct point
{
	int x,y;
	int degree;
}dir[8];

vector<point>hint;

//point **goal;

void initial()
{
	dir[0]={-1,-1};
	dir[1]={-1,0};
	dir[2]={-1,1};
	dir[3]={0,-1};
	dir[4]={0,1};
	dir[5]={1,-1};
	dir[6]={1,0};
	dir[7]={1,1};
	
	first.assigned_node = 0;
	first.TNT = 0;
	for(int i = 0 ; i < row ; i ++){
		for(int j = 0 ; j < column ; j ++){
			if(mine[i][j] == -1){
				first.domain[i][j] = 2;
				first.value[i][j] = -1;
			}
				
			else 
			{   // means hint
				point h = {i,j};
				hint.push_back(h);
				first.domain[i][j] = 1;
				first.value[i][j] = mine[i][j];
			}	
		}
	}
}

bool operator == (const node &p1,const node &p2)
{ 	
	if(p1.TNT != p2.TNT )
		return false;
	for(int i = 0 ; i < row ; i ++){
		for(int j = 0 ; j < column ; j ++){
			if(p1.domain[i][j] != p2.domain[i][j])
				return false;
			if(p1.value[i][j] != p2.value[i][j])
				return false;
		}
	}
	return true;
}

bool isVaild(int i,int j)
{
	if(mine[i][j] != -1) return false;
	return ( (i >= 0 && i < row ) && (j >= 0 && j < column) );
}

node copy_node(node A)
{
	node tmp;
	for(int i = 0 ; i < row ;i ++){
		for(int j = 0 ; j < column ; j ++){
			tmp.domain[i][j] = A.domain[i][j];
			tmp.value[i][j] = A.value[i][j];
		}
	}
	tmp.assigned_node = A.assigned_node;
	tmp.TNT = A.TNT;
	return tmp;
}

bool checkVaild(node now, int num_TNT, vector<point> hhint)
{
	bool all_assigned = true;
	if(now.TNT > num_TNT)	return false;
	for(int i = 0 ; i < hhint.size() ; i ++)
	{
		// check hint 的八維 
		int TNT = 0;
		for(int j = 0 ; j < 8 ; j ++){
			int px = hhint[i].x + dir[j].x;
			int py = hhint[i].y + dir[j].y;
			if(isVaild(px , py))
			{
				if(now.value[px][py] == 1) // 放炸彈的地方 
					TNT ++ ;
				if(now.domain[px][py] != 1) // 如果八個方位都assigned完 
					all_assigned = false;
			} 
				
		}
		if(TNT > mine[hhint[i].x][hhint[i].y])
			return false;
		// 八個方位都 assigned了 卻不符合TNT -> false 	
		if(all_assigned == true && TNT != mine[hhint[i].x][hhint[i].y] ) 
			return false;
	}
	return true;
}

void print_solution(node result)
{
	for(int i = 0 ; i < row ;i ++){
		for(int j = 0 ; j < column ; j ++){
			if(mine[i][j] != -1)
				cout<<mine[i][j]<<" ";
			else if(result.value[i][j] == 1)
				cout<<"*"<<" ";
			else
				cout<<" "<<" ";
		}
		cout<<endl;
	}
}

// using for checking
void print_do(node result)
{
	cout<<"domain: \n";
	for(int i = 0 ; i < row ;i ++){
		for(int j = 0 ; j < column ; j ++){
				cout<<result.domain[i][j]<<" ";
		}
		cout<<endl;
	}
}

// using for checking
void print_value(node result)
{
	cout<<"value: \n";
	for(int i = 0 ; i < row ;i ++){
		for(int j = 0 ; j < column ; j ++){
			if(mine[i][j] == -1 && result.value[i][j] == 1)
				cout<<" *"<<" ";
			else
				cout<<setw(2)<<result.value[i][j]<<" ";
		}
		cout<<endl;
	}
}


bool check_solution(node result)
{
	for(int i = 0; i < row ; i ++){
		for(int j = 0 ; j < column ; j ++){
			if(mine[i][j] != -1)
			{	// local check 8 directions 周圍的TNT 
				int TNT=0;
				for(int k = 0; k < 8 ; k ++){
					if(isVaild(i + dir[k].x,j +dir[k].y))
						if(result.value[i + dir[k].x][j +dir[k].y ] == 1)
							TNT ++;
				}
				if(TNT != mine[i][j])
					return false;
			}
		}
	}
	return true;
}

bool isExplored(stack<node> explored , stack<node>front, node now)
{
	// 在 Explored set裡面 
	while(!explored.empty())
	{
		node current=explored.top();
		explored.pop();
		// 如果current == now 一模一樣 
		if(current == now)
		{
			//cout<<"the same"<<endl;
			return false;
		}
	}
	// 在 Frontier 裏頭 
	while(!front.empty())
	{
		node current = front.top();
		front.pop();
		// 如果current == now 一模一樣 
		if(current == now)
		{
			//cout<<"the same"<<endl;
			return false;
		}
	}
	return true;
}

bool isExplored(stack<node> explored , priority_queue<node>front, node now)
{
	// 在 Explored set裡面 
	while(!explored.empty())
	{
		node current=explored.top();
		explored.pop();
		// 如果current == now 一模一樣 
		if(current == now)
		{
			//cout<<"the same"<<endl;
			return false;
		}
	}
	// 在 Frontier 裏頭 
	while(!front.empty())
	{
		node current = front.top();
		front.pop();
		// 如果current == now 一模一樣 
		if(current == now)
		{
			//cout<<"the same"<<endl;
			return false;
		}
	}
	return true;
}

// original without forward checking / heuristic
void find_solution(int r , int c , int num)
{
	stack<node> front;
	stack<node> explored;
	int step = 1;
	front.push(first);
	//bool flag = true;
	while(!front.empty())
	{
		node current = front.top();
		explored.push(current);
		front.pop();
		//cout<<"layer9 :"<<step<<endl;
		for(int i = 0 ; i < r ; i ++){
			for(int j = 0 ; j < c ; j++){
				if(mine[i][j] == -1 && current.domain[i][j] == 2) // unassigned varaible 
				{
					//cout<<"fill in (x,y)"<<j<<","<<i<<endl;
					for(int option = 0; option < 2 ; option ++){
						node now = copy_node(current);
						now.domain[i][j] = 1;
						now.value[i][j] = option;
						if(option == 1)
							now.TNT++;
						if(checkVaild(now,num,hint)) // if 這個點有符合 17 constraints
						{
							// 沒有被explored過的才能加進去！ 
							if(isExplored(explored,front,now))
							{
								if(now.TNT == num){
									if(check_solution(now)){
										print_solution(now);
										cout<<"node Expand:"<<step<<endl;
										return;
									}
									else
										continue;
								}
								front.push(now);
								step++;
							}	
						}	
					}
				}
			}
		}
	}
	cout<<"No Solution!"<<endl;
}

void find_forward_solution(int r, int c, int num)
{
	stack<node> front;
	stack<node> explored;
	front.push(first);
	int step = 1;
	while(!front.empty())
	{
		node current = front.top();
		explored.push(current);
		front.pop();
		
		// --- forward checking
		
		bool flag = true;
		for(int i = 0 ; i < hint.size() ; i ++){
			int lowerbound = 0;
			int upperbound = 0;
			point no = hint[i];
			// direction
			//cout<<"forward x,y :"<< no.x<<","<<no.y<<endl;
			for(int j = 0 ; j < 8 ; j ++){
				int x = no.x + dir[j].x;
				int y = no.y + dir[j].y;
				if(!isVaild(x,y))
					continue;
				if ( current.domain[x][y] == 1 && current.value[x][y] == 1 )
					lowerbound ++;
				else if(current.domain[x][y] == 2) // unassigned
					upperbound ++;
			}
			upperbound += lowerbound;
			// lower bound & upper bound
			if( lowerbound > mine[no.x][no.y] )
			{
				flag = false;
				//cout<<"lowerbound > mine[no.x][no.y]"<<endl;
				break;
			}
			else if (lowerbound == mine [no.x][no.y])
			{
				// 代表其他unassinged的值 都要成為 0  
				for(int j = 0 ; j < 8 ; j ++){
					int x = no.x + dir[j].x;
					int y = no.y + dir[j].y;
					if(!isVaild(x,y))
						continue;
					if(current.domain[x][y] == 2){
						current.domain[x][y] = 1 ;
						current.value[x][y] = 0 ;
					}
				}
			}
			if( upperbound < mine[no.x][no.y])
			{
				flag = false;
				//cout<<"upperbound < mine[no.x][no.y]"<<endl;
				break;
			}
			else if( upperbound == mine[no.x][no.y])
			{
				// 代表其他 unassigned的值 都要變成 1 
				for(int j = 0 ; j < 8 ; j ++){
					int x = no.x + dir[j].x;
					int y = no.y + dir[j].y;
					if(!isVaild(x,y))
						continue;
					if(current.domain[x][y] == 2){
						current.domain[x][y] = 1 ;
						current.value[x][y] = 1 ;
						current.TNT++;
					}
				}
			}
			//cout<<"TNT:"<<current.TNT<<endl;
			//print_value(current);
		}
		// 代表這個state不滿足 state 繼續pop 
		if(!flag) continue;
		
		// 檢查是不是答案 
		if( current.TNT == num )
		{
			if(check_solution(current)){
				print_solution(current);
				cout<<"node Expand:"<<step<<endl;
				return;
			}
			continue;
		}
		// to assign value！ 
		for(int i = 0 ; i < r ; i ++){
			for(int j = 0 ; j < c ; j++){
				if(mine[i][j] == -1 && current.domain[i][j] == 2) // unassigned varaible 
				{
					//cout<<"fill in (x,y)"<<j<<","<<i<<endl;
					// 去給這 value 是 1(mine) 還是 0 
					for(int option = 1; option >= 0  ; option --){
						node now = copy_node(current);
						now.domain[i][j] = 1;
						now.value[i][j] = option;
						if(option == 1)
							now.TNT++;
						if(checkVaild(now,num,hint)) // if 這個點有符合 17 constraints
						{
							// 沒有被explored過的才能加進去！ 	
							if(isExplored(explored,front,now))
							{
								// 如果TNT數量 == 炸彈的數量 檢查solution 
								if( now.TNT == num ){
									if(check_solution(now)){
										print_solution(now);
										cout<<"node Expand:"<<step<<endl;
										return;
									}
									else
										//cout<<"**************************************"<<endl;
									continue;
								}
								front.push(now);
								step++;
							}	
						}	
					}
				}
			}
		}
	}
	
}

// using for priority queue  
bool operator < (const point &p1,const point &p2){ return p1.degree < p2.degree;}
bool operator > (const point &p1,const point &p2){ return p1.degree > p2.degree;}

// forward function with Degree Heuristic
void find_forward_solution_Degree(int r, int c, int num)
{
	stack<node> front;
	stack<node> explored;
	front.push(first);
	int step = 1;
	while(!front.empty())
	{
		node current = front.top();
		explored.push(current);
		front.pop();
		
		// --- priorty_queue using for degree heuristic
		priority_queue<point,vector<point>,greater<point> > de;
		for(int i = 0 ; i < r ; i ++){
			for(int j = 0 ; j < c ; j ++){
				if(mine[i][j] != -1) continue;
				point p = { i , j , 0};
				for(int k = 0 ; k < 8 ; k ++)
					if(isVaild( i + dir[k].x , j + dir[k].y ))
						if(mine[i + dir[k].x][j + dir[k].y] != -1)
							p.degree ++;
				de.push(p);
			}
		}
		// to assign value！ 
		while(!de.empty()){
			point n = de.top();
			de.pop();
			int i = n.x, j = n.y;
			
			if(mine[i][j] == -1 && current.domain[i][j] == 2) // unassigned varaible 
			{
				// 去給這 value 是 1(mine) 還是 0 
				for(int option = 1; option >= 0  ; option --){
					node now = copy_node(current);
					now.domain[i][j] = 1;
					now.value[i][j] = option;
					if(option == 1)
						now.TNT++;
					// // 沒有被explored過的才能加進去！ 且 if 這個點有符合 17 constraints
					if(checkVaild(now,num,hint)&&isExplored(explored,front,now)) 
					{
						// 如果TNT數量 == 炸彈的數量 檢查solution 
						if( now.TNT == num ){
							if(check_solution(now)){
								print_solution(now);
								cout<<"node Expand:"<<step<<endl;
								return;
							}
							else
								continue;
						}
						front.push(now);
						step++;
					}	
				}
			}
		}
	}
	
}


bool operator < (const node &p1,const node &p2){ return p1.TNT < p2.TNT;}
bool operator > (const node &p1,const node &p2){ return p1.TNT > p2.TNT;}

// forward function with TNT Heuristic
void find_forward_solution_TNT	(int r, int c, int num)
{
	priority_queue<node> front;
	stack<node> explored;
	front.push(first);
	int step =1;
	while(!front.empty())
	{
		node current = front.top();
		explored.push(current);
		front.pop();
		if(current.TNT == num)
			for(int i = 0 ; i < r; i ++)
				for(int j = 0 ; j <c ; j ++)
					if(current.domain[i][j] == 2){
						current.domain[i][j] = 1;
						current.value[i][j] = 0;
					}
					
		// to assign value！ 
		for(int i = 0 ; i < r ; i ++){
			for(int j = 0 ; j < c ; j++){
				if(mine[i][j] == -1 && current.domain[i][j] == 2) // unassigned varaible 
				{
					//cout<<"fill in (x,y)"<<j<<","<<i<<endl;
					// 去給這 value 是 1(mine) 還是 0 
					for(int option = 1; option >= 0  ; option --){
						node now = copy_node(current);
						now.domain[i][j] = 1;
						now.value[i][j] = option;
						if(option == 1)
							now.TNT++;
						if(checkVaild(now,num,hint)) // if 這個點有符合 17 constraints
						{
							// 沒有被explored過的才能加進去！ 	
							if(isExplored(explored,front,now))
							{
								// 如果TNT數量 == 炸彈的數量 檢查solution 
								if( now.TNT == num ){
									if(check_solution(now)){
										print_solution(now);
										cout<<"node Expand:"<<step<<endl;
										return;
									}
									else
										continue;
								}
								front.push(now);
								step++;
							}	
						}	
					}
				}
			}
		}
	}
	
}

// forward function with MRV Heuristic
void find_forward_solution_MRV(int r, int c, int num)
{
	stack<node> front;
	stack<node> explored;
	front.push(first);
	int step = 1;
	while(!front.empty())
	{
		node current = front.top();
		explored.push(current);
		front.pop();
		
		// --- priorty_queue using for degree heuristic
		priority_queue<point,vector<point>,greater<point> > de;
		for(int i = 0 ; i < r ; i ++){
			for(int j = 0 ; j < c ; j ++){
				point p = { i , j , current.domain[i][j] };
				de.push(p);
			}
		}
		
		
		// to assign value！ 
		while(!de.empty()){
			point n = de.top();
			de.pop();
			int i = n.x, j = n.y;
			
			if(mine[i][j] == -1 && current.domain[i][j] == 2) // unassigned varaible 
			{
				// 去給這 value 是 1(mine) 還是 0 
				for(int option = 1; option >= 0  ; option --){
					node now = copy_node(current);
					now.domain[i][j] = 1;
					now.value[i][j] = option;
					if(option == 1)
						now.TNT++;
					// // 沒有被explored過的才能加進去！ 且 if 這個點有符合 17 constraints
					if(checkVaild(now,num,hint)&&isExplored(explored,front,now)) 
					{
						// 如果TNT數量 == 炸彈的數量 檢查solution 
						if( now.TNT == num ){
							if(check_solution(now)){
								print_solution(now);
								cout<<"node Expand:"<<step<<endl;
								return;
							}
							else
								continue;
						}
						front.push(now);
						step ++;
					}	
				}
			}
		}
	}	
}


int main()
{
	int num_mines;
	time_t start,end;
	double t;
	
	cin >> row >> column >> num_mines;
	
	for( int i = 0 ; i < row ; i ++ )
		for ( int j = 0 ; j < column ; j ++ )
			cin>>mine[i][j];
			
	initial();
	
	/*
	//---- original without forward & heuristic
	start = clock();
	find_solution(row,column,num_mines);
	end = clock();
	t = ((double)(end-start))/CLOCKS_PER_SEC;
	printf("Time : %fs\n",t);	*/
	
	//----  forward checking without heuristic
	cout<<"forward checking without heuristic\n";
	start = clock();
	find_forward_solution(row,column,num_mines);
	end = clock();
	t = ((double)(end-start))/CLOCKS_PER_SEC;
	printf("Time : %fs\n",t);	
	
	
	//----   with Degree heuristic
	cout<<"with Degree heuristic\n";
	start = clock();
	find_forward_solution_Degree(row,column,num_mines);
	end = clock();
	t = ((double)(end-start))/CLOCKS_PER_SEC;
	printf("Time : %fs\n",t);	
	
	
	
	//----  with uesr-defined heuristic
	cout<<"with TNT heuristic\n";
	start = clock();
	find_forward_solution_TNT(row,column,num_mines);
	end = clock();
	t = ((double)(end-start))/CLOCKS_PER_SEC;
	printf("Time : %fs\n",t);
	
	//----   with MRV heuristic
	cout<<"with MRV heuristic\n";
	start = clock();
	find_forward_solution_MRV(row,column,num_mines);
	end = clock();
	t = ((double)(end-start))/CLOCKS_PER_SEC;
	printf("Time : %fs\n",t);	
	
	return 0;
}

