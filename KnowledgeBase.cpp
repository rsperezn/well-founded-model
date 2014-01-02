#include "KnowledgeBase.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>

KnowledgeBase::KnowledgeBase(){
	//default constructor
}
void KnowledgeBase::initUnknown(){
	for(int i=0;i<therules.size();i++){
		Rules currRule=therules.at(i);
		if(Unknown.count(currRule.getHead())==0){//if the elements is not already there
			Unknown.insert(currRule.getHead());
		}
		string currBody=currRule.getBody();
		currBody.erase(remove(currBody.begin(),currBody.end(),'~'),currBody.end());//strip off all the ~ since we just care about the atoms
		currBody.erase(remove(currBody.begin(),currBody.end(),','),currBody.end());//strip off all the , since we just care about the atoms
		for(int i=0;i<currBody.length();i++){
			if(Unknown.count((currBody.substr(i,1)))==0){//if the element is not already there
				Unknown.insert(currBody.substr(i,1));
			}
		}
	}
}
void KnowledgeBase::setfacts(){
	for(int i =0; i<therules.size();i++){
		if(therules.at(i).getBody().compare("")==0 && facts.count(therules.at(i).getHead())==0 ){//if it doesnt have a body then it is just a fact or if its not already there
			facts.insert(therules.at(i).head);		
		}	
	}
}
void KnowledgeBase::setbodies(){
	for (int i =0; i<therules.size();i++){
		if(therules.at(i).getBody().compare("")!=0){
			bodies.push_back(therules.at(i).getBody());	
		}
	}
}
bool KnowledgeBase::hasNegAtm(string body)const{
	if(body.find("~")!=string::npos){
		return true;
	}
	return false;
}

bool KnowledgeBase::hasAllfacts(string body,set<string> &theset)const{
	for(int i=0;i<body.length();i++){
		if(isalpha(body[i])){
			string atom=body.substr(i,1);//just get the atom itself
			if(theset.count(atom)==0){//if its not there
				return false;
			}
		}
	}//end for
	return true;
}

void KnowledgeBase::BottomUpProc(string opt){
	/*we run bottom up procedure on those rules 
	opt= noneg when we must run the procedure on the rules
	that doesnt contain a negated atom in the body
	opt=ignore to run the procedure but ignoring the negated attoms*/
	//run loop until no more conclusion
	
	bool mustcheck=true;
	if(opt.compare("noneg")==0){
		cout<<"Step 1:"<<endl;
		cout<<"Bottom Up Procedure on the rules that dont contain a negated atom in the body"<<endl;
		setfacts();//set up the facts
		int newfact=false;//use as a flag for the while loop 
		while(mustcheck){
			for(int i=0;i<therules.size();i++){
				Rules currRule=therules.at(i);//current rule we are working with
				if(!currRule.hasNegAtm("")){//if it has not negated atoms
					if((hasAllfacts(currRule.getBody(),facts))  &&  (facts.count(currRule.getHead())==0)){//if all the elemts in the body are fact and the head of the rule is not in the facts set already
						facts.insert(currRule.getHead());//maybe return this later on
						newfact=true;
					}
				}//end if !hasNegAtm
			}//end for loop
			if(!newfact){//if there was something new added as a fact must continue, else we reached the fixed point
				mustcheck=false;//if there was nothing new added to the facts vector we stop the loop
			}
			newfact=false;//update so it must find something new to continue the while loop
		}//end while loop
		//add this new set of atoms to Tnew 
		Tnew=facts;
		cout<<"The set Tnew has been derived and contains:"<<endl;
		for(iter=Tnew.begin(); iter!=Tnew.end();++iter) {
			cout<<*iter+" ";
			Unknown.erase(*iter);//remove the True atoms form the unkwnown set
		}
		cout<<endl;
	}//end if noneg
	else{//do the same but we ignore the negated atoms
		cout<<"Step 2:"<<endl;
		cout<<"Bottom Up Procedure on the rule set but ignoring negated atoms"<<endl;
		setTempKB();
		int newfact=false;//use as a flag for the while loop 
		while(mustcheck){
			for(int i=0;i<tempKB.size();i++){
				Rules currRule=tempKB.at(i);//current rule we are working with
				if((hasAllfacts(currRule.getBody(),Tposs))  &&  (Tposs.count(currRule.getHead())==0)){//if all the elemts in the body are fact and the head of the rule is not in the facts set already
					Tposs.insert(currRule.getHead());
					newfact=true;		//update KB vector by deleteg rule at possitoins i
				}
			}//end for loop
			if(!newfact){//if there was something new added as a fact must continue, else we reached the fixed point
				mustcheck=false;//if there was nothing new added to the facts vector we stop the loop
			}
			newfact=false;//update so it must find something new to continue the while loop
		}//end while loop
		cout<<"The set Tposs has been derived and contains:"<<endl;
		for(iter=Tposs.begin(); iter!=Tposs.end();++iter) {
			cout<<*iter+" ";
		}
		cout<<endl;
		//calulate Fnew
		//caculate the set difference of Unknow and Tposs
		set_difference(Unknown.begin(),Unknown.end(),Tposs.begin(),Tposs.end(),inserter(Fnew,Fnew.end()));
		for(iter=Fnew.begin();iter!=Fnew.end();++iter){//remove the False atoms from the Unkwnown set
			Unknown.erase(*iter);
		}
		//display the items
		cout<<"The set Fnew has been derived and contains:"<<endl;
		for(iter=Fnew.begin();iter!=Fnew.end();++iter){//remove the False atoms from the Unkwnown set
			cout<<*iter+ " ";
		}
		cout<<endl;
	}//end else
 }
void KnowledgeBase::compileTnew(){
	//for items in Tnew
	for(iter=Tnew.begin(); iter!=Tnew.end();++iter) {
			deleteRule(*iter,"Tnew");
			removeOcurrence(*iter,0);//Tnew=0
	}
}

void KnowledgeBase::compileFnew(){
	for(iter=Fnew.begin();iter!=Fnew.end();++iter){
		deleteRule(*iter,"Fnew");
		removeOcurrence("~"+*iter,1);//Fnew=1
	}
}
void KnowledgeBase::deleteRule(string atom,string workSet){
	if(workSet.compare("Tnew")==0){
		for(int i=0;i<therules.size();i++){
			Rules currRule=therules.at(i);
				if((currRule.getHead().compare(atom)==0) || (currRule.hasNegAtm(atom))){
					therules.erase(therules.begin()+i);	//delete any rule with the atom in the head or ~a in the body
					i--;//to compensate the loss of one rule 
				}	
		}//end for 
	}//end if
	// the next code will be executed on Fnew set
	else{
		for(int i=0;i<therules.size();i++){
			Rules currRule=therules.at(i);
			if((currRule.getHead().compare(atom)==0) || ( (currRule.getBody().find(atom)!=string::npos) && (currRule.getBody().find("~"+atom)==string::npos) )){//need to make sure that we find the atom itself withouht any negation
				therules.erase(therules.begin()+i);	//delete any rule with the atom in the head or ~a in the body
				i--;//to compensate the loss of one rule 
			}	
		}
	}//end else 
}
void KnowledgeBase::removeOcurrence(string atom,int workSet){
	for(int i =0;i<therules.size();i++){
		string currBody=therules.at(i).getBody();
		string newBody;
		int pos=currBody.find(atom);
		while(pos!=string::npos){//must remove all occurrences
			newBody=currBody.erase(pos,1+workSet);//Tnew=0 and Fnew=1
			therules.at(i).body=newBody;//remove any occurrence of atom in the reamining rules
			pos=therules.at(i).getBody().find(atom);
		}//end while
	}
}


void KnowledgeBase::setTempKB(){
	tempKB=therules;
	for (int i=0;i<tempKB.size();i++){
		Rules currRule=tempKB.at(i);
		string currBody=currRule.getBody();
		currBody.erase(remove(currBody.begin(),currBody.end(),','),currBody.end());//remove to make it simpler removing the negated atom
		int pos= currBody.find("~");
		while(pos!=string::npos){
			string newbody=currBody.erase(pos,2);
			tempKB.at(i).body=newbody;//update the body without the negated atom
			pos=tempKB.at(i).getBody().find("~");
		}
	}
}
