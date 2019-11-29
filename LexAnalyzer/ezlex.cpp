#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include "lex_analyzer.h"
#include "Highlighter/highlighter.h"
using namespace std;
using namespace EZCompiler;

string src;
bool show_highlights = false;
fstream output_file;
LexAnalyzer analyzer;

void usage(){
	cout << "ezlex -x [file_path]: Analyze the file." << endl;
	cout << "ezlex -h -x [file_path]: Analyze the file and show highlights." << endl;
	cout << "ezlex -o [output_path] -x [file_path]: Analyze the file and output to file." << endl;
}

string read(fstream& f){
    stringstream ssr;
    ssr << f.rdbuf();
    return ssr.str();
}

void solve_params(int argc, char** argv){
	if(argc == 1){
		usage();
		exit(0);
	}

	int readx = 0, reado = 0;

	for(int i = 1; i < argc; ++i){
		if(readx == 1){
			fstream f(argv[i], ios::in);
			src = read(f);
			readx = 2;
		}
		else if(reado == 1){
			output_file = fstream(argv[i], ios::out);
			reado = 2;
		}
		else if(!strcmp(argv[i], "-x")){
			readx = 1;
		}
		else if(!strcmp(argv[i], "-h")){
			show_highlights = true;
		}
		else if(!strcmp(argv[i], "-o")){
			reado = 1;
		}
		else{
			usage();
			exit(0);
		}
	}

	if(readx != 2){
		usage();
		exit(0);
	}
}

int main(int argc, char** argv){
	solve_params(argc, argv);
	cout << "Starting analysis" << endl;
	analyzer.Analyze(src.c_str());
	LexAnalyzeResult res;
	analyzer.GetResult(res);
	if(output_file.is_open()){
		for(auto& seg : res){
			output_file << seg.left_index << " " << seg.right_index << " " << (int)(seg.component_type) << endl;
		}
		output_file.close();
	}
	else {
		for(auto& seg : res){
			cout << "[" << seg.left_index << " - " << seg.right_index << "] => " << to_string(seg.component_type) << endl;
		}
	}
	if(show_highlights){
		Highlighter h(src.c_str(), res);
		h.Output();
	}
	return 0;
}
