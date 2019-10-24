#include "functions.h"


//extern int merrno(){}


//extern int mpwd(std::string pwd_holder){}


//extern int mcd(std::string path){}


//extern int mexit(){}


//extern int mecho(){}


extern int mexport(const std::string& var_name, const std::string& val){
    if (! val.empty()){
        return setenv(var_name.c_str(), val.c_str(), 1);

    } else {
        // Make a non-const pointer from var_name - as required by putenv.
        char * var_name_ptr = new char[var_name.size() + 1];
        std::copy(var_name.begin(), var_name.end(), var_name_ptr);
        var_name_ptr[var_name.size()] = '\0';

        return putenv(var_name_ptr);
    }
}