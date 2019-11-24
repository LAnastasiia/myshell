#ifndef MYSHELL_MYSHELL_FUNCTIONS_H
#define MYSHELL_MYSHELL_FUNCTIONS_H

#include <iostream>
#include <cstdio>
#include <cstddef>
#include <string>
#include <boost/program_options.hpp>
#include <sstream>


int help_option_enabled(int argc, char** argv);

// Output error number.
// supports [-h | --help]
int merrno(int argc, char** argv, int &my_errno);

// Output current working directory.
//[-h|--help]
int mpwd(int argc, char** argv, int fd);

// Go to the given path.
//<path> [-h|--help]
int mcd(int argc, char** argv);

// Exit and update the status code.
// [finish code] [-h|--help]
int mexit(int argc, char** argv);

// Output given var_name-s or their values.
// Accepts arbitrary num of var_name. If var_name starts with $ - output it's value lookup. Otherwise - var_name itself.
// [text|$<var_name>] [text|$<var_name>]  [text|$<var_name>] ...
int mecho(int argc, char** argv);

// Add global env-variable by given var_name.
// If such variable already exists - overwrite it's value if also given (val).
// <var_name>[=VAL]
int mexport(int argc, char** argv);

#endif //MYSHELL_MYSHELL_FUNCTIONS_H
