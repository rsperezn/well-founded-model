#include "Rules.h"
#include <algorithm>
#include <cctype>
Rules::Rules(){
	//default constructor
}
Rules::Rules(string line){
	int pos=line.find('<');
	if (pos>0){//if its a rule with a body
		head=line.substr(0,pos);//just in case but still it should be the first character
		body=line.substr(pos+2, line.length());//same reasoning as before
		body.erase(remove(body.begin(),body.end(),','),body.end());
	}
	//
	else{
		head=line;
	}
}
string Rules::getBody(){
	return body;
}
string Rules::getHead(){
	return head;
}
bool Rules::hasNegAtm(string atom){
	bool result=false;
	if((atom.compare("")==0) && (body.find("~")!=string::npos) ){//just check if it has A negative in the body 
		return true;
	}
	else if(atom.compare("")!=0){//look into the body and find the ~atom
		string target="~"+atom;
		result=(string::npos == body.find(target) ? false :true);//if it doesnt have ~atom return false, true otherwise
	}//end else
	return result;
}