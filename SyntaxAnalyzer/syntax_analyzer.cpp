#include "syntax_analyzer.h"
#include <iostream>
using namespace std;

namespace EZCompiler{

SyntaxAnalyzerKernel::SyntaxAnalyzerKernel(const vector<vector<SyntaxAnalyzerKernel*>>& patt, SyntaxComponent comp) : patterns(patt), component(comp){
    for(int i = 0; i < patterns.size(); ++i){
        if(patterns[i].empty()) continue;
        for(auto& p : patterns[i][0]->FIRST){
            if(FIRST.find(p.first) != FIRST.end()){
                cerr << "Syntax input not LL(1)" << endl;
                throw -1;
            }
            else FIRST.insert(make_pair(p.first, i));
        }
    }
}

vector<SyntaxAnalyzerKernel*> SyntaxAnalyzerKernel::GetStep(LexComponent lex_component) const{
    auto target = FIRST.find(lex_component);
    if(target == FIRST.end())
        return vector<SyntaxAnalyzerKernel*>();
    return patterns[target->second];
}

bool SyntaxAnalyzerKernel::TermSymbol() const{
    return patterns.empty();
}

SyntaxComponent SyntaxAnalyzerKernel::Component() const{
    return component;
}

SyntaxAnalyzer::~SyntaxAnalyzer(){
    for(auto p : kernels) delete p;
}

void SyntaxAnalyzer::Clear(){
    pattern.clear();
    pattern.push_back(root);
}

vector<SyntaxComponent> SyntaxAnalyzer::GetStep(LexComponent lex_component){
    for(int i = 0; i < pattern.size(); ++i){
        if(pattern[i]->TermSymbol()) continue;
        vector<SyntaxAnalyzerKernel*> inner_pattern = pattern[i]->GetStep(lex_component);
        if(inner_pattern.empty()){
            pattern.clear();
            break;
        }
        inner_pattern.insert(inner_pattern.begin(),
            pattern.begin(), pattern.begin() + i);
        inner_pattern.insert(inner_pattern.end(),
            pattern.begin() + i + 1, pattern.end());
        swap(inner_pattern, pattern);
        break;
    }

    vector<SyntaxComponent> result(pattern.size());
    for(int i = 0; i < pattern.size(); ++i)
        result[i] = pattern[i]->Component();
    return result;
}

}