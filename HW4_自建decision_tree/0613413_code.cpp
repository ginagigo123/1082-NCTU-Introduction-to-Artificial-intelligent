#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream> 
#include <fstream> 
#include <time.h> // time
#include <math.h>
#include <algorithm> // std::random_shuffle
#include <vector>
#define N 178
#define type 13

using namespace std;

float data[N][type+1];
float train[N][type+1];
float valid[N][type+1];

int TN,VN;

string printAttr[] = {"class",
 "Alcohol",
 "Malic acid",
 "Ash",
 "Alcalinity of ash",
 "Magnesium",
 "Total phenols",
 "Flavanoids",
 "Nonflavanoid phenols",
 "Proanthocyanins",
 "Color intensity",
 "Hue",
"OD280/OD315 of diluted wines",
"Proline"};

void print_data(float input[][type+1], int n){
	for (int j = 0 ; j < n ; j ++) {
		for ( int i = 0 ; i < type+1 ; i ++)
			cout << input[j][i] <<",";
		cout << endl;	
	}
}
vector<string> _csv(string);
void load_data(string);

// split data to training / validation
void split_data(float p){
	int n = N * p;
	TN = n;
	VN = N - TN;
	//cout << "train N:" << FN << endl;
	//cout << "Valid N:" << VN << endl;
	random_shuffle(data,data+N);
	
	for (int i = 0 ; i < TN ; i ++)
		for (int j = 0 ; j < type+1 ; j ++) 			
			train[i][j] = data[i][j];
	
	int Vindex = 0;
	for (int i = TN ; i < N ; i ++) {
		for ( int j = 0 ; j < type+1 ; j ++)
			valid[Vindex][j] = data[i][j];
		Vindex++;
	}
} 

float gini_index(float input[][type+1],int n){
	int count[3]={0}; // 分別計算class 1,2,3 
	float gini = 0;
	for( int i = 0 ; i < n ; i ++) {
		count[ int(input[i][0]-1)] ++;
	}
	for (int i = 0 ; i < 3 ; i ++) {
		gini += pow(count[i]*1.0/n,2);
	}
	gini = 1.0 - gini;
	return gini;
}

struct split_info{
	int attribute;
	float threshold;
	float gini;
	split_info(void){
		gini = 1.0;
	}
};

class CART{
	private:
		float data[N][type+1];
		int length;
		int num_attr; // num of random attribute
	public:
		CART *Left;
		CART *Right;
		bool is_terminal;
		int end_class;
		struct split_info result; // save best attr, threshold, gini
		
		CART(float[][type+1],int l); // constructor
		int get_length();
		void set_num_attr(int); // set num_attr
		float test_split(int,float);
		struct split_info select_threshold(int);  // find best value (lowest gini) according to threshold
		void find_best_attr(); // select the threshold with the lowest total impurity
		void get_split();
		bool same_class();
		int to_terminal();
};

CART::CART(float input[][type+1],int l) {
	for (int i = 0 ; i < l ; i ++) {
		for(int j = 0 ; j < type+1 ; j ++)
			data[i][j] = input[i][j];
	}
	length = l;
	is_terminal = false;
} 

int CART::get_length() {
	return length;
}

float CART::test_split(int attr,float threshold){
	float left[N][type+1];
	float right[N][type+1];
	int Lindex = 0;
	int Rindex = 0;
	for (int i = 0 ; i < length ; i ++) {
		if (data[i][attr] < threshold) {
			for ( int j = 0 ; j < type +1 ; j ++)
				left[Lindex][j] = data[i][j];
			Lindex ++;
		}
		else {
			for (int j = 0 ; j < type+1 ; j ++)
				right[Rindex][j] = data[i][j];
			Rindex ++;
		}
	}
	float gini = Lindex*1.0/length*gini_index(left,Lindex) + Rindex*1.0/length*gini_index(right,Rindex);
	return gini;
}

void CART::set_num_attr(int n) {
	num_attr = n;
}

// if threshold = 1 代表 Alcohol 
struct split_info CART::select_threshold(int attr){
	// th 存放這 attribute 所有值 
	struct split_info best; 
	best.gini = 1.0;
	vector<float>th;
	for (int i = 0 ; i < length ; i ++)
		th.push_back(data[i][attr]);
	sort(th.begin(),th.begin()+length);
		
	for (int i = 0 ; i < length-1 ; i ++) {
		float threshold = (th[i] + th[i+1]) / 2.0;
		float resultgini = test_split(attr,threshold);
		if(resultgini < best.gini){
			best.threshold = threshold;
			best.gini = resultgini;
		}
	}
	best.attribute = attr;
	return best;
}

//select attribute from attr. bag
void CART::find_best_attr(){
	int select [type+1];
	for (int i = 0 ; i < type; i ++) 
		select[i] = i+1;
	random_shuffle(select,select+type);
	
	for (int i = 0 ; i < num_attr ; i ++) {
		struct split_info tmp;
		//select threshold of the attribute
		tmp = select_threshold(select[i]);
		//cout<<"attribute:"<<i<<endl;
		if (tmp.gini < result.gini) {
			result = tmp;
		}
	}
}

// check whether data in this class are the same class;
bool CART::same_class() {
	int first = data[0][0];
	for (int i = 0 ; i < length ; i ++) {
		if (data[i][0] != first)
			return false;
	}
	is_terminal = true;
	end_class = first;
	return true;
}

// find the best attribute -> produce left & rihgt CART
void CART::get_split(){
	set_num_attr(6);
	find_best_attr();
	float left[N][type+1];
	float right[N][type+1];
	int Lindex = 0;
	int Rindex = 0;
	for (int i = 0 ; i < length ; i ++) {
		if (data[i][result.attribute] < result.threshold) {
			for ( int j = 0 ; j < type +1 ; j ++)
				left[Lindex][j] = data[i][j];
			Lindex ++;
		}
		else {
			for (int j = 0 ; j < type+1 ; j ++)
				right[Rindex][j] = data[i][j];
			Rindex ++;
		}
	}	
	Left = new CART(left,Lindex);
	Right = new CART(right,Rindex);
}


int CART::to_terminal() {
	int count[3] = {0}; // calculate type
	for (int i = 0 ; i < length ; i ++) {
		count[ int(data[i][0]-1) ] ++;
	}
	int maxx = count[0];
	int ctype = 1;
	for (int i = 1 ; i < 3 ; i ++) {
		if(count[i] > maxx) {
			ctype = i+1;
			maxx = count[i];
		}
	}
	is_terminal = true;
	end_class = ctype;
	return ctype;
} 

void split(CART *r, int max_depth, int min_size , int depth) {
	//cout<<" Root split "<<endl;
	r->get_split();
	// check for a no split
	if(r->result.gini == 0) {
		r->to_terminal();
		return;
	}
	// check for max depth
	if (depth >= max_depth) {
		r->Left->to_terminal();
		r->Right->to_terminal();
		return;
	}
	// process left child
	if (r->Left->get_length() <= min_size)
		r->Left->to_terminal();
	else {
		r->Left->get_split();
		split(r->Left, max_depth, min_size, depth+1);
	}
	// process Right child
	if (r->Right->get_length() <= min_size)
		r->Right->to_terminal();
	else {
		r->Right->get_split();
		split(r->Right, max_depth, min_size, depth+1);
	}
}

void print_tree(CART *root,int depth){
	if ( root->is_terminal == false) {
		for(int i = 0 ; i < depth ; i ++)
			cout<<"  ";
		cout<<"gini index:"<< root->result.gini;
		cout <<" ["<< printAttr[root->result.attribute]<<"] < " << root->result.threshold << " length:" << root->get_length() << endl;
		cout << "Left" << endl;
		print_tree(root->Left,depth+1);
		cout << "Right" << endl;
		print_tree(root->Right,depth+1);
	}
	else{
		for(int i = 0 ; i < depth ; i ++)
			cout<<"  ";
		cout << "depth " << depth <<". ";
		cout << "terminal class:" <<  root->end_class << endl;
	}
}

CART build_tree(float TRAIN[][type+1],int l, int max_depth, int min_size) {
	CART root(TRAIN,l);
	split(&root,max_depth,min_size,0);
	return root;
}

int predict(CART *root, float input[type+1]) {
	CART *cursor = root;
	while ( !cursor->is_terminal ) {
		// < go to left 
		if( input[cursor->result.attribute] < cursor->result.threshold)
			cursor = cursor->Left;
		else
			cursor = cursor->Right;
	}
	return cursor->end_class;
}

void print_result(vector<CART> RF,float input[][type+1],int n) {
	int success = 0;
	for(int i = 0 ; i < n ; i ++) {
		int classCount[3] = {0};
		for (int j = 0 ; j < RF.size() ; j ++) {
			CART current = RF[j];
			classCount[ predict(&current,input[i]) - 1 ] ++;
		}
		// select most count to be the answer
		int maxx = 0;
		int finalclass;
		for(int a = 0 ; a < 3 ; a ++) {
			if(classCount[a] > maxx ) {
				maxx = classCount[a];
				finalclass = a+1; // 1 ~ 3
			}
		}
		if(input[i][0] == finalclass)
			success++;
	}
	cout << "success rate:" << success*1.0/n <<endl;
}

int main()
{
	// loading data 
	load_data("wine.data");
	// divide dataset into training & validation
	split_data(0.7);
	
	// build K tree
	int tree_num = 200;
	vector<CART> random_forest;
	for(int i = 0 ; i < tree_num ; i ++) {
		// split train to 0.8
		random_shuffle(train,train+TN);
		int IN = TN * 0.8;
		float in[N][type+1];
		for (int a = 0 ; a < IN ; a ++ )
			for (int b = 0 ; b < type+1 ; b ++)
				in[a][b] = train[a][b];
		// build each tree
		CART tmp = build_tree(in,IN,5,5);
		random_forest.push_back(tmp);
	} 
	// Tree bagging
	int TB = 0.8*random_forest.size();
	vector<CART> selected(random_forest.begin(),random_forest.begin()+TB);
	vector<CART> OOB(random_forest.begin()+TB,random_forest.end());
	
	// testing by using OOB err. or validation data set
	print_result(selected,train,TN);
	print_result(selected,valid,VN);
	cout<<"------------------OOB-----------------"<<endl;
	print_result(OOB,train,TN);
	print_result(OOB,valid,VN);
	
	//print_tree(&root,0);
	system("pause");
	return 0;
}

vector<string> _csv(string s){
	vector<string> arr;
	istringstream delim(s);
	string token;
	int c = 0;
	while (getline(delim,token,',')) {
		arr.push_back(token);
		c ++;
	}
	return arr;
}

void load_data(string f) {
	ifstream inFile("wine.data");
	if (!inFile){
		cout << "檔案無法開啟\n";
		exit(1);
	} 
	string line;
	int index = 0;
	while (getline(inFile,line)) {
		vector<string> a = _csv(line);
		for ( int i = 0 ; i < a.size() ; i ++)
			data[index][i] = atof(a[i].c_str());
		index ++;
	}
}


