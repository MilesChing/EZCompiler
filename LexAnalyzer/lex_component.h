#pragma once

#include <string>
using namespace std;

namespace EZCompiler{

enum LexComponent{
    LexComponentNull,
    LexComponentInteger,
    LexComponentString,
    LexComponentBoolean,
    LexComponentFloating,
    LexComponentCharacter,
    LexComponentComment,
    LexComponentMacro,
    LexComponentWhiteSpace,
    LexComponentEndLine,
    //brackets
    LexComponentLeftBrace,
    LexComponentRightBrace,
    LexComponentLeftSquareBracket,
    LexComponentRightSquareBracket,
    LexComponentLeftBracket,
    LexComponentRightBracket,
    //operators
    LexComponentPlus,
    LexComponentMinus,
    LexComponentMultiply,
    LexComponentDivide,
    LexComponentModulus,
    LexComponentIncrement,
    LexComponentDecrement,
    LexComponentEqual,
    LexComponentNotEqual,
    LexComponentGreater,
    LexComponentLess,
    LexComponentGE,
    LexComponentLE,
    LexComponentLogicalAnd,
    LexComponentLogicalOr,
    LexComponentLogicalNot,
    LexComponentBinaryAnd,
    LexComponentBinaryOr,
    LexComponentBinaryNot,
    LexComponentXor,
    LexComponentLeftShift,
    LexComponentRightShift,
    LexComponentAssignment,
    LexComponentAddAssignment,
    LexComponentSubtractAssignment,
    LexComponentMultiplyAssignment,
    LexComponentDivideAssignment,
    LexComponentModulusAssignment,
    LexComponentLeftShiftAssignment,
    LexComponentRightShiftAssignment,
    LexComponentBinaryAndAssignment,
    LexComponentBinaryOrAssignment,
    LexComponentBinaryXorAssignment,
    LexComponentComma,
    LexComponentSemicolon,
    //keywords
    LexComponentAuto,
    LexComponentBreak,
    LexComponentCase,
    LexComponentChar,
    LexComponentConst,
    LexComponentContinue,
    LexComponentDefault,
    LexComponentDo,
    LexComponentDouble,
    LexComponentElse,
    LexComponentEnum,
    LexComponentExtern,
    LexComponentFloat,
    LexComponentFor,
    LexComponentGoto,
    LexComponentIf,
    LexComponentInt,
    LexComponentLong,
    LexComponentRegister,
    LexComponentReturn,
    LexComponentShort,
    LexComponentSigned,
    LexComponentSizeof,
    LexComponentStatic,
    LexComponentStruct,
    LexComponentSwitch,
    LexComponentTypedef,
    LexComponentUnion,
    LexComponentUnsigned,
    LexComponentVoid,
    LexComponentVolatile,
    LexComponentWhile,

    //words
    LexComponentWord
};

extern const int LexComponentNum;

bool is_keyword(LexComponent component);

bool is_operator(LexComponent component);

bool is_bracket(LexComponent component);

bool is_value(LexComponent component);

string to_string(LexComponent component);

}