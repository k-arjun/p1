//generate the control structure
#include "standardizer.h"
#include <vector>
#include <sstream>

string intToStr(int i)
{
	stringstream ss;
	ss << i ;
	string s = ss.str();
	return s;
}

class nodeItem
{
	public:
	string type; //type can be conditional, lambda or name. Look at value and decide if it is required.
	string k; // index of new CS to be created below this lambda.
	string x; // x is the left child of lambda. can be a comma separated list
	string value;  // can we make the use of type redundant here. value will be -> or lambda for special cases
	
	//---------
	public:
	
};

class cStr
{    
	private:
	vector<nodeItem> nlist;
	
	public:
	void push(nodeItem item);
};

void cStr::push(nodeItem item)
{
}

class csGen
{	
	private:
	map< string, cStr> cStrMap;
	stack<int> csCountStk;
	//Private Functions
	void buildCS(treeNode *stNode);
	
	//Public Functions
	public:
	void run(const string &filename); //will pass the root node of the standard tree to a CSGen object.
}

void csGen::run(const string &filename)
{
	stdTree s;
	treeNode *stdRoot;
	s.run(filename);
	stdRoot = s.getRoot();
	
	int count = 0; // append this count to the CS name
	csCountStk.push(count);
	
	stack<treeNode*> csStack;
	csStack.push(stdRoot);
	
	while( !csStack.empty() )
	{
		treeNode *temp = csStack.top();
		csStack.pop();
		
		cStr CS;
		string csName("CS");
		csName.append(intToStr( csCountStk.top() ) );
		csCountStk.pop();
		count++;
		// we have csName as CS0, CS1, CS2... at this point.
		// Now initialize cStr CS
		// check what the data in 'temp' holds. create CS based on those rules
		buildCS(temp, CS, csStack, count);
		cStrMap[csName] = CS;
	}
}

void csGen::buildCS(treeNode *stNode, cStr &CS, stack<treeNode*> &csStack, int &count)
{
	if( stNode == 0 )
		return;
	
	nodeItem nItem;
	string data = stNode->data;
	
	bool lambdaEncountered = false;
	
	if( data.compare("tau")==0 )
	{
	}
	else if ( data.compare("lambda")==0 )
	{
		nodeItem item;
		item.type = "lambda";
		item.value = "lambda";
		lambdaEncountered = true;
		if( stNode->child->data.compare(",")==0 )
		{
			treeNode *temp = stNode->child->child;
			string tString("");
			while(temp)
			{
				tString.append( temp->data );
				temp = temp->sibling;
				
				if(temp)
					tString.append(",");
			}
			item.x = tString;
			item.k = intToStr( count++ );
		}
		else
		{
			item.x = stNode->child->data;
			item.k = intToStr( count++ );
		}
		CS.push(item);
		csStack.push( stNode->child->sibling );
		csCountStk.push(count);
	}
	else
	{
		nodeItem item;
		item.type = "name";
		item.k = "-1";
		item.x = "-1";
		item.value = data;
		CS.push(item);
	}
	
	if(!lambdaEncountered)
		buildCS(stNode->child, CS, csStack, count);
	
	buildCS(stNode->sibling, CS, csStack, count);
}
