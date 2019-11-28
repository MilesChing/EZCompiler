#pragma once
#include <cstring>
#include <vector>
#include "lex_component.h"
#include "TrieNAhoCorasick/ChisTrie.h"
using namespace std;
using namespace chis;

namespace EZCompiler{

class LexSegment{
    public:
        LexSegment();
        LexSegment(int left_index, int right_index, LexComponent component_type);
        int left_index = -1, right_index = -1;
        LexComponent component_type = LexComponent::LexComponentNull;
};

typedef vector<LexSegment> LexAnalyzeResult;

class LexAnalyzer{
    public:
        LexAnalyzer();
        void Analyze(const char* code);
        void Analyze(const string& code);
        void GetResult(LexAnalyzeResult& result);
    private:
        char* original_code = NULL;
        int code_length = -1;
        LexAnalyzeResult inner_result;
        vector<LexComponent> component_type;
        chis::lctrie<LexComponent> trie;
        void init();
        void preAnalyze();
        void analyzeSpliters();
        void analyzeKeywordsAndOperators();
        void analyzeWords();
        void setComponent(int start_index, int end_index, LexComponent component);
};

}
