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

bool ComponentKernel::GetStep(LexComponent lex_component, vector<SyntaxKernel*>& ext) const{
    auto target = FIRST.find(lex_component);
    if(target == FIRST.end())
        if(!nullable) thr("Parsing error occurs.");
        else{
            ext = vector<SyntaxKernel*>(1, (SyntaxKernel*)this);
            return false;
        }
    else ext = patterns[target->second];
    return true;
}

vector<LexComponent> ComponentKernel::GetFirst() {
    if(!first_assigned){
        for(int i = 0; i < patterns.size(); ++i){
            if(patterns[i].empty()) continue;
            for(auto& p : patterns[i][0]->GetFirst()){
                if(FIRST.find(p) != FIRST.end()){
                    thr("Syntax input not LL(1)");
                }
                else FIRST.insert(make_pair(p, i));
            }
        }
        first_assigned = true;
    }

    vector<LexComponent> res;
    res.reserve(FIRST.size());
    for(auto& f : FIRST)
        res.push_back(f.first);
    return res;
}

bool ComponentKernel::IsTerminator() const{
    return false;
}

TerminatorKernel::TerminatorKernel(LexComponent lex_cmp, const string& id) : comp(lex_cmp),
    SyntaxKernel(id){}

bool TerminatorKernel::GetStep(LexComponent lex_component, vector<SyntaxKernel*>& ext) const{
    if(lex_component == comp) ext = vector<SyntaxKernel*>(1, (SyntaxKernel*)this);
    else thr("Parsing error occurs.");
    return true;
}

vector<LexComponent> TerminatorKernel::GetFirst(){
    return vector<LexComponent>(1, comp);
}

bool TerminatorKernel::IsTerminator() const{
    return true;
}

SyntaxAnalyzer::~SyntaxAnalyzer(){
    for(int i = 0; i < kernels.size(); ++i)
        delete kernels[i];
}

void SyntaxAnalyzer::Clear(){
    pattern.clear();
    pattern.push_back(root);
    cptr = 0;
}

int SyntaxAnalyzer::CurrentPointer() const{
    return cptr;
}

vector<string> SyntaxAnalyzer::GetStep(LexComponent lex_component){
    if(lex_component == LexComponentNull) pattern.clear();

    while(cptr < pattern.size()){
        vector<SyntaxKernel*> partial_pattern;
        if(!pattern[cptr]->GetStep(lex_component, partial_pattern)){
            ++cptr;
            if(cptr >= pattern.size()) thr("Parsing error occurs.");
            continue;
        }
        partial_pattern.insert(partial_pattern.begin(),
            pattern.begin(), pattern.begin() + cptr);
        partial_pattern.insert(partial_pattern.end(),
            pattern.begin() + cptr + 1, pattern.end());
        swap(partial_pattern, pattern);
        if(pattern[cptr]->IsTerminator()) break;
    }

    ++cptr;

    vector<string> result(pattern.size());
    for(int i = 0; i < pattern.size(); ++i) result[i] = pattern[i]->ToString();
    return result;
}

CLikeSyntaxAnalyzer::CLikeSyntaxAnalyzer(){
    #define def_component(name, nullable)  \
        ComponentKernel* name = new ComponentKernel(#name, nullable);   \
        kernels.push_back(name)

        def_component(Program, false);
        def_component(Declstr, false);
        def_component(Declaration, false);
        def_component(Declstr_, true);
        def_component(DeclType, false);
        def_component(FunDecl, false);
        def_component(VarDecl, false);
        def_component(FormalPar, false);
        def_component(Block, false);
        def_component(ParList, false);
        def_component(Parameter, false);
        def_component(ParList_, true);
        def_component(InDecl, true);
        def_component(StatementStr, false);
        def_component(Statement, false);
        def_component(InVarDecl, false);
        def_component(InDecl_, true);
        def_component(StatementStr_, true);
        def_component(IfState, false);
        def_component(IfState_, true);
        def_component(WhileState, false);
        def_component(RetState, false);
        def_component(AssignState, false);
        def_component(Expr, false);
        def_component(RetState_, true);
        def_component(AddExpr, false);
        def_component(RelationOperator, false);
        def_component(Expr_, true);
        def_component(AddExpr_, true);
        def_component(Item, false);
        def_component(Factor, false);
        def_component(Item_, true);
        def_component(Const, false);
        def_component(FunCall, true);
        def_component(ActualParList, false);
        def_component(ActualPar, true);
        def_component(ActualParList_, true);

    #undef def_component

    #define def_terminator(name, lex_component)  \
        TerminatorKernel* name = new TerminatorKernel(lex_component, #name);   \
        kernels.push_back(name)

        def_terminator(ID, LexComponent::LexComponentWord);
        def_terminator(Int, LexComponent::LexComponentInt);
        def_terminator(Void, LexComponent::LexComponentVoid);
        def_terminator(Semicolon, LexComponent::LexComponentSemicolon);
        def_terminator(LeftBracket, LexComponent::LexComponentLeftBracket);
        def_terminator(RightBracket, LexComponent::LexComponentRightBracket);
        def_terminator(Comma, LexComponent::LexComponentComma);
        def_terminator(LeftBrace, LexComponent::LexComponentLeftBrace);
        def_terminator(RightBrace, LexComponent::LexComponentRightBrace);
        def_terminator(Assign, LexComponent::LexComponentAssignment);
        def_terminator(Return, LexComponent::LexComponentReturn);
        def_terminator(While, LexComponent::LexComponentWhile);
        def_terminator(If, LexComponent::LexComponentIf);
        def_terminator(Else, LexComponent::LexComponentElse);
        def_terminator(Plus, LexComponent::LexComponentPlus);
        def_terminator(Minus, LexComponent::LexComponentMinus);
        def_terminator(Greater, LexComponent::LexComponentGreater);
        def_terminator(Less, LexComponent::LexComponentLess);
        def_terminator(GE, LexComponent::LexComponentGE);
        def_terminator(LE, LexComponent::LexComponentLE);
        def_terminator(Equal, LexComponent::LexComponentEqual);
        def_terminator(NotEqual, LexComponent::LexComponentNotEqual);
        def_terminator(Multiply, LexComponent::LexComponentMultiply);
        def_terminator(Divide, LexComponent::LexComponentDivide);
        def_terminator(Integer, LexComponent::LexComponentInteger);
        def_terminator(Floating, LexComponent::LexComponentFloating);

    #undef def_terminator

    #define set_patterns(name, ...) \
        vector<vector<SyntaxKernel*>> name##Patterns = __VA_ARGS__;    \
        name->SetPatterns(name##Patterns)

        set_patterns(Program, {{Declstr}});
        set_patterns(Declstr, {{Declaration, Declstr_}});
        set_patterns(Declstr_, {{Declstr}});
        set_patterns(Declaration, {{Int, ID, DeclType}, {Void, ID, FunDecl}});
        set_patterns(DeclType, {{VarDecl}, {FunDecl}});
        set_patterns(VarDecl, {{Semicolon}});
        set_patterns(FunDecl, {{LeftBracket, FormalPar, RightBracket, Block}});
        set_patterns(FormalPar, {{ParList}, {Void}});
        set_patterns(ParList, {{Parameter, ParList_}});
        set_patterns(ParList_, {{Comma, ParList, ParList_}});
        set_patterns(Parameter, {{Int, ID}});
        set_patterns(Block, {{LeftBrace, InDecl, StatementStr, RightBrace}});
        set_patterns(InDecl, {{InVarDecl, InDecl_}});
        set_patterns(InDecl_, {{Semicolon, InDecl, InDecl_}});
        set_patterns(InVarDecl, {{Int, ID}});
        set_patterns(StatementStr, {{Statement, StatementStr_}});
        set_patterns(StatementStr_, {{StatementStr}});
        set_patterns(Statement, {{IfState}, {WhileState}, {RetState}, {AssignState}});
        set_patterns(AssignState, {{ID, Assign, Expr, Semicolon}});
        set_patterns(RetState, {{Return, RetState_, Semicolon}});
        set_patterns(RetState_, {{Expr}});
        set_patterns(WhileState, {{While, LeftBracket, Expr, RightBracket, Block}});
        set_patterns(IfState, {{If, LeftBracket, Expr, RightBracket, Block, IfState_}});
        set_patterns(IfState_, {{Else, Block}});
        set_patterns(Expr, {{AddExpr, Expr_}});
        set_patterns(Expr_, {{RelationOperator, AddExpr, AddExpr_}});
        set_patterns(AddExpr, {{Item, AddExpr_}});
        set_patterns(AddExpr_, {{Plus, Item, AddExpr_}, {Minus, Item, AddExpr_}});
        set_patterns(RelationOperator, {{LE}, {GE}, {NotEqual}, {Equal}, {Greater}, {Less}});
        set_patterns(Item, {{Factor, Item_}});
        set_patterns(Item_, {{Multiply, Factor, Item_}, {Divide, Factor, Item_}});
        set_patterns(Factor, {{Const}, {LeftBracket, Expr, RightBracket}, {ID, FunCall}});
        set_patterns(FunCall, {{LeftBracket, ActualParList, RightBracket}});
        set_patterns(ActualPar, {{ActualParList}});
        set_patterns(ActualParList, {{Expr, ActualParList_}});
        set_patterns(ActualParList_, {{Comma, Expr, ActualParList_}});
        set_patterns(Const, {{Integer}, {Floating}});

    #undef set_patterns

    root = Program;
    pattern.push_back(root);

    for(auto& kernel : kernels) kernel->GetFirst();
}

}