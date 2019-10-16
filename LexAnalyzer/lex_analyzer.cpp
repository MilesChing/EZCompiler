#include "lex_analyzer.h"
#include "StringConverter/string_converter.h"
#include <iostream>

namespace EZCompiler{

#define nameof(x) (#x)
#define len(array) (sizeof(array) / sizeof(array[0]));

LexSegment::LexSegment(){}

LexSegment::LexSegment(int li, int ri, LexComponent ctype) :
    left_index(li), right_index(ri), component_type(ctype){}

LexAnalyzer::LexAnalyzer(){
    //add keywords & operators
    for(int i = 10; i < 83; ++i){
        LexComponent component = (LexComponent)i;
        string sign = to_string(component);
        trie.insert(sign, component);
    }
}

void LexAnalyzer::Analyze(const string& code){
    Analyze(code.c_str());
}

void LexAnalyzer::Analyze(const char* code){
    init();
    original_code = new char[code_length = strlen(code)];
    memcpy(original_code, code, code_length);
    component_type.clear();
    component_type.resize(strlen(code), LexComponent::LexComponentNull);
    preAnalyze();
    analyzeSpliters();
    analyzeKeywordsAndOperators();
    analyzeWords();
}

void LexAnalyzer::GetResult(LexAnalyzeResult& result){
    result = inner_result;
}

void LexAnalyzer::init(){
    if(original_code){
        delete original_code;
        original_code = NULL;
    }
    inner_result.clear();
}

void LexAnalyzer::preAnalyze(){
    bool is_single_line_comment = false;
    bool is_multiline_comment = false;
    bool is_string = false;
    bool is_char = false;
    bool is_macro = false;
    int start_index = 0;
    LexComponent tmp_component;
    for(int i = 0; ; ++i){
        const char cch = original_code[i];
        if(is_macro){
            if(!cch || cch == '\n'){
                setComponent(start_index, i, LexComponent::LexComponentMacro);
                is_macro = false;
            }
        }
        else if(is_single_line_comment){
            if(!cch || cch == '\n'){
                setComponent(start_index, i, LexComponent::LexComponentComment);
                is_single_line_comment = false;
            }
        }
        else if(is_multiline_comment){
            if(cch == '/' && original_code[i-1] == '*'){
                setComponent(start_index, i+1, LexComponent::LexComponentComment);
                is_multiline_comment = false;
            }
        }
        else if(is_string){
            if(cch == '\"'){
                setComponent(start_index, i+1, LexComponent::LexComponentString);
                is_string = false;
            }
        }
        else if(is_char){
            if(cch == '\''){
                setComponent(start_index, i+1, LexComponent::LexComponentCharacter);
                is_char = false;
            }
        }
        else if(cch == '*' && i != 0 && original_code[i-1] == '/'){
            is_multiline_comment = true;
            start_index = i-1;
        }
        else if(cch == '\"'){
            is_string = true;
            start_index = i;
        }
        else if(cch == '\''){
            is_char = true;
            start_index = i;
        }
        else if(cch == '/' && i != 0 && original_code[i-1] == '/'){
            is_single_line_comment = true;
            start_index = i-1;
        }
        else if(cch == '#'){
            is_macro = true;
            start_index = i;
        }
        if(!cch) return;
    }
}

void LexAnalyzer::analyzeSpliters(){
    int start_index = 0;
    bool is_spliter = false;
    for(int i = 0;; ++i){
        if(component_type[i] && !is_spliter) continue;
        char cch = original_code[i];
        if(cch == ' ' || cch == '\t'){
            if(!is_spliter){
                is_spliter = true;
                start_index = i;
            }
        }
        else{
            if(is_spliter){
                setComponent(start_index, i, LexComponent::LexComponentWhiteSpace);
                is_spliter = false;
            }
        }

        if(cch == '\n')
            setComponent(i, i+1, LexComponent::LexComponentEndLine);
        else if(!cch) break;
    }
}

void LexAnalyzer::analyzeKeywordsAndOperators(){
    string tmp_code = original_code;
    for(int i = 0; i < code_length; ++i)
        if(component_type[i])
            tmp_code[i] = ' ';
    auto res = trie.match(tmp_code);
    const char* head = tmp_code.c_str();
    for(auto& seg : res)
        if(is_operator(seg.val))
            setComponent(seg.begin - head, seg.end - head, seg.val);
    for(auto& seg : res)
        if(is_bracket(seg.val))
            setComponent(seg.begin - head, seg.end - head, seg.val);
    for(auto& seg : res)
        if(is_keyword(seg.val)){
            if((seg.begin - head == 0 || component_type[seg.begin - head - 1])
                && (seg.end - head == code_length || component_type[seg.end - head]))
                setComponent(seg.begin - head, seg.end - head, seg.val);
        }
}

void LexAnalyzer::analyzeWords(){
    int start_index = -1;
    string ori = original_code;
    for(int i = 0; ; ++i){
        if(start_index < 0 && (original_code[i] && !component_type[i]))
            start_index = i;
        else if(start_index >= 0 && (!original_code[i] || component_type[i])){
            string code = ori.substr(start_index, i - start_index);
            if(StringConverter::TryParse<unsigned long long>(code))
                setComponent(start_index, i, LexComponent::LexComponentInteger);
            else if(StringConverter::TryParse<double>(code))
                setComponent(start_index, i, LexComponent::LexComponentFloating);
            else if(code == "true" || code == "false")
                setComponent(start_index, i, LexComponent::LexComponentBoolean);
            else setComponent(start_index, i, LexComponentWord);
            start_index = -1;
        }
        if(!original_code[i]) break;
    }
}

void LexAnalyzer::setComponent(int start_index, int end_index, LexComponent component){
    inner_result.emplace_back(start_index, end_index, component);
    for(int i = start_index; i < end_index; ++i)
        component_type[i] = component;
}

}
