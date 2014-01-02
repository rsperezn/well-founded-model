#ifndef _RULES_
#define _RULES_
#endif
#include <string>
using namespace std;


class Rules{
public:
	Rules();
	Rules(string line);
	string getBody();
	string getHead();
	string body;
	string head;
	bool hasNegAtm(string atom);
};