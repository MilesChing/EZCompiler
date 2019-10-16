#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "lex_analyzer.h"
#include "Highlighter/highlighter.h"
using namespace std;
using namespace EZCompiler;

char* buffer = NULL;
bool show_highlights = false;
bool output_details = false;
LexAnalyzer analyzer;

void usage(){
	cout << "ezlex -x [file_path]: Analyze the file." << endl;
	cout << "ezlex -h -x [file_path]: Analyze the file and show highlights." << endl;
	cout << "ezlex -o -x [file_path]: Analyze the file and output details." << endl;
}

void solve_params(int argc, char** argv){
	if(argc == 1){
		usage();
		exit(0);
	}
	for(int i = 1; i < argc; ++i){
		if(!strcmp(argv[i], "-x")){
			if(i+1 < argc){
				fstream f(argv[i+1]);
				cout << "Open file: " << argv[i+1] << endl;
				if(!f.is_open()){
					cout << "File not exist." << endl;
					exit(0);
				}
				f.seekg(0,std::ios::end);
				int len = f.tellg();
				f.seekg(0,std::ios::beg);
				buffer = new char[len + 1];
				memset(buffer, 0, len+1);
				f.read(buffer, len);
				return;
			}
			else{
				usage();
				exit(0);
			}
		}
		else if(!strcmp(argv[i], "-h")){
			show_highlights = true;
		}
		else if(!strcmp(argv[i], "-o")){
			output_details = true;
		}
		else{
			usage();
			exit(0);
		}
	}
}

int main(int argc, char** argv){
	solve_params(argc, argv);
	cout << "Starting analysis" << endl;
	analyzer.Analyze(buffer);
	LexAnalyzeResult res;
	analyzer.GetResult(res);
	if(output_details){
		for(auto& seg : res){
			cout << "[" << seg.left_index << " - " << seg.right_index << "] => " << to_string(seg.component_type) << endl;
		}
	}
	if(show_highlights){
		Highlighter h(buffer, res);
		h.Output();
	}
	if(buffer) delete buffer;
	return 0;
}
