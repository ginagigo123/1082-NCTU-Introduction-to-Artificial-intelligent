#include <iostream>
#include <bits/stdc++.h> 
#include <math.h>
#include <chrono>
using namespace  std;

int expand=1;//expand node
int h[12][12];
int s[12][12]={0};//constrain map
class Node
{
	int n[12][12];//assignment list
	int domain[144][2];//domain for variable
	int mines;//current assigned variable
	int doms;//current single domain
	int bomb;//current assign mines
	public:
		Node()//initialize
		{
			for(int i=0;i<144;i++)
			{
				domain[i][0]=0;
				domain[i][1]=1;
			}
			for (int i=0;i<12;i++)
			{
				for (int j=0;j<12;j++)
				{
					n[i][j]=-1;
				}
			}
			mines=0;
			doms=0;
			bomb=0;
		}
		int getn(int i,int j)
		{
			return n[i][j];
		}
		
		int checkd(int i,int j)
		{
			int num;
			num=i*12+j;
			if(domain[num][0]==0&&domain[num][1]==1)//domain=0,1
				return -1;
			else if(domain[num][0]==0)//domain=0
				return 0;
			else if(domain[num][1]==1)//domain=1
				return 1;
			else return 999;//empty
		}
		int getbomb()
		{
			return bomb;
		}
		int getm()
		{
			return mines;
		}
		int getdom()
		{
			return doms;
		}
		void setn(int i,int j,int value)
		{
			n[i][j]=value;
			mines+=1;
			if(value==1)
			{
				bomb+=1;
			}
		}
		
		void setd(int i,int j,int value)
		{
			int num;
			num=i*12+j;
			if(value==1)
			{
				domain[num][0]=-1;
				domain[num][1]=1;
				doms+=1;
			}
			else if(value==0)
			{
				domain[num][0]=0;
				domain[num][1]=-1;
				doms+=1;
			}
		}
		

};

void print(Node n)
{
	cout<<"-----------------------------------------------------------------------------"<<endl;
	for(int i=0;i<12;i++)
	{
		cout<<"| ";
		for(int j=0;j<12;j++)
		{
			if(h[i][j]!=-1)
			{
				cout<<"   "<<h[i][j]<<"\t| ";
			}
			else
			{
				if(n.getn(i,j)==1)
					cout<<"   X\t| ";
				else
					cout<<"\t| ";
			}
		}
		cout<<endl<<"-----------------------------------------------------------------------------"<<endl;
	}
}

struct heuristic//find more mines
{
	bool operator()(Node &x , Node &y)const
	{
		return (x.getbomb())<(y.getbomb());
	}
};

int fwacheck(Node *now)//forward checking
{
	int min,max;
	int hint;

//cout<<"foeward check"<<endl;
	min=max=0;
	for(int i=0;i<12;i++)
	{
		for (int j=0;j<12;j++)
		{
			hint=h[i][j];
			if(hint!=(-1))//find hint
			{
				min=max=0;
				for(int a=0;a<3;a++)//get bound
				{
					for(int b=0;b<3;b++)
					{
						if(i-1+a>=0&&j-1+b>=0&&j-1+b<12&&i-1+a<12)
						{
							if(now->checkd(i-1+a,j-1+b)==-1)
							{
								max+=1;
							}
							else
							{
								max+=now->checkd(i-1+a,j-1+b);
								min+=now->checkd(i-1+a,j-1+b);
							}
						}			
					}
				}
				//check bound
				if(min<hint&&hint<max)
					continue;
				else if(min>hint||max<hint)//no answer pop node
				{
					return 0;
				}
				else if(min==hint)//low bound=hint
				{
					for(int a=0;a<3;a++)
					{
						for(int b=0;b<3;b++)
						{
							if(i-1+a>=0&&j-1+b>=0&&j-1+b<12&&i-1+a<12)
							{
								if(now->checkd(i-1+a,j-1+b)==-1)
								{
									now->setd(i-1+a,j-1+b,0);
									//cout<<"set domain"<<i-1+a<<","<<j-1+b<<": 0"<<endl;
								}
							}
						}
					}
				}
				else if(max==hint)//large bound=hint
				{
					for(int a=0;a<3;a++)
					{
						for(int b=0;b<3;b++)
						{
							if(i-1+a>=0&&j-1+b>=0&&j-1+b<12&&i-1+a<12)
							{
								if(now->checkd(i-1+a,j-1+b)==-1)
								{
									now->setd(i-1+a,j-1+b,1);
									//cout<<"set domain"<<i-1+a<<","<<j-1+b<<": 1"<<endl;
								}
							}
						}
					}
				}
			
			}
		}
	}
	return 1;
}



int LCM(Node now,int i,int j)//for full domain test assigment use forward checking depend who to assign
{
	Node t1=now;
	Node t2=now;
	t1.setn(i,j,1);
	t1.setd(i,j,1);
	t2.setn(i,j,0);
	t2.setd(i,j,0);
	int a,b;
	a=fwacheck(&t1);
	b=fwacheck(&t2);
	
	if(a==0)
	{		
		return 0;
	}
	else if(b==0)
	{
		return 1;
	}
	else 
	{
		return -1;
	}
}

Node Minesweeper(Node n)
{
	//initial game tree
	vector<Node> tree;
	tree.push_back(n);
	make_heap(tree.begin(),tree.end(),heuristic());
	
	bool flag=true;
	Node now,child,ans;
	expand=1;
	while (!tree.empty())
	{
		now=tree.front();
		
		pop_heap(tree.begin(),tree.end(),heuristic());
		tree.pop_back();
		//if bomb all set other is 0;
		if(now.getbomb()==40)
		{
			child=now;
			for(int i=0;i<12;i++)
				for(int j=0;j<12;j++)
				{
					if(child.getn(i,j)==-1)
					{
						child.setn(i,j,0);
						child.setd(i,j,0);
					}
				}
			return child;
		}
		
		//forward checking
		if(fwacheck(&now)==0)//constrain default
		{
			pop_heap(tree.begin(),tree.end(),heuristic());
			tree.pop_back();
		}
		//generate child node
		else
		{
			//MRV
			if(flag)
			{
				for(int i=0;i<12;i++)
				{
					for(int j=0;j<12;j++)
					{
						int v;
						v=now.checkd(i,j);
						//cout<<"check domain:("<<i<<","<<j<<")"<<now.checkd(i,j)<<endl;
						if(v==0||v==1)//domain with only single value
						{
							if(now.getn(i,j)==-1)
							{
								//copy
								child=now;
								child.setn(i,j,v);
								//push child
								//cout<<"push:("<<i<<","<<j<<")"<<endl;
								tree.push_back(child);
								make_heap(tree.begin(),tree.end(),heuristic());
								expand+=1;
								flag=false;//don't need to go to degree & LCV
							}	
						}	
					}
				}
			}
			//degree
			if(flag)
			{
				int max=-1,a,b;
				for(int i=0;i<12;i++)
				{
					for(int j=0;j<12;j++)
					{
						if(max<s[i][j]&&now.getn(i,j)==-1)
						{
							max=s[i][j];
							a=i;
							b=j;
						}
					}
				}
				//cout<<"max"<<max<<"(i,j):("<<a<<","<<b<<")"<<endl;
				//LCM for domain isn't single
				if(max>=0)
				{
					int va;
					
					va=LCM(now,a,b);
					//cout<<"value:"<<va<<endl;
					if(va==-1)// 0,1 is both work
					{
						child=now;
						child.setn(a,b,0);
						child.setd(a,b,0);
						expand+=1;
						//push child
						//cout<<"push:("<<a<<","<<b<<") value:0"<<endl;
						tree.push_back(child);
						make_heap(tree.begin(),tree.end(),heuristic());
						child=now;
						child.setn(a,b,1);
						child.setd(a,b,1);
						expand+=1;
						//cout<<"push:("<<a<<","<<b<<") value:1"<<endl;
						tree.push_back(child);
						make_heap(tree.begin(),tree.end(),heuristic());
					}
					else//only one value work
					{
						//copy
						child=now;
						child.setn(a,b,va);
						child.setd(a,b,va);
						expand+=1;
						//push child
						//cout<<"push:("<<a<<","<<b<<") value:"<<va<<endl;		
						tree.push_back(child);
						make_heap(tree.begin(),tree.end(),heuristic());
					}
				}
			}
			flag=true;
		}
		ans=now;
		if(ans.getm()==144)//finished all assignment
		{
			return ans;
		}
			

	}
	return ans;
}


void newmap(int mode)
{
	if(mode==1)
	{
		//easy for 6*6 board 10 mines 16 hints
		srand(time(NULL));
		int count=10;
		int tmp;
		int map[6][6]={0};//ans
		
		for(int i=0;i<6;i++)
			for(int j=0;j<6;j++)
			{
				h[i][j]=-1;
			
			}	 
		while(count>0)
		{
			tmp=rand()%36;
			if(map[tmp/6][tmp%6]==0)
			{
				map[tmp/6][tmp%6]=1;
				count--;
			}
		}
		//build hint map
		count=16;
		int hnum;
		while(count>0)
		{
			tmp=rand()%36;
			if(map[tmp/6][tmp%6]==0&&h[tmp/6][tmp%6]==-1)//no mines no hint
			{
				hnum=0;
				for(int a=0;a<3;a++)
					for(int b=0;b<3;b++)
					{
						if((tmp/6)-1+a>=0&&(tmp%6)-1+b>=0 &&(tmp/6)-1+a<6&&(tmp%6)-1+b<6)
						{
							if(map[(tmp/6)-1+a][(tmp%6)-1+b]==1)
								hnum+=1;
						}			
					}
				h[tmp/6][tmp%6]=hnum;
				count--;
			}
		}

	}
	else if(mode==2)
	{
		//middle for 8*8 board 40 mines 64 hints
		srand(time(NULL));
		int count=18;
		int tmp;
		int map[8][8]={0};//ans		
		//initial hint map
		for(int i=0;i<8;i++)
		{
			for(int j=0;j<8;j++)
			{
				h[i][j]=-1;
			}
		}
		//set bomb
		while(count>0)
		{
			tmp=rand()%64;
			if(map[tmp/8][tmp%8]==0)
			{
				map[tmp/8][tmp%8]=1;
				count--;
			}
		}
		//build hint map
		count=28;
		int hnum;
		//random hint variable
		while(count>0)
		{
			tmp=rand()%64;
			if(map[tmp/8][tmp%8]==0&&h[tmp/8][tmp%8]==-1)//no mines no hint
			{
				hnum=0;
				for(int a=0;a<3;a++)
					for(int b=0;b<3;b++)
					{
						if((tmp/8)-1+a>=0&&(tmp%8)-1+b>=0 &&(tmp/8)-1+a<8&&(tmp%8)-1+b<8)
						{
							if(map[(tmp/8)-1+a][(tmp%8)-1+b]==1)
								hnum+=1;
						}			
					}
				h[tmp/8][tmp%8]=hnum;
				count--;
			}
		}
		//build hint map
	}
	else if(mode==3)
	{
		///hard for 10*10 bard 90 mines 144 hints
		srand(time(NULL));
		int count=28;
		int tmp;
		int map[10][10]={0};//ans
	
		for(int i=0;i<10;i++)
			for(int j=0;j<10;j++)
			{
				h[i][j]=-1;
			
			}
		//set bomb
		while(count>0)
		{
			tmp=rand()%100;
			if(map[tmp/10][tmp%10]==0)
			{
				map[tmp/10][tmp%10]=1;
				count--;
			}
		}
		//build hint map
		count=44;
		int hnum;
		//random hint variable
		while(count>0)
		{
			tmp=rand()%100;
			if(map[tmp/10][tmp%10]==0&&h[tmp/10][tmp%10]==-1)//no mines no hint
			{
				hnum=0;
				for(int a=0;a<3;a++)
					for(int b=0;b<3;b++)
					{
						if((tmp/10)-1+a>=0&&(tmp%10)-1+b>=0 &&(tmp/10)-1+a<10&&(tmp%10)-1+b<10)
						{
							if(map[(tmp/10)-1+a][(tmp%10)-1+b]==1)
								hnum+=1;
						}			
					}
				h[tmp/10][tmp%10]=hnum;
				count--;
			}
		}

	}
	else if(mode==4)
	{
		///hard for 12*12 bard 90 mines 144 hints
		srand(time(NULL));
		int count=40;
		int tmp;
		int map[12][12]={0};//ans
	
		for(int i=0;i<12;i++)
			for(int j=0;j<12;j++)
			{
				h[i][j]=-1;
			
			}
		//set bomb
		while(count>0)
		{
			tmp=rand()%144;
			if(map[tmp/12][tmp%12]==0)
			{
				map[tmp/12][tmp%12]=1;
				count--;
			}
		}
		//build hint map
		count=64;
		int hnum;
		//random hint variable
		while(count>0)
		{
			tmp=rand()%144;
			if(map[tmp/12][tmp%12]==0&&h[tmp/12][tmp%12]==-1)//no mines no hint
			{
				hnum=0;
				for(int a=0;a<3;a++)
					for(int b=0;b<3;b++)
					{
						if((tmp/12)-1+a>=0&&(tmp%12)-1+b>=0 &&(tmp/12)-1+a<12&&(tmp%12)-1+b<12)
						{
							if(map[(tmp/12)-1+a][(tmp%12)-1+b]==1)
								hnum+=1;
						}			
					}
				h[tmp/12][tmp%12]=hnum;
				count--;
			}
		}

	}
	
}

int main()
{
	//input:(board size, #mines, board (-1: variable)) 
	int mode,i,j;
  	Node root;
  	Node ans;
  	int re;
  	
	cout<<"enter board size mode(1~3):";
	cin>>mode;
	//cout<<"enter repete times:";
	//cin>>re;
	
	
	//while(re>0)
	//{
		//set up hint board
		getchar();
		Node root;
  		Node ans;
  		for(i=0;i<12;i++)
		{
			for (j=0;j<12;j++)
			{
				h[i][j]=0;
			}
		}
		newmap(mode);

		for(i=0;i<12;i++)
		{
			for (j=0;j<12;j++)
			{
				cout<<h[i][j]<<"\t";
			}
			cout<<endl;
		}
		cout<<endl;
		
		//initialize root node for hint domain=0
		for(i=0;i<12;i++)
		{
			for (j=0;j<12;j++)
			{
				if(h[i][j]!=(-1))
				{
					root.setd(i,j,0);
					root.setn(i,j,0);
					for(int a=0;a<3;a++)//set constrain map
					{
						for(int b=0;b<3;b++)
						{
							if(i-1+a>=0&&j-1+b>=0 && i-1+a<12&&j-1+b<12)
							{
								s[i-1+a][j-1+b]+=1;
							}			
						}
					}
				}	
			}
		}

		auto start =chrono::steady_clock::now();
		ans=Minesweeper(root);
		auto end =chrono::steady_clock::now();
		
		
		print(ans);	
		cout<< "It took "<<std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()<< " microseconds \n";
		cout<<"node expand:" <<expand<<endl;
		//re--;
	//}
	
	return 0;
} 
