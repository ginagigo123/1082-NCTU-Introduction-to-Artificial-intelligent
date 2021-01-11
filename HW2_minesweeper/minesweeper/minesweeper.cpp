/*
** Author:0613414 鄭程哲 
** Programming Assignment #2 
*/

#include<iostream>
#include<stdlib.h>
#include<stack> 
using namespace std;
#define N 100

//node structure
struct node{
 int domain[N][N],value[N][N];
 int assigned,mines;
};

//function prototype
void printBoard(int**,int,int);
void mineSweeper();
bool isLegal(bool);
bool isOptimal(node);
node copyNode(node);

//global variables
int WIDTH,HEIGHT,MINES,hint[N][N];

int main()
{
 cin>>WIDTH>>HEIGHT>>MINES;
    for(int i=0;i<HEIGHT;i++)
  for(int j=0;j<WIDTH;j++)
   cin>>hint[i][j];
 
 mineSweeper();
 
 return 0;
}
//-------------------------------------------------------------------------------------------
void printBoard(int board[N][N]){
    for(int i=0;i<HEIGHT;i++){
  for(int j=0;j<WIDTH;j++){
   cout<<board[i][j]<<" ";   
  }
  cout<<endl;
 }
 cout<<endl;
}

node copyNode(node n){
 node tmp;
 tmp.assigned = n.assigned;
 tmp.mines = n.mines;
 for(int i=0;i<HEIGHT;i++){
  for(int j=0;j<WIDTH;j++){
   tmp.domain[i][j] = n.domain[i][j];
   tmp.value[i][j] = n.value[i][j];
  }
 }
 return tmp;
}

bool isLegal(node n){
 int mines = 0;
 for(int i=0;i<HEIGHT;i++){
  for(int j=0;j<WIDTH;j++){
   if(n.value[i][j]==1)
    mines++;
   if(mines>MINES)
    return false;
   if(hint[i][j]!=-1){
    //check 周圍8格 
    int localMines = 0;
    int px[8]={-1,0,1,-1,1,-1,0,1};
    int py[8]={-1,-1,-1,0,0,1,1,1};
    for(int k=0;k<8;k++){
     if(j+px[k]>=0 && j+px[k]<WIDTH && i+py[k]>=0 && i+py[k]<HEIGHT)
      localMines+=n.value[i+py[k]][j+px[k]];
    }
    if(localMines>hint[i][j])
     return false;
   }
  }
 }
 return true;
}

bool isOptimal(node n){
 if(n.assigned!=WIDTH*HEIGHT || n.mines!=MINES)
  return false;
 for(int i=0;i<HEIGHT;i++){
  for(int j=0;j<WIDTH;j++){
   if(hint[i][j]!=-1){
    int localMines = 0;
    int px[8]={-1,0,1,-1,1,-1,0,1};
    int py[8]={-1,-1,-1,0,0,1,1,1};
    for(int k=0;k<8;k++){
     if(j+px[k]>=0 && j+px[k]<WIDTH && i+py[k]>=0 && i+py[k]<HEIGHT)
      localMines+=n.value[i+py[k]][j+px[k]];
    }
    if(localMines>hint[i][j] || localMines<hint[i][j])
     return false;
   }
  }
 }
 return true;
}

void mineSweeper(){
 
 //create root
 bool found = false;
 stack<node> s;
 node root;
 root.assigned = 0;
 root.mines = 0;
 for(int i=0;i<HEIGHT;i++){
  for(int j=0;j<WIDTH;j++){
   if(hint[i][j]!=-1){
    root.domain[i][j] = 1;
    root.value[i][j] = 0; //no mine
    root.assigned++;
   }
   else{
    root.domain[i][j] = 2;
    root.value[i][j] = -1; 
   }
  }
 }
 
 s.push(root);
 while(!found){
  node current = s.top();
  s.pop();
  for(int i=0;i<HEIGHT;i++){
   for(int j=0;j<WIDTH;j++){
    node tmp = copyNode(current);
    //printf("(%d,%d) #mines:%d\n",i,j,tmp.mines);
    if(tmp.domain[i][j]==2 && !found){ //unassigned
     tmp.domain[i][j] = 1;
     tmp.assigned ++;
     
     tmp.value[i][j] = 0; 
     if(isLegal(tmp)){
      if(isOptimal(tmp)){//terminal state
       printBoard(hint);
       printBoard(tmp.value);
       found=true;
      }
      s.push(tmp);
      //printf("AV:%d\tPush(%d,%d) with mine=%d\n",tmp.assigned,i,j,tmp.value[i][j]);
      //printBoard(tmp.value);
     }
     tmp.value[i][j] = 1;
     if(isLegal(tmp)){
      tmp.mines++;
      if(isOptimal(tmp)){//terminal state
       printBoard(hint);
       printBoard(tmp.value);
       found=true;
      }
      s.push(tmp);
      //printf("AV:%d\tPush(%d,%d) with mine=%d\n",tmp.assigned,i,j,tmp.value[i][j]);
      //printBoard(tmp.value);
     }
    }
   }
  }
 }
 
}
