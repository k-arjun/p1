#include "parser.h"
//#define DEBUG

bool isOp(const string &s)
{
    if( s.compare("aug") == 0 || 
    s.compare("or") == 0 || 
    s.compare("&") == 0 || 
    s.compare("+") == 0 ||
    s.compare("-") == 0 ||
    s.compare("/") == 0 || 
    s.compare("**") == 0 || 
    s.compare("*") == 0 || 
    s.compare("gr") == 0 || 
    s.compare("ge") == 0 || 
    s.compare("ls") == 0 || 
    s.compare("le") == 0 || 
    s.compare("eq") == 0 || 
    s.compare("ne") == 0 
    )
    return true;
    else return false;
}

bool isId(string s)
{
    if(isKeyword(s))
        return false;
        
    if(isLetter(s[0]))
    {
        for(int i=1; i<s.size(); ++i)
        {
            if(isLetter(s[i]) || isDigit(s[i]) || s[i] == '_' )
                continue;
            return false;
        }
        return true;
    }
    else
        return false;
}

bool isInt(string s)
{
    if(isKeyword(s))
        return false;
        
    if(isDigit(s[0]))
    {
        for(int i=1; i<s.size(); ++i)
        {
            if(isDigit(s[i]))
                continue;
            return false;
        }
        return true;
    }
    else
        return false;
}

bool isString(string s)
{
    if(isKeyword(s))
        return false;
        
    if(s[0] == '\'')
    {
        if(s.size() > 1 && s[s.size()-1] == '\'')
            return true;
        
        return false;
    }
    else
        return false;
}

class stdTree
{
    //Public Functions
    public:
    void run(const string &filename);
    treeNode* getRoot();
    
    //Private Functions
    private:
    void standardize(treeNode *node);
    void preOrder(treeNode *node, int dots);
    
    //Private Data Members
    private:
    parser p;
    treeNode *stdRoot;
};

treeNode* stdTree::getRoot()
{
	return stdRoot;
}

void stdTree::preOrder(treeNode *node, int dots) //display the standard tree on console
{
    if(node)
    {
        for(int i=0; i<dots; ++i)
        cout << ".";
    
        if(isId(node->data))
		{   
			cout << "<ID:"<<node->data<<">"<<endl;
		}
		else if(isInt(node->data))
		{
			cout << "<INT:"<<node->data<<">"<<endl;
		}
		else if(isString(node->data))
		{
			cout << "<STR:"<<node->data<<">"<<endl;
		}
		else
			cout << node->data << endl;
		
        preOrder(node->child, dots+1);
        preOrder(node->sibling, dots);
    }
}

void stdTree::run(const string &filename)
{
    p.run(filename);
    stdRoot = p.returnTop();
    standardize(stdRoot);
    preOrder(stdRoot, 0);
}

void stdTree::standardize(treeNode *node)
{
    if ( node == 0 )
        return;
    
    #ifdef DEBUG
        cout << node->data << endl;
    #endif    
    standardize(node->child);
    standardize(node->sibling);
    
    treeNode *temp;
    string data;
    data = node->data;
    
    if(data.compare("let")==0)
    {
        node->data = "gamma";
        node->child->data = "lambda";
        
        temp = node->child->sibling; //temp = p
        node->child->sibling = node->child->child->sibling; // p = e
        node->child->child->sibling = temp; // e = temp
    }
    else if(data.compare("where")==0)
    {
        node->data = "gamma";
        treeNode *p = node->child;
        treeNode *lambda = p->sibling;
        p->sibling = 0;
        
        lambda->data = "lambda";
        treeNode *x = lambda->child;
        treeNode *e = x->sibling;
        
        node->child = lambda;
        lambda->sibling = e;
        lambda->child = x;
        x->sibling = p;
    }
    else if(data.compare("tau")==0) //Not standardizing this. Can uncomment code in case change of mind.
    {
        //~ treeNode *gamma = node;
        //~ gamma->data = "gamma";
        //~ treeNode *e = gamma->child;
        //~ 
        //~ int count = 0;
        //~ stack<treeNode *> eStack;
        //~ while(e)
        //~ {
            //~ eStack.push(e);
            //~ e = e->sibling;
            //~ count++;
        //~ }
        //~ while( count != 0 )
        //~ {
            //~ treeNode *gamma2 = new treeNode();
            //~ gamma2->data = "gamma";
            //~ treeNode *aug = new treeNode();
            //~ aug->data = "aug";
            //~ gamma->child = gamma2;
            //~ e = eStack.top();
            //~ gamma2->sibling = e;
            //~ e->sibling = 0;
            //~ eStack.pop();
            //~ count--;
            //~ gamma2->child = aug;
            //~ if(count)
            //~ {
                //~ gamma = new treeNode();
                //~ gamma->data = "gamma";
                //~ aug->sibling = gamma;
            //~ }
            //~ else
            //~ {
                //~ treeNode *nil = new treeNode();
                //~ nil->data = "nil";
                //~ aug->sibling = nil;
                //~ break;
            //~ }
        //~ }
    }
    else if(data.compare("->")==0)
    {
        // do nothing
    }
    else if(data.compare("within")==0)
    {
        treeNode *x1 = node->child->child;
        treeNode *e1 = node->child->child->sibling;
        
        treeNode *x2 = node->child->sibling->child;
        treeNode *e2 = node->child->sibling->child->sibling;
        
        temp = node->child;
        temp->data = "gamma";
        node->data = "=";
        node->child = x2;
        x2->sibling = temp;
        
        treeNode *lambda = temp->sibling;
        lambda->data = "lambda";
        temp->sibling = 0; //no sibling to the right of gamma
        temp->child = lambda;
        lambda->sibling = e1;
        lambda->child = x1;
        x1->sibling = e2;
    }
    else if(data.compare("rec")==0) //do we need a deep copy? for the repeated x?
    {
        treeNode *x = node->child->child;
        treeNode *e = x->sibling;
        
        node->data = "=";
        
        treeNode *gamma = node->child;
        gamma->data = "gamma";
        
        treeNode *ystar = new treeNode();
        ystar->data = "Ystar";
        
        node->child = x;
        x->sibling = gamma;
        gamma->child = ystar;
        
        treeNode *lambda = new treeNode();
        lambda->data = "lambda";
        
        ystar->sibling = lambda;
        
        treeNode *x_copy = new treeNode();
        x_copy->data = x->data;
        
        lambda->child = x_copy;
        x_copy->sibling = e;
    }
    else if(data.compare("function_form")==0)
    {
        node->data = "=";
        treeNode *p = node->child;
        temp = p->sibling; //temp is first v
        treeNode *lambda = new treeNode();
        lambda->data = "lambda";
        p->sibling = lambda;
        lambda->child = temp;
        
        if(! (temp->sibling) )
        {
			cerr << "Unable to standardize 'function_form'\n";
		}
		
        while(temp->sibling->sibling) //repeat pattern
        {
            treeNode *temp2 = temp->sibling;
            treeNode *lambda_temp = new treeNode();
            lambda_temp->data = "lambda";
            temp->sibling = lambda_temp; // set temp->sibling to lambda;
            lambda_temp->child = temp2;
            temp = temp2; 
        }

    }
    else if(data.compare("lambda")==0) //two sub cases
    {
        if(node->child->data.compare(",")== 0 ) //case 1
        {
        }
        else //case 2
        {
            temp = node->child; //temp = v
            while(temp->sibling->sibling)
            {
                treeNode *lambda = new treeNode();
                lambda->data = "lambda";
                treeNode *temp2 = temp->sibling;
                temp->sibling = lambda;
                lambda->child = temp2;
                temp = temp2;
            }
        }
    }
    else if(isOp(data))
    {
        //do nothing
    }
    else if(data.compare("and")==0)
    {
        node->data = "=";
        temp = node->child; // temp and temp->siblings will be a list of '='
        treeNode *x = temp->child;
        treeNode *e = x->sibling;
        
        treeNode *comma = new treeNode();
        comma->data = ",";
        treeNode *tau = new treeNode();
        tau->data = "tau";
        
        node->child = comma;
        comma->child = x;
        comma->sibling = tau;
        tau->child = e;
        while(temp->sibling) //append the upcoming x's and e's to the x and e
        {
            treeNode *temp2 = temp->sibling;
            delete(temp); //should i comment this out in the beggining? or later
            temp = temp2;
            
            x->sibling = temp->child;
            x = x->sibling;
            
            e->sibling = temp->child->sibling;
            e = e->sibling;
        }
        x->sibling = 0;
        delete(temp);
    }
    else if(data.compare("@")==0)
    {
        node->data = "gamma";
        treeNode *gamma = new treeNode();
        gamma->data = "gamma";
        
        treeNode *e1 = node->child;
        treeNode *n = e1->sibling;
        treeNode *e2 = n->sibling;
        
        node->child = gamma;
        gamma->sibling = e2;
        gamma->child = n;
        n->sibling = e1;
        e1->sibling = 0;
    }
    else if(data.compare("not")==0 || data.compare("neg")) //Unary operators 
    {
        //do nothing
    }
}
