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
    virtual vector<SyntaxKernel*> GetStep(LexComponent lex_component) const = 0;
    virtual vector<LexComponent> GetFirst() = 0;
    virtual string ToString() const;
private:
    string syn_id;
};

class ComponentKernel : virtual public SyntaxKernel{
public:
    ComponentKernel(const string& id, bool nullable = false);
    void SetPatterns(const vector<vector<SyntaxKernel*>>& patt);
    vector<SyntaxKernel*> GetStep(LexComponent lex_component) const;
    vector<LexComponent> GetFirst();
private:
    vector<vector<SyntaxKernel*>> patterns;
    unordered_map<LexComponent, int> FIRST;
    bool nullable;
};

class TerminatorKernel : virtual public SyntaxKernel{
public:
    TerminatorKernel(LexComponent lex_component, const string& id);
    vector<SyntaxKernel*> GetStep(LexComponent lex_component) const;
    vector<LexComponent> GetFirst();
private:
    LexComponent comp;
};

class SyntaxAnalyzer{
public:
    ~SyntaxAnalyzer();
    void Clear();
    vector<string> GetStep(LexComponent lex_component);
protected:
    SyntaxKernel* root = NULL;
    int cptr = 0;
    vector<SyntaxKernel*> kernels;
    vector<SyntaxKernel*> pattern;
};

class CLikeSyntaxAnalyzer : SyntaxAnalyzer{
public:
    inline CLikeSyntaxAnalyzer(){
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
            def_component(RetState_, false);
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
            def_terminator(Equal, LexComponent::LexComponentEqual);
            def_terminator(Return, LexComponent::LexComponentReturn);
            def_terminator(While, LexComponent::LexComponentWhile);
            def_terminator(If, LexComponent::LexComponentIf);
            def_terminator(Else, LexComponent::LexComponentElse);
            def_terminator(Plus, LexComponent::LexComponentPlus);
            def_terminator(Minus, LexComponent::LexComponentMinus);
            def_terminator(Greater, LexComponent::LexComponentGreater);
            def_terminator(Less, LexComponent::LexComponentLess);
            def_terminator(GE, LexComponent::LexComponentGE);
            def_terminator(LE, LexComponent::LexComponentGE);
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
            set_patterns(FunDecl, {{LeftBrace, FormalPar, RightBrace, Block}});
            set_patterns(FormalPar, {{ParList}, {Void}});
            set_patterns(ParList, {{Parameter, ParList_}});
            set_patterns(ParList_, {{Comma, ParList, ParList_}});
            set_patterns(Parameter, {{Int, ID}});
            set_patterns(Block, {{InDecl, StatementStr}});
            set_patterns(InDecl, {{InVarDecl, InDecl_}});
            set_patterns(InDecl_, {{Semicolon, InVarDecl, InDecl_}});
            set_patterns(InVarDecl, {{Int, ID}});
            set_patterns(StatementStr, {{Statement, StatementStr_}});
            set_patterns(StatementStr_, {{StatementStr}});
            set_patterns(Statement, {{IfState}, {WhileState}, {RetState}, {AssignState}});
            set_patterns(AssignState, {{ID, Equal, Expr}});
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
    }
};

}