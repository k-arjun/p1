// read a file and create tokens out of it.
#include "tokenizer.h"
   
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cout << "Enter 1 filename argument to the program\n";
        return 0;
    }
    string s(argv[1]);
    cout << "The entered filename is " << s << endl;
    
    tokenList t;
    t.init(s);
    t.showTokens();
}
