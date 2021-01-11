#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<queue>
#include<stack>
#include<vector>
#include<time.h>
#define N 100
using namespace std;

/*
	author : 0613413 蔡怡君
	content: knight in a 8*8 chessboard
			move only allow (+-1,+-2)or(+-2,+-1)
			find optimal path from 2 given locations 
*/

struct point
{
	int x,y;
	int cost; // use for heuristic function
}p1,p2,dir[8];

// parent 紀錄這一個node上面的爸爸是誰
// table 紀錄這一個點有沒有被visited過 
// low cost using for IDA* 紀錄目前最小的cost 
point parent[N][N];
int table[N][N]={0},lowcost=1,node=1;

void initial()
{
	dir[0]={1,2};
	dir[1]={1,-2};
	dir[2]={-1,2};
	dir[3]={-1,-2};
	dir[4]={2,1};
	dir[5]={2,-1};
	dir[6]={-2,1};
	dir[7]={-2,-1};
}

void clear()
{
	// reset table & parent
	node = 0;
	for(int i = 0 ; i < N ; i++){
		for(int j = 0 ; j < N ; j++){
			table[i][j] = 0;
			parent[i][j].x = -1;
			parent[i][j].y = -1;
		}
	}
		
	
}

void print_path(point now)
{
	if(parent[now.x][now.y].x == now.x && parent[now.x][now.y].y == now.y)
	{
		cout<<"("<<now.x<<","<<now.y<<")";
		return;
	}	
	else
		print_path(parent[now.x][now.y]);
	cout<<"("<<now.x<<","<<now.y<<")";
}

bool operator < (const point &p1,const point &p2){ return p1.cost < p2.cost;}
bool operator > (const point &p1,const point &p2){ return p1.cost > p2.cost;}

int BFS(point p1,point p2)
{
	node = 1;
	queue<point>front;
	front.push(p1);
	table[p1.x][p1.y]=1;
	parent[p1.x][p1.y]=p1; // root的parent=自己  
	while(!front.empty())
	{
		point current = front.front();
		front.pop();
		// Look 8 directions and checked whether it's visited ?
		for(int i = 0 ; i < 8 ; i++){
			point now = { current.x + dir[i].x , current.y + dir[i].y };
			if(now.x < 0 || now.y < 0 || now.x > N-1 || now.y > N-1) // out of index
				continue;
			if(table[now.x][now.y] == 0) // mean 還沒被走過 
			{
				front.push(now);
				node++;
				table[now.x][now.y] = 1;
				parent[now.x][now.y] = current;
				// if we find point! print path and break.
				if(now.x == p2.x && now.y == p2.y)
				{
					cout<<"--- Find way ---\npath:";
					print_path(front.back());
					return node;
				}
			} 
		}
	}
}

// 找到 node 時檢查explored set & frontier (單純使用 table去記這點是否被visited) 
int DFS(point p1,point p2)
{
	int i,depth[N][N] = {0}; node = 1;
	stack<point>front;
	front.push(p1);
	table[p1.x][p1.y] = 1;
	parent[p1.x][p1.y] = p1; // root的parent=自己 
	while(!front.empty())
	{
		point current = front.top();
		front.pop();
		//printf("(%d,%d)",current.x,current.y);
		for(i = 0 ; i < 8 ; i ++){
			point now = { current.x + dir[i].x , current.y + dir[i].y};
			if(now.x < 0 || now.y < 0 || now.x > N-1 || now.y > N-1)
				continue;
			if(table[now.x][now.y] && depth[now.x][now.y] > depth[current.x][current.y]+1)
			{
				table[now.x][now.y]=0;
			}
			if( table[now.x][now.y] == 0)
			{
				table[now.x][now.y] = 1;
				front.push(now);
				parent[now.x][now.y] = current;
				depth[now.x][now.y] = depth[current.x][current.y] + 1;
				node++;
				if(now.x == p2.x && now.y == p2.y) // means arrive 
				{
					cout<<"--- Find way ---\npath:";
					print_path(front.top());
					return node;
				}
			}
		}
	}
	return 0;
}

// -- Depth-limited Search
int DLS(point p1,point p2,int d)
{
	// add depth[][] to memorize depth of the node
	int i, depth[N][N]={0};
	node = 1;
	stack<point>front;
	front.push(p1);
	table[p1.x][p1.y]=1;
	parent[p1.x][p1.y] = p1; // root的parent=自己 
	//printf("\n\ndepth limit:%d\n",d);
	while(!front.empty())
	{
		point current = front.top();
		front.pop();
		// control depth 
		if(depth[current.x][current.y] == d)
			continue;
		for(i = 0 ; i < 8 ; i ++){
			point now = { current.x + dir[i].x , current.y + dir[i].y};
			if(now.x < 0 || now.y < 0 || now.x > N-1 || now.y > N-1)
				continue;
			// 新的點 之前被走過但之前的深度比較大 現今走的depth比較小 所以要再丟入frontier一次 
			if(table[now.x][now.y] && depth[now.x][now.y] > depth[current.x][current.y]+1)
			{
				table[now.x][now.y]=0;
			} 
			if( table[now.x][now.y] == 0)
			{
				table[now.x][now.y] = 1;
				front.push(now);
				parent[now.x][now.y] = current;
				depth[now.x][now.y] = depth[current.x][current.y]+1; // depth = 原來的點 +1 
				node++;
				if(now.x == p2.x && now.y == p2.y) // means arrive 
				{
					cout<<"--- Find way ---\npath:";
					print_path(front.top());
					return node;
				}
				// printf("depth=%d. (%d,%d)",depth[now.x][now.y],now.x,now.y);
			}
		}
	}
	return 0; // if it's not found return 0
}

// iterative-deepening search
int IDS(point p1,point p2)
{
	// i for control depth
	int result = 0, i = 1 ,lowcost = 1, sum =0;
	while(1){
		clear();
		result = DLS(p1,p2,i++);
		sum += node;
		if(result != 0)
			return sum;
	}
	return 0;
}

int h(point n,point p2)
{
	int x,y;
	x = (n.x > p2.x) ? n.x - p2.x : p2.x - n.x;
	y = (n.y > p2.x) ? n.y - p2.y : p2.y - n.y;
	if(n.x == p2.x && n.y == p2.y)
	{
		x=-6666; y=-6666; 
	}
	return floor((x+y)/3);
}

// A*
int Astar(point p1,point p2)
{
	int depth[N][N] = {0}; node=1;
	priority_queue<point,vector<point>,greater<point> >front;
	front.push(p1);
	table[p1.x][p1.y]=1;
	parent[p1.x][p1.y]=p1;
	while(!front.empty())
	{
		point current = front.top();
		front.pop();
		for(int i = 0 ; i < 8 ; i ++){
			point now = { current.x + dir[i].x, current.y + dir[i].y};
			if(now.x < 0 || now.y < 0 || now.x > N-1 || now.y > N-1)
				continue;
			if(table[now.x][now.y] == 0)
			{
				table[now.x][now.y] = 1;
				depth[now.x][now.y] = depth[current.x][current.y] + 1;
				parent[now.x][now.y] = current;
				node ++;
				now.cost = depth[now.x][now.y] + h(now,p2);
				front.push(now);
				//printf("depth=%d. (%d,%d) cost=%d\n",depth[now.x][now.y],now.x,now.y,now.cost);
				if ( now.x == p2.x && now.y == p2.y )
				{
					cout<<"--- Find way ---\npath:";
					print_path(front.top());
					return node;
				}
				//cout<<"now.cost"<<depth[now.x][now.y]<<"+"<<h(now,p2)<<"="<<now.cost<<endl;
			}
		}
	}
}

// using for IDA*
// Deepening Limited A*

int DLAstar(point p1,point p2)
{
	// cost = 第一次跑DLAstar的 low cost; 
	int depth[N][N] = {0},cost = lowcost;
	node = 1;
	bool flag = false;
	
	priority_queue<point,vector<point>,greater<point> >front;
	front.push(p1);
	table[p1.x][p1.y]=1;
	parent[p1.x][p1.y]=p1;
	while(!front.empty())
	{
		point current = front.top();
		front.pop();
		for(int i = 0 ; i < 8 ; i ++){
			point now = { current.x + dir[i].x, current.y + dir[i].y};
			if(now.x < 0 || now.y < 0 || now.x > N-1 || now.y > N-1)
				continue;
			if(table[now.x][now.y] == 0)
			{
				table[now.x][now.y] = 1;
				depth[now.x][now.y] = depth[current.x][current.y] + 1;
				parent[now.x][now.y] = current;
				now.cost = depth[now.x][now.y] + h(now,p2);
				node ++;
				// push 之前要看這個點的 f() 是否超過 limited f()？ 
				if(now.cost > cost) 
				{
					// 紀錄第一次 大於low cost 的 flag 要用於下一次 					
					if(!flag){ lowcost = now.cost; flag = true; }
					// cout<<"the step lowcost:"<<lowcost<<endl;}
					//cout<<"超過limit 不push進去 -"; 
				}
				else
					front.push(now);
				//printf("(%d,%d) cost=%d\n",now.x,now.y,now.cost);
				if ( now.x == p2.x && now.y == p2.y )
				{
					cout<<"--- Find way ---\npath:";
					print_path(front.top());
					return node;
				}
			}
		}
	}
	return 0;
}

int IDAstar(point p1,point p2)
{
	int result = 0, i = 1, sum =0;
	lowcost = 1;
	while(1)
	{
		clear();
		result = DLAstar(p1,p2);
		sum += node; i++;
		//cout<<"第"<<i++<<"次try"<<"lower cost:"<<lowcost<<endl;
		if(result != 0)
		{
			cout<<"\n總共search "<<i<<"次";
			return sum;	
		}
	}
	
}


int DLAstar_stack(point p1,point p2)
{
	// cost = 第一次跑DLAstar的 low cost; 
	int depth[N][N] = {0},cost = lowcost;
	node = 1;
	bool flag = false;
	
	stack<point>front;
	front.push(p1);
	table[p1.x][p1.y]=1;
	parent[p1.x][p1.y]=p1;
	while(!front.empty())
	{
		point current = front.top();
		front.pop();
		for(int i = 0 ; i < 8 ; i ++){
			point now = { current.x + dir[i].x, current.y + dir[i].y};
			if(now.x < 0 || now.y < 0 || now.x > N-1 || now.y > N-1)
				continue;
			// 新的點 之前被走過但之前的深度比較大 現今走的depth比較小 所以要再丟入frontier一次 
			if(table[now.x][now.y] && depth[now.x][now.y] > depth[current.x][current.y]+1)
				table[now.x][now.y]=0;
			if(table[now.x][now.y] == 0)
			{
				table[now.x][now.y] = 1;
				depth[now.x][now.y] = depth[current.x][current.y] + 1;
				parent[now.x][now.y] = current;
				now.cost = depth[now.x][now.y] + h(now,p2);
				node ++;
				// push 之前要看這個點的 f() 是否超過 limited f()？ 
				if(now.cost > cost) 
				{
					// 紀錄第一次 大於low cost 的 flag 要用於下一次 					
					if(!flag){ lowcost = now.cost; flag = true;  }
					//cout<<"the step lowcost:"<<lowcost<<endl;
					//cout<<"超過limit 不push進去 -"; 
				}
				else
					front.push(now);
				//printf("(%d,%d) cost=%d\n",now.x,now.y,now.cost);
				if ( now.x == p2.x && now.y == p2.y )
				{
					cout<<"--- Find way ---\npath:";
					print_path(front.top());
					return node;
				}
			}
		}
	}
	return 0;
}

int IDAstar_stack(point p1,point p2)
{
	int result = 0, i = 1, sum =0;
	lowcost = 1;
	while(1)
	{
		clear();
		result = DLAstar_stack(p1,p2);
		sum += node; i++;
		//cout<<"第"<<i++<<"次try"<<"lower cost:"<<lowcost<<endl;
		if(result != 0)
		{
			cout<<"\n總共search "<<i<<"次";
			return sum;	
		}
	}
	
}


// only using for 印 node/ time 
void check(point p1,point p2)
{
	time_t start,end;
	double t;
	cout<<"BFS";
	start = clock();
	cout<<"\n # of nodes expanded is "<<BFS(p1,p2)<<endl;
	end = clock();
	t = ((double)(end-start))/CLOCKS_PER_SEC;
	printf(" Time : %fs\n",t);
	clear();

	cout<<"DFS";
	start = clock();
	cout<<"\n # of nodes expanded is "<<DFS(p1,p2)<<endl;
	end = clock();
	t = ((double)(end-start))/CLOCKS_PER_SEC;
	printf("Time : %fs\n",t);
	clear();

	cout<<"IDS";
	start = clock();
	cout<<"\n # of nodes expanded is "<<IDS(p1,p2)<<endl;
	end = clock();
	t = ((double)(end-start))/CLOCKS_PER_SEC;
	printf("Time : %fs\n",t);				
	clear();
	
	cout<<"A*";
	start = clock();
	cout<<"\n # of nodes expanded is "<<Astar(p1,p2)<<endl;
	end = clock();
	t = ((double)(end-start))/CLOCKS_PER_SEC;
	printf("Time : %fs\n",t);	
	clear();
	
	cout<<"IDA*";
	start = clock();
	cout<<"\n # of nodes expanded is "<<IDAstar(p1,p2)<<endl;
	end = clock();
	t = ((double)(end-start))/CLOCKS_PER_SEC;
	printf("Time : %fs\n",t);	
	clear();
}

int main()
{
	int type;
	initial();
	cout<<"--- Welcome to 8-kight ---\n";
	time_t start,end;
	double t;
	while(1)
	{
		cout<<"\n\nchioce 1:BFS 2:DFS 3:IDS 4:A* 5:IDA* 6.IDA* (using stack)\n";
		cout<<"plz input type of algorithm:";
		cin>>type;
		cout<<"plz input (x1,y1) (x2,y2):";
		cin>>p1.x>>p1.y>>p2.x>>p2.y;
		clear();
		//check(p1,p2);
		switch(type)
		{
			case 1:
				cout<<"BFS";
				start = clock();
				cout<<"\n # of nodes expanded is "<<BFS(p1,p2)<<endl;
				end = clock();
				t = ((double)(end-start))/CLOCKS_PER_SEC;
				printf(" Time : %fs",t);
				break;
			case 2:
				cout<<"DFS";
				start = clock();
				cout<<"\n # of nodes expanded is "<<DFS(p1,p2)<<endl;
				end = clock();
				t = ((double)(end-start))/CLOCKS_PER_SEC;
				printf("Time : %fs",t);
				break;
			case 3:
				cout<<"IDS";
				start = clock();
				cout<<"\n # of nodes expanded is "<<IDS(p1,p2)<<endl;
				end = clock();
				t = ((double)(end-start))/CLOCKS_PER_SEC;
				printf("Time : %fs",t);				
				break;
			case 4:
				cout<<"A*";
				start = clock();
				cout<<"\n # of nodes expanded is "<<Astar(p1,p2)<<endl;
				end = clock();
				t = ((double)(end-start))/CLOCKS_PER_SEC;
				printf("Time : %fs",t);	
				break;
			case 5:	
				cout<<"IDA*";
				start = clock();
				cout<<"\n # of nodes expanded is "<<IDAstar(p1,p2)<<endl;
				end = clock();
				t = ((double)(end-start))/CLOCKS_PER_SEC;
				printf("Time : %fs",t);	
				break;
			case 6:	
				cout<<"IDA* using stack";
				start = clock();
				cout<<"\n # of nodes expanded is "<<IDAstar_stack(p1,p2)<<endl;
				end = clock();
				t = ((double)(end-start))/CLOCKS_PER_SEC;
				printf("Time : %fs",t);	
				break;
			default:
				cout<<"----- warning:wrong input -----\n";
		}
	}
	return 0;
}

