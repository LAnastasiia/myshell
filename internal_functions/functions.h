#include <cstdio>
#include <cstddef>
#include <string>


#ifndef MYSHELL_MYSHELL_FUNCTIONS_H
#define MYSHELL_MYSHELL_FUNCTIONS_H

// Output error number.
// supports [-h | --help]
extern int merrno();

// Output current working directory.
//[-h|--help]
extern int mpwd(std::string pwd_holder);

// Go to the given path.
//<path> [-h|--help]
extern int mcd(std::string path);

// Exit and update the status code.
// [finish code] [-h|--help]
extern int mexit();

// Output given var_name-s or their values.
// Accepts arbitrary num of var_name. If var_name starts with $ - output it's value lookup. Otherwise - var_name itself.
// [text|$<var_name>] [text|$<var_name>]  [text|$<var_name>] ...
extern int mecho();

// Add global env-variable by given var_name.
// If such variable already exists - overwrite it's value if also given (val).
// <var_name>[=VAL]
extern int mexport(const std::string& var_name, const std::string& val="");

#endif //MYSHELL_MYSHELL_FUNCTIONS_H
