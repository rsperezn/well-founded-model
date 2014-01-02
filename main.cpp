#include <iostream>
#include "KnowledgeBase.h"
#include <fstream>
#include <string>
#include <vector>
#include <string>
using namespace std;
//variables
set<string> T;
set<string> F;
set<string>::iterator iter;//just to iterate the T set and F set
KnowledgeBase KB;
//function
void WellFoundedModel();

int main (int argc, char * argv[]){
	if (argc>1){
		string line;
		ifstream infile;
		infile.open(argv[1]);
		while(!infile.eof()){
			getline(infile,line);
			//build a rule object right away and put it in a Vector
			Rules myrule(line);
			KB.therules.push_back(line);
		}
	}

	else{
		cout<<"you haven't input a file. Using default file: testcase1.txt";
	}
	//run the Well Founded Model procedure
	WellFoundedModel();
	system("pause");
	return 0;
}
void WellFoundedModel(){
	bool changeT=true;
	bool changeF=true;
	//step 0
	cout<<"Program starting...."<<endl;
	KB.initUnknown();//set all the atoms to unknown
	cout<<"The Unknown set contains:"<<endl;
	for(iter=KB.Unknown.begin(); iter!=KB.Unknown.end();++iter) {
		cout<<*iter+" ";
	}
	cout<<endl;
	int iteration=1;
	//we must loop while changes in T or T or there is nothing Unknown
	while(changeT && changeF){
		cout<<"****Iteration:"<<iteration<<"****"<<endl;
		//step 1
		KB.BottomUpProc("noneg");//run the BU procedure dicarding those rules with a negation in their body
		int tempTsize=T.size();
		T=KB.Tnew;
		//check if there is no change in the set
		if(tempTsize==T.size()){//if there was nothing new added to the T set
			changeT=false;
		}
		cout<<"Atoms added to T"<<endl;
		cout<<"The set T contains:"<<endl;
		for(iter=T.begin(); iter!=T.end();++iter) {
			cout<<*iter+" ";
		}
		cout<<endl;
		KB.compileTnew();
		//step 2
		KB.BottomUpProc("ignore");//run the BU procedute ignoring all negated atoms
		int tempFsize=F.size();
		F=KB.Fnew;
		//check if there is no change in the set
		if(tempFsize==F.size()){//if there was nothing new added to the F set
			changeT=false;
		}
		cout<<"Atoms added to F"<<endl;
		cout<<"The set F contains:"<<endl;
		for(iter=F.begin(); iter!=F.end();++iter) {
				cout<<*iter+" ";
			}
		cout<<endl;
		KB.compileFnew();
		iteration++;
	}//end while loop
	cout<<"***Finished the Well Founded Model***"<<endl;
	cout<<"The Unknown set contains:"<<endl;
	for(iter=KB.Unknown.begin(); iter!=KB.Unknown.end();++iter) {
			cout<<*iter+" ";
		}
	cout<<endl;
	cout<<"The set T contains:"<<endl;
	for(iter=T.begin(); iter!=T.end();++iter) {
			cout<<*iter+" ";
		}
	cout<<endl;
	cout<<"The F set contains:"<<endl;
	for(iter=F.begin(); iter!=F.end();++iter) {
			cout<<*iter+" ";
		}
	cout<<endl;
}




