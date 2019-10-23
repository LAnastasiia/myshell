#include <cstdio>
#include <cstddef>
#include <string>


#ifndef MYSHELL_MYSHELL_FUNCTIONS_H
#define MYSHELL_MYSHELL_FUNCTIONS_H

//[-h | --help]
extern int merrno();

//[-h|--help]
extern int mpwd(std::string pwd_holder);

//<path> [-h|--help]
extern int mcd(std::string path);

// [код завершення] [-h|--help]
extern int mexit();

// [text|$<var_name>] [text|$<var_name>]  [text|$<var_name>] ...
extern int mecho();

// <var_name>[=VAL]
extern int mexport(const std::string& var_name, const std::string& val="");

#endif //MYSHELL_MYSHELL_FUNCTIONS_H
