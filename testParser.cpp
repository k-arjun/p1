//Testing the parser
#include "parser.h"

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        cout << "Enter the required switches to the program\n";
        return -1;
    }

    bool AstSwitch = false;
    int i;

    for(i = 1; i < argc; ++i)
    {
        string s(argv[i]);
        if( s.compare("-ast")==0 )
        {
            AstSwitch = true;
            break;
        }
    }

    if( AstSwitch && i==argc-1 )
        cout << "Please enter input program name\n";
    else if(!AstSwitch)
        cout << "Please give -ast switch\n";
    else
    {
        string filename(argv[argc-1]);
        parser p;
        p.run(filename);
        return 0;
    }
}
