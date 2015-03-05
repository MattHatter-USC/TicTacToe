#include <cmath>
#include <algorithm>
#include <iostream>
using namespace std;

//static bool stackcall[9];
static int tilestate[9];
static bool first = true; //comp is second
static int turn=0;
static bool done = false;
struct gnode {
	gnode();
	double val;
	gnode * substates = NULL;
};
gnode::gnode() {
	val = 0;
	substates = NULL;
}
static gnode mainstate;
static gnode * cstate;
static int forceval = -1;
int equal(int a,int b,int c) {
	return (((a==b) && (b==c))?a:0);
}
bool won() {
	if (0!=equal(tilestate[0],tilestate[1],tilestate[2]))
		return true;
	if (0!=equal(tilestate[3],tilestate[4],tilestate[5]))
		return true;
	if (0!=equal(tilestate[6],tilestate[7],tilestate[8]))
		return true;
	if (0!=equal(tilestate[0],tilestate[3],tilestate[6]))
		return true;
	if (0!=equal(tilestate[1],tilestate[4],tilestate[7]))
		return true;
	if (0!=equal(tilestate[2],tilestate[5],tilestate[8]))
		return true;
	if (0!=equal(tilestate[0],tilestate[4],tilestate[8]))
		return true;
	if (0!=equal(tilestate[2],tilestate[4],tilestate[6]))
		return true;
	return false;
}
int almostwon(int recur) {;
	int a = 0;
	int wins = 0;
	int wini = -1;
	for (int i=0;i<9;i++){
		if (tilestate[i] == 0) {
			tilestate[i] = 1 + ((recur+1) % 2);
			if (won()) {
				wins++;
				wini = a;
			}
			tilestate[i]=0;
			a++;
		}
	}
	if (wins == 1)
		return wini;
	else
		return -1;
}
void printstate() {
	for (int y = 0;y<3;y++) {
		for (int x = 0;x<3;x++) {
			int state = tilestate[y*3+x];
			if (state == 0)
				cout << "*";
			else if (state == 1)
				cout << "O";
			else if (state == 2)
				cout << "X";
		}
		cout << endl;
	}
	turn++;
	if (won()) {
		if (((turn%2==1) && (first)) || ((turn%2==0)&&(!first)))
			cout << "You lost!" << endl;
		else 
			cout << "You win!" << endl;
		done = true;
	}
	if (turn == 9) {
		cout << "You tied!" << endl;
		done = true;
	}
	cout << endl;
}
double recurf(int recur,gnode * parentnode) { //positive good for P1, negative good for P2
	if (won()) {
		parentnode->val = recur%2*(-2)+1;
		return recur%2*(-2)+1;
	}
	if (recur > 0) {
		int a = 0;
		int fin = 0;
		for (int i=0;i<9;i++) {
			if (tilestate[i] == 0) {
				tilestate[i] = 1 + (recur % 2); //set tilestate for calculations
				parentnode->substates[a].substates = new gnode[recur-1];
				double retv = recurf(recur-1,&(parentnode->substates[a])); //add result of recursive call to max val
				if (retv == ((recur%2)*2)-1) { //if a solid win node for this side
					fin = retv;
				}
				parentnode->val+=retv;
				tilestate[i] = 0; //reset tilestate
				a++;
			}
		}
		int w = almostwon(recur);
		if (w != -1) { //make immediate grabs less appealing
			if ((parentnode->val/recur == 1) && (parentnode->substates[w].val != 1)) {
				printstate();
				cout << w << " " << parentnode->substates[w].val << endl << endl;
			}
			parentnode->val = parentnode->substates[w].val;
		}
		else
			parentnode->val = parentnode->val/recur;
		if (fin != 0) {
			parentnode->val = fin;
		}
		return parentnode->val;
	}
	else {
		return 0; //ties worth 0
	}
}

int getmove(gnode * cstate) {
	int a = 0;
	if (!(cstate->substates)) {
		cout << "Wat" << endl;
		return -1;
	}
	int maxindex = -1;
	double maxindexv = -2;
	for (int i = 0;i<9;i++) {
		if (tilestate[i] == 0) { //if empty
				//cout << i + 1 << " " << cstate->substates[a].val << endl;
				if (maxindex == -1) { //not initialized max
					maxindex = i;
					maxindexv = cstate->substates[a].val;
				}
				else if (cstate->substates[a].val > maxindexv) { //newmax
					maxindexv = cstate->substates[a].val;
					maxindex = i;
				}
			//}
			a++;
		}
	}
	if (forceval != -1) {
		int q = forceval;
		forceval = -1;
		return q;
	}
	if (maxindex != -1) {
		return maxindex;
	}
	else {
		cout << ":(" << endl;
		return -1;
	}
}
void printshit() {
	int a = 0;
	for (int i=0;i<9;i++) {
		if (tilestate[i] == 0) {
			cout << i+1 << " " << cstate->substates[a].val << endl;
			a++;
		}
	}
}
int addrmove(int addr) {
	int a=0;
	for (int i=0;i<addr;i++)
		if (tilestate[i] == 0)
			a++;
	return a;
}
void process(int thing) {
	if (thing > 9) {
		forceval = thing - 11;
	}
}
int main() {
	for (int i=0;i<9;i++)
		tilestate[i] = 0;
	//gnode mainstate;
	mainstate.substates = new gnode[9];
	recurf(9,&mainstate);
	for (int i=0;i<9;i++) //reset tilestates
		tilestate[i] = 0;
	int move;
	cstate = &mainstate;
	for (int i=0;i<6;i++) {
		if (first) {
			move = getmove(cstate);
			if (tilestate[move]==0)
				tilestate[move] = 2;
			else
				cout << "Rar" << endl;
			printstate();
			if (done) {
				return 0;
			}
			else
				cstate = &(cstate->substates[addrmove(move)]); //move down line
			//printshit();
			if (i!=5) {
				cin >> move;
				process(move);
				move--;
				while ((tilestate[move]!=0) || (move > 9)) {
					cout << "Another selection please" << endl;
					cin >> move;
					move--;
				}
				tilestate[move] = 1;
				printstate();
				if (done) {
					return 0;
				}
				else
					cstate = &(cstate->substates[addrmove(move)]); //move down line
			}
		}
		else {
			cin >> move;
			move--;
			while (tilestate[move]!=0) {
				cout << "Another selection please" << endl;
				cin >> move;
				move--;
			}
			tilestate[move] = 2;
			printstate();
			if (done) {
				return 0;
			}
			else
				cstate = &(cstate->substates[addrmove(move)]); //move down line
			if (i!=5) {
				move = getmove(cstate);
				if (tilestate[move]==0)
					tilestate[move] = 1;
				else
					cout << "Rar" << endl;
				printstate();
				if (done) {
					return 0;
				}
				else
					cstate = &(cstate->substates[addrmove(move)]); //move down line
			}
		}
	}
}
