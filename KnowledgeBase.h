#ifndef _KNOWLEDEGEBASE_
#define _KNOWLEDEGEBASE_
#endif
#include "Rules.h"
#include <ctype.h>
#include <sstream>
#include <vector>
#include <set>
class KnowledgeBase{
public:
	KnowledgeBase();
	void initUnknown();//all atoms should be in the Unknown set
	void BottomUpProc(string opt);
	void setfacts();//sets the content of the vector with all the facts from therules vector
	void setbodies();//sets the content of the vector with all the bodies only from therules vector
	bool hasAllfacts(string body,set<string> &theset)const;//tells is the body passed in has all its members in facts vector OR the Tposs vector
	void setFnew();//those atoms that are in Unknowns set and dont appear in Tposs
	bool hasNegAtm(string body)const;//tells if this body has a any ~ (NEGated AToM)
	void compileTnew();
	void compileFnew();
	void deleteRule(string atom,string workSet);//we can either make the working set Tnew or Fnew
	void removeOcurrence(string atom,int workSet);//we can either make the working set Tnew=0 and Fnew=1
	void setTempKB();//creates the KB version that ignores the negated atoms and uses it to run the ButtomUpProc
	vector<Rules> therules;//vector with all the rules that form the KB
	set<string> facts;
	vector<string> bodies;
	set<string> Tnew;
	set<string> Tposs;
	set<string> Fnew;
	set<string> Unknown;
	set<string>::iterator iter;//just to iterate the Tnew set
	vector<Rules> tempKB;
};
