#include<iostream>
#include<string.h>
#include<stdio.h>
#include<queue>
#include<stdlib.h>
#include<algorithm>

using namespace std;


struct point
{
	int x,y;
	int cost; // use for heuristic function
}p1,p2,dir[8];

bool operator < (const point &p1,const point &p2){ return p1.cost < p2.cost;}
bool operator > (const point &p1,const point &p2){ return p1.cost > p2.cost;}

int main()
{
	priority_queue<point,vector<point>,greater<point> >front;
	point t1,t2;
	t1.x=0; t1.y=2; t1.cost=20;
	t2.x=1; t2.y=2; t2.cost=2;
	front.push(t1);
	front.push(t2);
	while(!front.empty())
	{
		point tmp = front.top();
		cout<<tmp.x<<" "<<tmp.y<<endl;
		front.pop();
	}
	return 0;
}

