#include "syntax_analyzer.h"
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include "Highlighter/colors.h"
using namespace std;
using namespace EZCompiler;

void usage(){
    cout << "ezsyntax -f [lex_file] -x [input_src]" << endl;
}

string read(fstream& f){
    stringstream ssr;
    ssr << f.rdbuf();
    return ssr.str();
}

string src;
string lex;

bool analyze(int argc, char** argv){
    int readf = 0, readx = 0;
    for(int i = 1; i < argc; ++i){
        if(readf == 1){
            fstream f(argv[i], ios::in);
            if(!f.is_open()){
                cerr << "File \'" << argv[i] << "\' not exist." << endl;
                return false;
            }
            lex = read(f);
            f.close();
            readf = 2;
        }
        else if(readx == 1){
            fstream f(argv[i], ios::in);
            if(!f.is_open()){
                cerr << "File \'" << argv[i] << "\' not exist." << endl;
                return false;
            }
            src = read(f);
            f.close();
            readx = 2;
        }
        else if(strcmp(argv[i], "-f") == 0){
            readf = 1;
        }
        else if(strcmp(argv[i], "-x") == 0){
            readx = 1;
        }
    }

    if(readf != 2 || readx != 2)
        return false;

    return true;
}

bool complex(const LexSegment& a, const LexSegment& b){
    return a.left_index < b.right_index;
}

void highlight_line(const LexSegment& seg, int bcolor){
    int start = 0, end = src.size();
    for(int i = seg.left_index - 1; i >= 0; --i)
        if(src[i] == '\n'){ start = i + 1; break; }
    for(int i = seg.right_index; i < src.size(); ++i)
        if(src[i] == '\n'){ end = i; break; }
    resetBColor();
    resetFColor();
    for(int i = start; i < end; ++i){
        if(i >= seg.left_index && i < seg.right_index) setColor(F_WHITE | bcolor);
        else setColor(F_WHITE | B_BLACK);
        cout << src[i];
    }
    resetBColor();
    resetFColor();
    cout << endl;
}

int main(int argc, char** argv){
    if(!analyze(argc, argv)){
        usage();
        return -1;
    }

    stringstream ssr(lex);
    vector<LexSegment> segs;
    LexSegment lex_seg(-1, -1, LexComponent::LexComponentNull);
    int comp;
    while(ssr >> lex_seg.left_index >> lex_seg.right_index >> comp){
        lex_seg.component_type = (LexComponent)comp;
        if(lex_seg.component_type != LexComponent::LexComponentWhiteSpace &&
            lex_seg.component_type != LexComponent::LexComponentEndLine)
            segs.push_back(lex_seg);
    }

    sort(segs.begin(), segs.end(), &complex);

    CLikeSyntaxAnalyzer top;
    for(auto& seg : segs){
        vector<string> res;
        try{ res = top.GetStep(seg.component_type); }
        catch(const string& mes){
            highlight_line(seg, B_RED);
            return -1;
        }
        int cptr = top.CurrentPointer();
        highlight_line(seg, B_BLUE);
        resetBColor();
        resetFColor();
        cout << "[ ";
        for(int i = 0; i < res.size(); ++i){
            if(i < cptr - 1) setColor(F_GREEN | B_BLACK);
            else if(i == cptr - 1) setColor(F_BLACK | B_GREEN);
            else setColor(F_WHITE | B_BLACK);
            cout << res[i];
            resetBColor();
            resetFColor();
            cout << " ";
        }
        cout << "]" << endl;
    }

    return 0;
}