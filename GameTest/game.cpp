#include<iostream>
#include<stdlib.h>
#include<vector>
#include<conio.h>
#include<math.h> 
#include<time.h>

#define SIZE 8
#define TIMES 20
#define C_PARA 1.4 
using namespace std;

class Node{
public:
	vector<vector<int> > state;
	Node *parent;
	vector<Node> child;
	int wins;
	int visits;
	bool isSampled;
	bool black;
	vector<int> step;//for first layer
	
	Node();
	Node(vector<vector<int> >,Node*,bool);
	bool isLeaf();
	double calculUCB();
	Node* maxUCB();
	double calculUCB1();//from root
	Node* maxUCB1();//from root
};

int pi[8]={-1,-1,-1,0,0,1,1,1};
int pj[8]={-1,0,1,-1,1,-1,0,1};
int h[8][8] = {  0,100,-25, 10, 10,-25,100,  0,
			   100,-25,  2,  0,  0,  2,-25,100,
			   -25,  2,  5,  2,  2,  5,  2,-25,
			    10,  0,  2,  1,  1,  2,  0, 10,
			    10,  0,  2,  1,  1,  2,  0, 10,
			   -25,  2,  5,  2,  2,  5,  2,-25,
			   100,-25,  2,  0,  0,  2,-25,100,
			     0,100,-25, 10, 10,-25,100,  0  };


void printBoard(vector<vector<int> >);
bool checkBoard(vector<vector<int> >);
int checkBoardSim(vector<vector<int> >);
int stuckWin(vector<vector<int> >);
void initBoard(vector<vector<int> >&);
vector<vector<int> > getAllStep(vector<vector<int> >&,bool);
vector<int> getStep(vector<vector<int> >&,bool);
void handleStep(vector<vector<int> >&,bool,vector<int>);
int simulation(Node*);

int main()
{
	srand(time(NULL));
	clock_t start, end;
	
	vector<vector<int> >board;
	bool black = true, stuck = false;
	
	initBoard(board);
	cout<<"======Initialize Board======\n";
	printBoard(board);
	
	while(true){
		vector<int> step;
		step.push_back(-1);
		step.push_back(-1);
		if(black){//my turn
			int trainCount = 0;
			start = clock();
			end = clock();
			
			Node root(board,NULL,black);
			
			//Init
			vector<vector<int> > allStep = getAllStep(board,black);
			for(int i=0;i<allStep.size();i++){
				vector<int> tmpStep = allStep[i];
				vector<vector<int> > tmpBoard = board;
				handleStep(tmpBoard,black,tmpStep);
				Node childN(tmpBoard,&root,black);
				childN.step = tmpStep;
				root.child.push_back(childN);
			}
			//There is at least one move for me
			if(allStep.size()>0){
				//Trainning
				//for(int times=0;times<TIMES;times++){
				while((end-start)/(double)(CLOCKS_PER_SEC) < 1){
					trainCount++;
					end = clock();
					//cout<<"Current last time:"<<(end-start)/(double)(CLOCKS_PER_SEC)<<endl;
					
					Node *current = root.maxUCB1();
					//selection
					while(!current->isLeaf()){
						current = current->maxUCB();
					}
					int result = checkBoardSim(current->state);
					if(result!=-1){//terminal state
						while(current!=NULL){//backpropagation
							current->visits += 1;
							if((result==1 && current->black) || (result==2 && !current->black))
								current->wins += 1;
							current = current->parent;
						}
						continue;
					}
					
					//expansion
					if(current->isSampled){
						vector<vector<int> > tmpallStep = getAllStep(current->state,!current->black);
						for(int i=0;i<tmpallStep.size();i++){
							vector<int> tmpStep = tmpallStep[i];
							vector<vector<int> > tmpBoard = current->state;
							handleStep(tmpBoard,!current->black,tmpStep);
							Node childNo(tmpBoard,current,!current->black);
							current->child.push_back(childNo);
						}
						if(tmpallStep.size()<=0){//no step for oppoent and not terimal
							tmpallStep = getAllStep(current->state,current->black);
							for(int i=0;i<tmpallStep.size();i++){
								vector<int> tmpStep = tmpallStep[i];
								vector<vector<int> > tmpBoard = current->state;
								handleStep(tmpBoard,current->black,tmpStep);
								Node childNo(tmpBoard,current,current->black);
								current->child.push_back(childNo);
							}
						}
						current = &current->child[0];//can modify to random pick
					}
					/*cout<<"Current to simulation--------------\n";
					current->black ? cout<<"[black]\n" : cout<<"[white]\n";
					printBoard(current->state);*/
					
					//simulation
					int reward = simulation(current);
					bool tmpBlack = current->black;
					current->isSampled = true;
					
					//backpropagation
					while(current!=NULL){
						current->visits += 1;
						if(tmpBlack==current->black)
							current->wins += reward;
						else
							current->wins += (reward+1) % 2;
						current = current->parent;
					}
					
					// print update status
					
				}
				//Choose optimal step
				cout<<"Train counts = "<<trainCount<<endl;
				int winMost = -1;
				step = root.child[0].step;
				for(int i=0;i<root.child.size();i++){
					if(root.child[i].wins>winMost){
						winMost = root.child[i].wins;
						step = root.child[i].step;
					}
				}
			}
		}		
		if(!black){//oppoent just randomly choose one step 
			/*int ci,cj;
			cin>>ci>>cj;
			step[0] = ci;
			step[1] = cj;*/
			step = getStep(board,black);
		} 
			
		if(stuck){
			if(step[0]==-1){
				int r = stuckWin(board); 
				if(r==1)
					cout<<"black win.\n";
				else if(r==2)
					cout<<"white win.\n";
				else
					cout<<"deuce.\n";
				break;
			}
			else if(step[0]!=-1)
				stuck = false;
		}
		if(step[0]==-1)
			stuck = true;
			
		handleStep(board,black,step);
		
		black? cout<<"[black]," : cout<<"[white], ";
		printf("(%d,%d)\n",step[0],step[1]);
		black = !black;
		printBoard(board);
		if(checkBoard(board))
			break;
	}
	
	return 0;
}
//-----------------------------------------------------------------------
Node::Node(){
	parent = NULL;
	black = isSampled = false;
	wins = 0;
	visits = 1;
}

Node::Node(vector<vector<int> > s, Node *p,bool b){
	state = s;
	parent = p;
	black = b;
	isSampled = false;
	wins = 0;
	visits = 1;
}

bool Node::isLeaf(){
	if(child.size()==0)
		return true;
	return false;
}

double Node::calculUCB1(){
	if(visits!=0)
		return wins/(visits*1.0) + C_PARA*sqrt(2.0 * log(parent->visits) / (visits*1.0) );
	else
		return 1.5;
}

Node* Node::maxUCB1(){
	Node *maxNode;
	maxNode = &child[0];
	double max = -999;
	for(int i=0;i<child.size();i++){
		double tmp = child[i].calculUCB1();
		//cout<<"Child "<<i<<" win:"<<child[i].wins<<" visits:"<<child[i].visits<<" pv:"<<visits <<" tmp ucb:"<<tmp<<endl;
		if(tmp>max){
			maxNode = &child[i];
			max = tmp;
		}
	}
	return maxNode;
}

double Node::calculUCB(){
	if(visits!=0)
		return -wins/(visits*1.0) + C_PARA * sqrt(2* log(parent->visits) / (visits*1.0) );
	else
		return 1.5;
}

Node* Node::maxUCB(){
	Node *maxNode;
	maxNode = &child[0];
	double max = -999;
	for(int i=0;i<child.size();i++){
		double tmp;
		if(child[i].black==black)
			tmp = child[i].calculUCB1();
		else
			tmp = child[i].calculUCB();
		//cout<<"Child "<<i<<" win:"<<child[i].wins<<" visits:"<<child[i].visits<<" pv:"<<visits <<" tmp ucb:"<<tmp<<endl;
		if(tmp>max){
			maxNode = &child[i];
			max = tmp;
		}
	}
	return maxNode;
}

//-----------------------------------------------------------------------
void printBoard(vector<vector<int> > board){
	cout<<"~";
	for(int i=0;i<SIZE;i++)
		cout<<"  "<<i;
	for(int i=0;i<board.size();i++){
		cout<<endl<<i;
		for(int j=0;j<board[i].size();j++){
			if(board[i][j]==0)
				cout<<"  -";
			else if(board[i][j]==1)
				cout<<"  X";
			else if(board[i][j]==2)
				cout<<"  O";
			else
				cout<<"   ";
				
		}
	}
	cout<<endl<<endl;
}

void initBoard(vector<vector<int> >&board){
	for(int i=0;i<SIZE;i++){
		vector<int> row;
		for(int j=0;j<SIZE;j++){
			if((j==0||j==SIZE-1) && (i==0||i==SIZE-1))
				row.push_back(-1);
			else
				row.push_back(0);
		}
		board.push_back(row);
	}
}

bool checkBoard(vector<vector<int> >board){
	int black = 0, white = 0;
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			if(board[i][j]==1)
				black++;
			else if(board[i][j]==2)
				white++;
		}
	}
	cout<<"[black]:"<<black<<"\t[white]:"<<white<<endl;
	if((black+white)==(SIZE*SIZE-4)){
		if(black>white)
			cout<<"black win.\n";
		else if(black<white)
			cout<<"white win.\n";
		else
			cout<<"deuce.\n";
		return true;
	}
	return false;
}

bool checkCanFlip(vector<vector<int> > board,int r,int c,bool black){
	for(int k=0;k<8;k++){
		int ri = r+pi[k];
		int rj = c+pj[k];
		if(ri>=0 && ri<SIZE && rj>=0 && rj<SIZE){
			if(board[ri][rj]==-1)
				continue;
			else if((board[ri][rj]==2 && black) || (board[ri][rj]==1 && !black)){//There is an oppoent piece nearby
				while(true){
					ri+=pi[k];
					rj+=pj[k];
					if(ri>=0 && ri<SIZE && rj>=0 && rj<SIZE){
						if(board[ri][rj]==0)
							break;
						else if((board[ri][rj]==2 && !black) || (board[ri][rj]==1 && black))
							return true;
						else if((board[ri][rj]==2 && black) || (board[ri][rj]==1 && !black))
							continue;
					}
					else
						break;
				}
			}
		}
	} 
	return false;
}

vector<vector<int> > getAllStep(vector<vector<int> >&board,bool black){
	vector<vector<int> >legalStep;
	vector<int>step;
	
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			vector<int> tmpstep;
			tmpstep.resize(2);
			tmpstep[0] = i;
			tmpstep[1] = j;
			if(board[i][j]!=0)
				continue;
			else if((j>0&&j<SIZE-1) && (i>0&&i<SIZE-1))
				legalStep.push_back(tmpstep);	
			else if(checkCanFlip(board,i,j,black))
				legalStep.push_back(tmpstep);
		}
	}
	return legalStep;
}

vector<int> getStep(vector<vector<int> >&board,bool black){
	vector<vector<int> >legalStep;
	vector<int>step;
	
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			vector<int> tmpstep;
			tmpstep.resize(2);
			tmpstep[0] = i;
			tmpstep[1] = j;
			if(board[i][j]!=0)
				continue;
			else if((j>0&&j<SIZE-1) && (i>0&&i<SIZE-1))
				legalStep.push_back(tmpstep);	
			else if(checkCanFlip(board,i,j,black))
				legalStep.push_back(tmpstep);
		}
	}
	
	/*cout<<"Legal step---------------------\n";
	for(int k=0;k<legalStep.size();k++)
		printf("(%d,%d)\t",legalStep[k][0],legalStep[k][1]);
	cout<<endl<<endl;*/
	if(legalStep.size()==0){//no step
		step.push_back(-1);
		step.push_back(-1);
	}
	else{
		step = legalStep[0];
		int tmpHighHeru = -100;
		for(int i=0;i<legalStep.size();i++){
			int r = legalStep[i][0];
			int c = legalStep[i][1];
			if(h[r][c]>tmpHighHeru){
				tmpHighHeru = h[r][c];
				step = legalStep[i];
			}
		}
		//step = legalStep[rand()%legalStep.size()];	
	}
	return step;
}

bool existSame(vector<vector<int> >board, bool black, vector<int> from, int diri, int dirj){
	int ri = from[0]+diri;
	int rj = from[1]+dirj;
	while(true){
		if(ri<0||ri>=SIZE || rj<0||rj>=SIZE)//out of bound
			return false;
		else if(board[ri][rj]==0 || board[ri][rj]==-1)
			return false;
		else if((board[ri][rj]==2 && !black) || (board[ri][rj]==1 && black))
			return true;
		ri += diri;
		rj += dirj;
	}
	return false;
}

void handleStep(vector<vector<int> >&board,bool black,vector<int>step){
	if(step[0]==-1 || step[1]==-1)//no step
		return;
	black? board[step[0]][step[1]]=1 : board[step[0]][step[1]]=2;
	
	//Flipping
	for(int k=0;k<8;k++){
		int ri = step[0]+pi[k];
		int rj = step[1]+pj[k];
		if(existSame(board,black,step,pi[k],pj[k])){
			while((board[ri][rj]==2 && black) || (board[ri][rj]==1 && !black)){
				black? board[ri][rj]=1 : board[ri][rj]=2;
				ri += pi[k];
				rj += pj[k];
			}
		}
	}
}

int checkBoardSim(vector<vector<int> > board){
	int black = 0, white = 0;
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			if(board[i][j]==1)
				black++;
			else if(board[i][j]==2)
				white++;
		}
	}
	//cout<<"[black]:"<<black<<"\t[white]:"<<white<<endl;
	if((black+white)==(SIZE*SIZE-4)){
		if(black>white)
			return 1;
		else if(black<white)
			return 2;
		else
			return 0;
	}
	return -1;
}

int stuckWin(vector<vector<int> > board){
	int black = 0, white = 0;
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			if(board[i][j]==1)
				black++;
			else if(board[i][j]==2)
				white++;
		}
	}
	if(black>white)
		return 1;
	else if(black<white)
		return 2;
	else
		return 0;
	return -1;
}

int simulation(Node *current){
	vector<vector<int> > board = current->state;
	bool black = !current->black;
	bool stuck = false;
	int result = -1;
	while(true){
		vector<int> step;		
		step = getStep(board,black);
		
		if(stuck){
			if(step[0]==-1){
				int r = stuckWin(board); 
				if((r==1 && current->black) || (r==2 && !current->black))
					return 1;//win
				else
					return 0;
			}
			else if(step[0]!=-1)
				stuck = false;
		}
		if(step[0]==-1)
			stuck = true;
		
		handleStep(board,black,step);
		
		black = !black;
		result = checkBoardSim(board);
		if((result==1 && current->black) || (result==2 && !current->black))
			return 1;//win
		else if(result!=-1)
			return 0;
	}
}
