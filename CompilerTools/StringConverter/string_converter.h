#pragma once

#include <sstream>

using namespace std;

namespace EZCompiler{

class StringConverter{
public:
    template<typename T> static bool TryParse(const string& str, T* val_ptr = NULL){
        stringstream ori(str);
        T val;
        if(!val_ptr){
            val_ptr = &val;
        }
        if(ori >> *val_ptr) return true;
        else return false;
    }
};

}