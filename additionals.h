#ifndef MYSHELL_ADDITIONALS_H
#define MYSHELL_ADDITIONALS_H

#include <iostream>
#include <glob.h>
#include <string>
#include <vector>

int parse_arguments(char* buf, int &argc, char** &argv);


bool is_wildcard(std::string path);

void wildcard_matching(std::string wildcard, std::vector<std::string> &matched_objects);

std::vector<std::string> split_str_by(const std::string& str, const std::string& delimiter);

std::string strip_str_edges(const std::string& str, bool (*predicate) (char str));

bool is_file_executable(const std::string &file);


#endif //MYSHELL_ADDITIONALS_H
