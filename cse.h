// CSE Machine
// Takes a map of control structures from control.h

#include "control.h"
class Control
{
};

class cseStack
{
};

class env
{
};

class CSE
{
	private:
	map<string, cStr> cStrMap;
	
	public:
	void run(const string &filename);
};

void CSE::run(const string &filename)
{
	csGen c;
	c.run(filename);
	c.getMap(cStrMap); //function initializes the cStrMap object in CSE class.
}
