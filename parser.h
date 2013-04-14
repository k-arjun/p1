//Parser
#ifndef PARSER_H
#define PARSER_H
#include "tokenizer.h"
#include <cstdlib>
#include <stack>

struct treeNode
{
    string data;
    treeNode *child;
    treeNode *sibling;
};

class parser
{
    // DATA MEMBERS
    string NT; //keeps the next token
    tokenList tl;
    stack<treeNode *> treeStack;
    //stack<treeNode *> standardStack;
    
    // PRIVATE FUNCTIONS
    
    // RPALs Phrase Structure Grammar:
    void proc_E();
    void proc_Ew();
    void proc_T();
    void proc_Ta();
    void proc_Tc();
    void proc_B();
    void proc_Bt();
    void proc_Bs();
    void proc_Bp();
    void proc_A();
    void proc_At();
    void proc_Af();
    void proc_Ap();
    void proc_R();
    void proc_Rn();
    void proc_D();
    void proc_Da();
    void proc_Dr();
    void proc_Db();
    void proc_Vb();
    void proc_Vl();
    
    //Auxillary functions
    
    void buildTree(string node, int pops);
    void readToken(string readTok);
    //void standardize(); //makes a standard tree from the AST
    //void stdTree(treeNode *node); //helper function for the standardize function
    bool isId(); //Returns true is current val of NT is an Identifier
    bool isInt(); //Returns true is current val of NT is an Integer
    bool isString();
    bool isId(string s);
    bool isInt(string s);
    bool isString(string s);
    void preOrder(treeNode *node, int dots);
    void delNodes(treeNode *node); //helper function for the destructor
    bool endReads();
    
    // PUBLIC FUNCTIONS
    public:
    void run(const string &filename);
    void printAST();
    treeNode* returnTop();
    //~parser();
    
};

//~ parser::~parser()
//~ {
    //~ treeNode *root;
    //~ root = treeStack.top();
    //~ delNodes(root);
//~ }

void parser::delNodes(treeNode *node)
{
    if(node)
    {
        delNodes(node->child);
        delNodes(node->sibling);
        delete(node);
    }
}

treeNode* parser::returnTop()
{
    treeNode *root = treeStack.top();
    return root;
}

void parser::run(const string &filename)
{
    tl.init(filename);
    NT = tl.getCurTokenVal();
    proc_E();
    //printAST(); //dont need it right now.
}

void parser::printAST()
{
    if(treeStack.size() != 1)
    {
        cerr << "AST not made properly. More than one node in the TreeStack\n";
        exit(-1);
    }
    
    treeNode *root;
    root = treeStack.top();
    preOrder(root,0);
}

void parser::preOrder(treeNode *node, int dots)
{
    if(node)
    {
        for(int i=0; i<dots; ++i)
        cout << ".";
    
        cout << node->data << endl;
        preOrder(node->child, dots+1);
        preOrder(node->sibling, dots);
    }
}

void parser::buildTree(string node, int pops)
{
    treeNode *newNode = new treeNode();
    newNode->child = 0;
    newNode->sibling = 0;
    string tempName;
    //~ if( tl.getCurTokenClass().compare("ID") == 0 || tl.getCurTokenClass().compare("INT") == 0 
        //~ || tl.getCurTokenClass().compare("STR") == 0)
    //~ {
        //~ newNode->data = tempName.append("<").append( tl.getCurTokenClass() ).append(":").append(node).append(">");
    //~ }

    if(isId(node))
    {   
        newNode->data = tempName.append("<ID:").append(node).append(">");
    }
    else if(isInt(node))
    {
        newNode->data = tempName.append("<INT:").append(node).append(">");
    }
    else if(isString(node))
    {
        newNode->data = tempName.append("<STR:").append(node).append(">");
    }
    else
        newNode->data = node;
        
    if(pops == 0)
    {
        treeStack.push(newNode);
    }
    else
    {
        treeNode *tempNode;
        tempNode = treeStack.top();
        treeStack.pop();
        while(pops != 1)
        {
            treeNode *tempSibling;
            tempSibling = treeStack.top();
            treeStack.pop();
            pops--;
            
            tempSibling->sibling = tempNode;
            tempNode = tempSibling;
        }
        
        newNode->child = tempNode;
        treeStack.push(newNode);
    }
}

void parser::readToken(string readTok)
{
    if( readTok.compare(NT) != 0 )
    {
        cerr << "Expected " << readTok << ", but found " << NT << endl;
        exit(-1);
    }
    if(endReads())
    {
        cerr << "Cannot read anymore tokens from the file\n";
        exit(-1);
    }
    if( tl.getCurTokenClass().compare("ID") == 0 || tl.getCurTokenClass().compare("INT") == 0 || tl.getCurTokenClass().compare("STR") == 0)
    {
        buildTree(readTok, 0);
    }
    
    if(!endReads())
        NT = tl.getNextTokenVal();
}

bool parser::endReads()
{
    if(tl.getCurIndex() == tl.getTlistSize())
        return true;
    else
        return false;
}

bool parser::isId()
{
    if(tl.getCurTokenClass().compare("ID") == 0)
        return true;
        
    return false;
}

bool parser::isInt()
{
    if(tl.getCurTokenClass().compare("INT") == 0)
        return true;
        
    return false;
}

bool parser::isString()
{
    if(tl.getCurTokenClass().compare("STR") == 0)
        return true;
        
    return false;
}

bool parser::isId(string s)
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

bool parser::isInt(string s)
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

bool parser::isString(string s)
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

void parser::proc_E()
{
    if(NT.compare("let") == 0 )
    {
        readToken("let");
        proc_D();
        readToken("in");
        proc_E();
        buildTree("let",2);
    }
    else if(NT.compare("fn") == 0 )
    {
        int n = 0;
        readToken("fn");
        do{
            proc_Vb();
            n++;
        }while( NT.compare("(") == 0 || isId(NT) );
        
        readToken(".");
        proc_E();
        buildTree("lambda",n+1);
    }
    else
    {
        proc_Ew();
    }
}

void parser::proc_Ew()
{
    proc_T();
    if(NT.compare("where") == 0 )
    {
        readToken("where");
        proc_Dr();
        buildTree("where",2);
    }
}

void parser::proc_T()
{
    proc_Ta();
    
    int n = 1;
    while(NT.compare(",") == 0 )
    {
        n++;
        readToken(",");
        proc_Ta();
    }
    if( n > 1)
    {
        buildTree("tau", n);
    }
}

void parser::proc_Ta()
{
    proc_Tc();
    while(NT.compare("aug") == 0 )
    {
        readToken("aug");
        proc_Tc();
        buildTree("aug",2);
    }
}

void parser::proc_Tc()
{
    proc_B();
    
    if(NT.compare("->") == 0 )
    {
        readToken("->");
        proc_Tc();
        readToken("|");
        proc_Tc();
        buildTree("->",3);
    }
}

void parser::proc_B()
{
    proc_Bt();
    while(NT.compare("or") == 0 )
    {
        readToken("or");
        proc_Bt();
        buildTree("or",2);
    }
}

void parser::proc_Bt()
{
    proc_Bs();
    while(NT.compare("&") == 0 )
    {
        readToken("&");
        proc_Bs();
        buildTree("&", 2);
    }
}

void parser::proc_Bs()
{
    if(NT.compare("not") == 0 )
    {
        readToken("not");
        proc_Bp();
        buildTree("not", 1);
    }
    else
    {
        proc_Bp();
    }
}

void parser::proc_Bp()
{
    string temp;
    proc_A();
    if( NT.compare("gr") == 0 || NT.compare(">") == 0)
    {
        if( NT.compare("gr") == 0 )
            temp = "gr";
        else
            temp = ">";
            
        readToken(temp);
        proc_A();
        buildTree("gr",2);
    }
    else if (NT.compare("ge") == 0 || NT.compare(">=") == 0)
    {
        if( NT.compare("ge") == 0 )
            temp = "ge";
        else
            temp = ">=";
        
        readToken(temp);
        proc_A();
        buildTree("ge",2);
    }
    else if (NT.compare("ls") == 0 || NT.compare("<") == 0)
    {
        if( NT.compare("ls") == 0 )
            temp = "ls";
        else
            temp = "<";
            
        readToken(temp);
        proc_A();
        buildTree("ls",2);
    }
    else if (NT.compare("le") == 0 || NT.compare("<=") == 0)
    {
        if( NT.compare("le") == 0 )
            temp = "le";
        else
            temp = "<=";
            
        readToken(temp);
        proc_A();
        buildTree("le",2);
    }
    else if (NT.compare("eq") == 0)
    {
        readToken("eq");
        proc_A();
        buildTree("eq",2);
    }
    else if (NT.compare("ne") == 0)
    {
        readToken("ne");
        proc_A();
        buildTree("ne",2);
    }
}

void parser::proc_A()
{
    if (NT.compare("+") == 0)
    {
        readToken("+");
        proc_At();
    }
    else if (NT.compare("-") == 0)
    {
        readToken("-");
        proc_At();
        buildTree("neg",1);
    }
    else
    {
        proc_At();
    }
    
    while (NT.compare("+") == 0 || NT.compare("-") == 0)
    {
        if (NT.compare("+") == 0)
        {
            readToken("+");
            proc_At();
            buildTree("+",2);
        }
        else if (NT.compare("-") == 0)
        {
            readToken("-");
            proc_At();
            buildTree("-",2);
        }
    }
}

void parser::proc_At()
{
    proc_Af();
    
    while( (NT.compare("*") == 0 || NT.compare("/") == 0) && !endReads() )
    {
        if(NT.compare("*") == 0)
        {
            readToken("*");
            proc_Af();
            buildTree("*",2);
        }
        else if(NT.compare("/") == 0)
        {
            readToken("/");
            proc_Af();
            buildTree("/",2);
        }
    }
}

void parser::proc_Af()
{
    proc_Ap();
    
    if( (NT.compare("**") == 0) && !endReads() )
    {
        readToken("**");
        proc_Af();
        buildTree("**",2);
    }
}

void parser::proc_Ap()
{
    proc_R();
    
    while( (NT.compare("@") == 0) && !endReads() )
    {
        readToken("@");
        if( tl.getCurTokenClass().compare("ID") != 0)
        {
            cerr << "Expecting IDENTIFIER, recieved " << tl.getCurTokenClass();
        }
        else
        {
            readToken(NT);
            proc_R();
            buildTree("@",3);
        }
    }
}

void parser::proc_R()
{
    proc_Rn();
    
    while( (NT.compare("true") == 0 || NT.compare("false") == 0 || NT.compare("nil") == 0 || NT.compare("dummy") == 0
         || NT.compare("(") == 0 || isId(NT) || isInt(NT) || isString(NT) ) && !endReads())
     {
         proc_Rn();
         buildTree("gamma",2);
     }
}

void parser::proc_Rn()
{
    if( NT.compare("true") == 0 || NT.compare("false") == 0 || NT.compare("nil") == 0 || NT.compare("dummy") == 0)
    {
        string temp("<");
        temp.append(NT).append(">");
        readToken(NT);
        buildTree(temp, 0);
    }
    else if( NT.compare("(") == 0)
    {
        readToken("(");
        proc_E();
        readToken(")");
    }
    else
    {
        //read identifier, integer, string
        readToken(NT);
    }
}

void parser::proc_D()
{
    proc_Da();
    
    if( NT.compare("within") == 0 )
    {
        readToken("within");
        proc_Da();
        buildTree("within",2);
    }
}

void parser::proc_Da()
{
    proc_Dr();
    int n = 1;
    while( NT.compare("and") == 0 )
    {
        n++;
        readToken("and");
        proc_Dr();
    }
    if( n > 1)
    {
        buildTree("and", n);
    }
}

void parser::proc_Dr()
{
    if( NT.compare("rec") == 0 )
    {
        readToken("rec");
        proc_Db();
        buildTree("rec", 1);
    }
    else
        proc_Db();
    
}

void parser::proc_Db()
{
    if( NT.compare("(") == 0 )
    {
        readToken("(");
        proc_D();
        readToken(")");
    }
    else
    {
        //check whether the next two tokens are Identifiers or not.
        //if both are identifiers build fcn_form
        //if only one is an identifier build =
        if(isId(NT))
        {            
            //check if the next char is "," or "=" or not
            if( tl.valOfNextToken().compare("=")==0 || tl.valOfNextToken().compare(",")==0) //Vl = E
            {
                proc_Vl();
                readToken("=");
                proc_E();
                buildTree("=",2);
            }
            else // <ID> Vb+ = E
            {
                int n = 1;
                readToken(NT); //read the <ID>
                while( NT.compare("(") == 0 || isId() )
                {
                    n++;
                    proc_Vb();
                }
                
                readToken("=");
                proc_E();
                buildTree("function_form",n + 1); 
            }
        }
        else
        {
            cerr << "Was expecting to read an Identifier. Received " << NT << endl;
        }
    }
}

void parser::proc_Vb()
{
    if( isId() )
    {
        readToken(NT);
    }
    else if( NT.compare("(")==0)
    {
        readToken("(");
        
        if( NT.compare(")")==0 )
        {
            readToken(")");
            buildTree("()",0);
        }
        else
        {
            proc_Vl();
            readToken(")");
        }
    }
}

void parser::proc_Vl()
{
    int n = 1;
    readToken(NT); // read the <ID> at the beginning of the list
    while( NT.compare(",")==0 )
    {
        n++;
        readToken(",");
        if(isId())
        {
            readToken(NT);
        }
    }
    if( n > 1 )
    {
        buildTree(",", n);
    }
}
#endif
