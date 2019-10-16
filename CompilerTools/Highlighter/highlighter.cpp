#include "Highlighter/highlighter.h"
#include <iostream>
using namespace std;
namespace EZCompiler{

Highlighter::Highlighter(const string& code, const LexAnalyzeResult& res) :
    original_code(code){
    component_type.clear();
    component_type.resize(code.length(), LexComponentNull);
    for(auto& seg : res)
        for(int i = seg.left_index; i < seg.right_index; ++i)
            component_type[i] = seg.component_type;
}

void Highlighter::Output(){
    int c_color = 3986;
    resetFColor();
    resetBColor();
    cout << "------------------------------" << endl;
    for(int i = 0; i < original_code.length(); ++i){
        int o_color = Highlighter::getColor(component_type[i]);
        if(c_color != o_color){
            c_color = o_color;
            setColor(o_color);
        }
        cout << original_code[i];
    }
    resetFColor();
    resetBColor();
    cout << endl << "------------------------------" << endl;
}

}