#pragma once
#include "lex_analyzer.h"
#include "syntax_component.h"
#include <unordered_map>
#include <vector>

using namespace std;

namespace EZCompiler{

class SyntaxAnalyzerKernel{
public:
    SyntaxAnalyzerKernel(const vector<vector<SyntaxAnalyzerKernel*>>& pattern, SyntaxComponent component);
    vector<SyntaxAnalyzerKernel*> GetStep(LexComponent lex_component) const;
    bool TermSymbol() const;
    SyntaxComponent Component() const;
protected:
    vector<vector<SyntaxAnalyzerKernel*>> patterns;
    unordered_map<LexComponent, int> FIRST;
    SyntaxComponent component;
};

class SyntaxAnalyzer{
public:
    ~SyntaxAnalyzer();
    void Clear();
    vector<SyntaxComponent> GetStep(LexComponent lex_component);
protected:
    SyntaxAnalyzerKernel* root = NULL;
    vector<SyntaxAnalyzerKernel*> kernels;
    vector<SyntaxAnalyzerKernel*> pattern;
};

}