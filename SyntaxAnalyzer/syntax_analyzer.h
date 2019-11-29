#pragma once
#include "lex_analyzer.h"
#include "syntax_component.h"
#include <unordered_map>
#include <vector>

using namespace std;

namespace EZCompiler{

class SyntaxKernel{
public:
    SyntaxKernel(const string& syntax_id);
    virtual ~SyntaxKernel(){}
    virtual bool GetStep(LexComponent lex_component, vector<SyntaxKernel*>& ext) const = 0;
    virtual vector<LexComponent> GetFirst() = 0;
    virtual string ToString() const;
    virtual bool IsTerminator() const = 0;
private:
    string syn_id;
};

class ComponentKernel : virtual public SyntaxKernel{
public:
    ComponentKernel(const string& id, bool nullable = false);
    void SetPatterns(const vector<vector<SyntaxKernel*>>& patt);
    bool GetStep(LexComponent lex_component, vector<SyntaxKernel*>& ext) const;
    vector<LexComponent> GetFirst();
    inline bool IsTerminator() const;
private:
    vector<vector<SyntaxKernel*>> patterns;
    unordered_map<LexComponent, int> FIRST;
    bool nullable, first_assigned = false;
};

class TerminatorKernel : virtual public SyntaxKernel{
public:
    TerminatorKernel(LexComponent lex_component, const string& id);
    bool GetStep(LexComponent lex_component, vector<SyntaxKernel*>& ext) const;
    vector<LexComponent> GetFirst();
    inline bool IsTerminator() const;
private:
    LexComponent comp;
};

class SyntaxAnalyzer{
public:
    ~SyntaxAnalyzer();
    void Clear();
    int CurrentPointer() const;
    vector<string> GetStep(LexComponent lex_component);
protected:
    SyntaxKernel* root = NULL;
    int cptr = 0;
    vector<SyntaxKernel*> kernels;
    vector<SyntaxKernel*> pattern;
};

class CLikeSyntaxAnalyzer : public SyntaxAnalyzer{
public:
    CLikeSyntaxAnalyzer();
};

}