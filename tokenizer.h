#ifndef TOKENIZER_H
#define TOKENIZER_H
//#define DEBUG
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

// have a special check for comments

bool isLetter(char c)
{
    int index;
    index = int(c);
    
    if(index >= 65 && index <= 90)
        return true;
        
    if(index >= 97 && index <= 122)
        return true;
        
    return false;
}

bool isDigit(char c)
{
    int index;
    index = int(c);
    
    if(index >= 48 && index <= 57 )
        return true;
        
    return false;
}

bool isPunction(char c)
{
    if(c == '(' || c == ')' || c == ';' || c == ',' )
        return true;
        
    return false;
}

bool isOperator_symbol(char c)
{
    if (   c == '+' || c == '-' || c == '*' || c == '<' || c == '>' || c == '&' || c == '.'
        || c == '@' || c == '/' || c == ':' || c == '=' || c == '~' || c == '|' || c == '$'
        || c == '!' || c == '#' || c == '%' || c == '^' || c == '_' || c == '[' || c == ']'
        || c == '{' || c == '}' || c == '"' || c == '`' || c == '?' )
        {
            return true;
        }
        
    return false;
}

bool isSpace(char c)
{
    if(c == ' ' || c == '\t' || c == '\n' )
        return true;
    
    return false;
}

bool isKeyword(const string &s)
{
    if( s.compare("let") == 0 || 
    s.compare("in") == 0 || 
    s.compare("fn") == 0 || 
    s.compare("lambda") == 0 ||
    s.compare("where") == 0 ||
    s.compare("tau") == 0 || 
    s.compare("aug") == 0 || 
    s.compare("or") == 0 || 
    s.compare("not") == 0 || 
    s.compare("gr") == 0 || 
    s.compare("ge") == 0 || 
    s.compare("ls") == 0 || 
    s.compare("le") == 0 || 
    s.compare("eq") == 0 || 
    s.compare("ne") == 0 || 
    s.compare("true") == 0 || 
    s.compare("false") == 0 || 
    s.compare("nil") == 0 ||
    s.compare("gamma") == 0 || 
    s.compare("dummy") == 0 || 
    s.compare("within") == 0 || 
    s.compare("function_form") == 0 ||
    s.compare("and") == 0 ||
    s.compare("neg") == 0 || 
    s.compare("rec") == 0 )    
    {
        return true;
    }
    
    return false;
}

class token
{
    private:
    string t_class;
    string t_value;
    
    public:
    
    void setClass(const string &s);
    void setValue(const string &s);
    string getClass();
    string getValue();
};

void token::setClass(const string &s)
{
    t_class = s;
}

void token::setValue(const string &s)
{
    t_value = s;
}

string token::getClass()
{
    return t_class;
}

string token::getValue()
{
    return t_value;
}
 
class tokenList
{
    vector<token> tlist;
    int cur_tok;
    int tlist_size;

    public:
    
    tokenList()
    {  
        cur_tok = 0;  
        tlist_size = 0;
    }
    
    string getNextTokenVal(); //reads and updates index ie cur_tok
    string getCurTokenVal();
    string getCurTokenClass();
    string valOfNextToken(); // no update to cur_tok

    void init(const string &filename);
    void createTokenList(const string &filename);
    void updateKeywords();
    void showTokens();
    int getTlistSize()
    {
        return tlist_size;
    }
    int getCurIndex()
    {
        return cur_tok;
    }
};
string tokenList::valOfNextToken()
{
    if(cur_tok + 1 < tlist_size)
        return tlist[ cur_tok + 1 ].getValue();
    else
    {
        #ifdef DEBUG
        cerr << "Cannot read the value of the next token\n";
        #endif
        return "ERROR";
    }
}
string tokenList::getNextTokenVal()
{
    cur_tok++;
    if(cur_tok < tlist_size)
    {
        return tlist[cur_tok].getValue();
    }
    else
    {
        #ifdef DEBUG
        cout << "No more tokens to read\n";
        #endif
        return "ERROR";
    }
} 

string tokenList::getCurTokenVal()
{
    return tlist[cur_tok].getValue();
}

string tokenList::getCurTokenClass()
{
    return tlist[cur_tok].getClass();
}

void tokenList::init(const string &filename)
{
    createTokenList(filename);
    tlist_size = tlist.size();
    updateKeywords();
}

void tokenList::updateKeywords()
{
    for(int i = 0; i < tlist_size; i++)
    {
        if(isKeyword( tlist[i].getValue()) )
        {
            tlist[i].setClass("KEY");
        }
    }
}
void tokenList::createTokenList(const string &filename)
{
    ifstream fin;
    fin.open( filename.c_str() );
     
    if( !fin )
    {
        cerr << "File " << filename << " not found\n";
        return ;
    }
    
    fin.seekg(0, ios::end);
    int end = fin.tellg();
    
    int cur = 0;
    fin.seekg(0, ios::beg);
    
    while(cur != end)
    {
        token t;
        char c; // curr char read
        char nc; //next char read
        int caseindex;
        bool writeOp = false;
        string temp;
        
        c = fin.get(); // read a char from input file
        cur++;
        
        while( isSpace(c) && cur != end )
        {
            c = fin.get();
            cur++;
        }
        
        if( isLetter(c) )
            caseindex = 1;
        else if ( isDigit(c) )
            caseindex = 2;
        else if ( isPunction(c) )
            caseindex = 3;
        else if( c == '\'' ) //checking for the case of string
            caseindex = 4;
        else if( isOperator_symbol(c) )
            caseindex = 5;
        else
            caseindex = 9; // goes to defualt
        
        switch(caseindex)
        {
            case 1: // read character is a letter. check for <IDENTIFIER>
                temp = c;
                while(cur != end)
                {
                    nc = fin.get(); //read next character
                    cur++;
                    
                    if(isLetter(nc) || isDigit(nc) || nc == '_' )
                    {
                        c = nc;
                        temp.append( 1, c );
                    }
                    else
                        break;
                }
                
                t.setValue(temp);
                t.setClass("ID");
                tlist.push_back(t);
                break;
                
            case 2: // read character is a digit. check for <DIGIT>
                temp = c;
                while( cur != end)
                {
                    nc = fin.get();
                    cur++;
                    
                    if( isDigit( nc ) )
                    {
                        c = nc;
                        temp.append( 1 , c );
                    }
                    else
                        break;
                }
                
                t.setValue(temp);
                t.setClass("INT");
                tlist.push_back(t);
                break;
                
            case 3: // read character is a Punction '(' , ')' , ';' , ','
                temp = c;
                    #ifdef DEBUG
                    cout<< "Read char : " << c << " " <<int(c) << cur << endl;
                    #endif
                nc = fin.get(); // read one more character as we go back at the end of switch case by 1 pos
                cur++ ;
                t.setValue(temp);
                t.setClass("PUNCTION");
                tlist.push_back(t);
                break;
                
            case 4: // read character is ' . could be string
                temp = c;
                while(cur != end)
                {
                    nc = fin.get();
                    cur++;
                    if(cur == end && nc != '\'') // see that the string is closed at the end
                    {
                        cout << "String not closed properly in input\n";
                    }
                    
                    if( isLetter(nc) || isDigit(nc) || isOperator_symbol(nc) || isPunction(nc) 
                        || isSpace(nc) || nc == '\\'  )
                    {
                        //try to make a string token
                        temp.append(1,nc);
                    }
                    if( nc == '\'')
                    {
                        while(nc == '\'' && cur < end)
                        {
                            temp.append(1,nc);
                            cur++;
                            nc = fin.get();
                        }
                        break;
                    }
                }
                
                t.setValue(temp);
                t.setClass("STR");
                tlist.push_back(t);
                break;
                
            case 5: // could be operator symbol or comment
                temp = c;
                while( cur != end )
                {
                    nc = fin.get();
                    cur++;
                    
                    if( c == '/' && nc == '/' )
                    {
                        writeOp = false;
                        // everything till end of line is a comment
                        while(cur != end && nc!= '\n')
                        {
                            nc = fin.get();
                            cur++;
                        }
                        break;
                    }
                    else if(isOperator_symbol(nc))
                    {
                        writeOp = true;
                        c = nc;
                        temp.append(1, c);
                    }
                    else
                    {
                        writeOp = true;
                        break;
                    }
                }
                                
                if(writeOp)
                {
                    t.setValue(temp);
                    t.setClass("OPERATOR");
                    tlist.push_back(t);
                }
                break ;
            default :
                break ;// what cases have i missed ?
        }
        
        if(cur != end) 
        {
            cur--;
            fin.seekg(-1, ios::cur);
        }
    }
    
    fin.close();
}

void tokenList::showTokens()
{
    for(unsigned int i=0; i < tlist.size(); i++)
    {
        cout << "Class :" << tlist[i].getClass() << "  Value : " << tlist[i].getValue() << endl; 
    }
}
#endif
