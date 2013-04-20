//generate the control structure
#include "standardizer.h"
#include <vector>
#include <map>
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
	string type; //type can be lambda, tau or name. is conditional required?
	string k; // index of new CS to be created below this lambda.
	string x; // x is the left child of lambda. can be a comma separated list
	string value;  // can we make the use of type redundant here. value will be -> or lambda for special cases
	
	//---------
	public:
	void display();
};

void nodeItem::display()
{
	if( type.compare("tau")==0 )
	{
		cout << "[" << value << " " << k << "]";
	}
	else if( type.compare("lambda")==0 )
	{
		cout << "[" << value << " " << k << " " << x << "]";
	}
	else if( type.compare("name")==0 )
	{
		cout << value ;
	}
	else
	{
		cerr << " nodeItem::display unreachable code\n";
	}
}

class cStr
{    
	private:
	vector<nodeItem> nlist;
	
	public:
	void display();
	void push(nodeItem item);
	nodeItem getItem(int i)
	{
		return nlist[i];
	}
	int getCount()
	{
		return nlist.size();
	}
};

void cStr::push(nodeItem item)
{
	nlist.push_back(item);
}

void cStr::display()
{
	vector<nodeItem>::iterator it;
	it = nlist.begin();
	
	vector<nodeItem>::iterator it2;
	it2 = nlist.end();
	
	for(; it!=it2; ++it)
	{
		(*it).display();
		cout << " ";
	}
	cout << endl;
}

class csGen
{	
	private:
	map< string, cStr> cStrMap;
	stack<int> csCountStk;
	//Private Functions
	void buildCS(treeNode *stNode, cStr &CS, stack<treeNode*> &csStack, int &count);
	void displayMap();
	
	//Public Functions
	public:
	void run(const string &filename); //will pass the root node of the standard tree to a CSGen object.
	void getMap(map< string, cStr > &newMap );
};

void csGen::getMap(map< string, cStr > &newMap )
{
	newMap = cStrMap;
}

void csGen::displayMap()
{
	map<string, cStr>::iterator it;
	map<string, cStr>::iterator it2;
	
	it = cStrMap.begin();
	it2 = cStrMap.end();
	
	for(; it != it2; ++it)
	{
		cout << it->first << ": ";
		(it->second).display();
	}
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
		//count++;
		// we have csName as CS0, CS1, CS2... at this point.
		// Now initialize cStr CS
		// check what the data in 'temp' holds. create CS based on those rules
		buildCS(temp, CS, csStack, count);
		cStrMap[csName] = CS;
	}
	
	cout << endl;
	displayMap();
}

void csGen::buildCS(treeNode *stNode, cStr &CS, stack<treeNode*> &csStack, int &count)
{
	if( stNode == 0 )
		return;
	
	//nodeItem nItem;
	string data = stNode->data;
	
	bool lambdaEncountered = false;
	
	if( data.compare("tau")==0 )
	{
		treeNode *temp;
		temp = stNode->child;
		if( temp == 0 )
		{
			cerr << "Tau had zero childs! Error in standard tree\n";
		}
		int tauCount = 0;
		while(temp)
		{
			tauCount++;
			temp = temp->sibling;
		}
		
		nodeItem item;
		item.type = "tau";
		item.value = "tau";
		item.x = "-1";
		item.k = intToStr(tauCount);
		CS.push(item);
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
			item.k = intToStr( ++count );
		}
		else
		{
			item.x = stNode->child->data;
			item.k = intToStr( ++count );
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
