#ifndef _HIGHLIGHTER_
#define _HIGHLIGHTER_

#include "lex_component.h"
#include "lex_analyzer.h"
#include "Highlighter/colors.h"
#include <vector>
#include <string>
using namespace std;
namespace EZCompiler{

class Highlighter{
public:
    Highlighter(const string& code, const LexAnalyzeResult& res);
    void Output();
private:
    string original_code;
    vector<LexComponent> component_type;
    static int getColor(LexComponent component){
        if(is_keyword(component)) return F_PURPLE;
        else if(is_operator(component)) return F_YELLOW;
        else if(is_bracket(component)) return F_RED;
        else if(is_value(component)) return F_GREEN;
        else if(component == LexComponentComment) return F_BLUE;
        else return F_WHITE;
    }
};

}

#endif
