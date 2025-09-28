#include <string>

enum Token {
    token_eof = -1,
    token_def = -2,
    token_extern = -3,
    token_identifier = -4,
    token_number = -5
};

static std::string IdentifierStr;
static double NumVal;


static int getToken()
{
    static int LastChar = ' ';
    while(isspace(LastChar))
        LastChar = getchar();


    if(isalpha(LastChar))
    {
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;

        if (IdentifierStr == "def") return token_def;
        if (IdentifierStr == "exter") return token_extern;

        return token_identifier;
    }

    if(isdigit(LastChar) || LastChar == '.')
    {
        std::string NumStr;
        do
        {
            NumStr += LastChar;
            LastChar = getchar();

        } while (isdigit(LastChar) || LastChar == '.');
        
        NumVal = strtod(NumStr.c_str(), 0);
        return token_number;
    }

    if(LastChar == '#'){
        do
            LastChar = getchar();
        while(LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if(LastChar != EOF) return getToken();
    }   

    if (LastChar == EOF) return token_eof;

    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;

}

