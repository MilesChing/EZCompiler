#include "syntax_analyzer.h"
#include <iostream>
using namespace std;

namespace EZCompiler{

static inline void thr(const string& message){
    cerr << message << endl;
    throw message;
}

SyntaxKernel::SyntaxKernel(const string& syn) : syn_id(syn){}

string SyntaxKernel::ToString() const{
    return syn_id;
}

ComponentKernel::ComponentKernel(const string& id, bool n)
    : SyntaxKernel(id), nullable(n){}

void ComponentKernel::SetPatterns(const vector<vector<SyntaxKernel*>>& patt){
    patterns = patt;
    if(patt.empty()) thr("Patterns must not be empty.");
}

vector<SyntaxKernel*> ComponentKernel::GetStep(LexComponent lex_component) const{
    auto target = FIRST.find(lex_component);
    if(target == FIRST.end())
        if(!nullable) thr("Parsing error occurs.");
        else return vector<SyntaxKernel*>();
    else return patterns[target->second];
}

vector<LexComponent> ComponentKernel::GetFirst() {
    for(int i = 0; i < patterns.size(); ++i){
        if(patterns[i].empty()) continue;
        for(auto& p : patterns[i][0]->GetFirst()){
            if(FIRST.find(p) != FIRST.end())
                thr("Syntax input not LL(1)");
            else FIRST.insert(make_pair(p, i));
        }
    }
}

TerminatorKernel::TerminatorKernel(LexComponent lex_cmp, const string& id) : comp(lex_cmp),
    SyntaxKernel(id){}

vector<SyntaxKernel*> TerminatorKernel::GetStep(LexComponent lex_component) const{
    if(lex_component == comp) return vector<SyntaxKernel*>();
    else thr("Parsing error occurs.");
}

vector<LexComponent> TerminatorKernel::GetFirst(){
    return vector<LexComponent>(1, comp);
}

SyntaxAnalyzer::~SyntaxAnalyzer(){
    for(auto p : kernels) delete p;
}

void SyntaxAnalyzer::Clear(){
    pattern.clear();
    pattern.push_back(root);
    cptr = 0;
}

vector<string> SyntaxAnalyzer::GetStep(LexComponent lex_component){
    if(lex_component == LexComponentNull) pattern.clear();
    if(cptr >= pattern.size()) pattern.clear();
    else{
        vector<SyntaxKernel*> partial_pattern =
            pattern[cptr]->GetStep(lex_component);
        partial_pattern.insert(partial_pattern.begin(),
            pattern.begin(), pattern.begin() + cptr);
        partial_pattern.insert(partial_pattern.end(),
            pattern.begin() + cptr + 1, pattern.end());
        swap(partial_pattern, pattern);
    }

    vector<string> result(pattern.size());
    for(int i = 0; i < pattern.size(); ++i) result[i] = pattern[i]->ToString();
    return result;
}

}