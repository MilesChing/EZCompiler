#include "lex_component.h"

namespace EZCompiler{

const int LexComponentNum = LexComponentWord;

const char* component_names[] = {
    "Null",
    "Integer",
    "String",
    "Boolean",
    "Floating",
    "Character",
    "Comment",
    "Macro",
    "WhiteSpace",
    "EndLine",
    //operators
    "{",
    "}",
    "[",
    "]",
    "(",
    ")",
    "+",
    "-",
    "*",
    "/",
    "%",
    "++",
    "--",
    "==",
    "!=",
    ">",
    "<",
    ">=",
    "<=",
    "&&",
    "||",
    "!",
    "&",
    "|",
    "~",
    "^",
    "<<",
    ">>",
    "=",
    "+=",
    "-=",
    "*=",
    "/=",
    "%=",
    "<<=",
    ">>=",
    "&=",
    "|=",
    "^=",
    ",",
    ";",
    //keywords
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "int",
    "long",
    "register",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while",
    "Word"
};

string to_string(LexComponent component){
    return component_names[(int)component];
}

bool is_keyword(LexComponent component){
    return component >= 51 && component < 83;
}

bool is_operator(LexComponent component){
    return component >= 16 && component < 51;
}

bool is_bracket(LexComponent component){
    return component >= 10 && component < 16;
}

bool is_value(LexComponent component){
    return component >= 1 && component < 6;
}

}
