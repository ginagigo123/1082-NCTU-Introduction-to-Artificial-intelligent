#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include<stack>
#define N 8
using namespace std;

void change(int ** input)
{
	for(int i = 0 ; i < N ; i ++)
		for(int j = 0 ; j < N ; j ++)
			input[i][j] = 66;
	
}

struct node
{
	int domain[N][N]; // domain's number {0,1} = 2
	int value[N][N]; // value : 0 or 1 (have mines)
	node& operator=(node other)
	{
		for(int i = 0 ; i < N ; i ++){
			for(int j = 0 ; j < N ; j ++){
				domain[i][j] = other.domain[i][j];
				value[i][j] = other.value[i][j];
				return *this;
			}
		}
	}
}first,sec;

int main()
{
	int **t , **a;
	t = new int *[N];
	a = new int *[N];
	for(int i = 0 ; i < N ; i ++)
	{
		t[i] = new int [N];
		a[i] = new int [N];
	}	
	for(int i = 0 ; i < N ; i ++)
		for(int j = 0 ; j < N ; j++)
		{
			t[i][j]=0;
			a[i][j]=2;
		}
	stack<int **>front;
	front.push(t);
	front.push(a);
	while(!front.empty())
	{
		int ** tmp = front.top();
		int ** tmp1 = tmp;
		front.pop();
		change(tmp);
		for(int i = 0 ; i < N ; i ++){
			for(int j = 0 ; j < N ; j ++)
				cout<<tmp[i][j]<<" ";
			cout<<endl;
		}
		cout<<endl;
	}
	for(int i = 0 ; i < N ; i ++){
		for(int j = 0 ; j < N ; j ++){
			first.domain[i][j] = 22;
			first.value[i][j]=66;
		}
	}
		
	first = sec;
	cout<<"success\n";
	for(int i = 0 ; i < N ; i ++){
			for(int j = 0 ; j < N ; j ++)
				cout<<sec.domain[i][j]<<" ";
			cout<<endl;
		}
	return 0;
}

