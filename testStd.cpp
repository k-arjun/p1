//Testing the standardizer
#include "standardizer.h"

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        cout << "Enter the required switches to the program\n";
        return -1;
    }

    bool stSwitch = false;
    int i;

    for(i = 1; i < argc; ++i)
    {
        string s(argv[i]);
        if( s.compare("-st")==0 )
        {
            stSwitch = true;
            break;
        }
    }

    if( stSwitch && i==argc-1 )
        cout << "Please enter input program name\n";
    else if(!stSwitch)
        cout << "Please give -ast switch\n";
    else
    {
        string filename(argv[argc-1]);
        stdTree s;
        s.run(filename);
        return 0;
    }
}
