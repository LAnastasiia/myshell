#ifndef MYSHELL_HELPER_FUNCTIONS_H
#define MYSHELL_HELPER_FUNCTIONS_H

#include <iostream>
#include <glob.h>
#include <string.h>
#include <vector>
#include <algorithm>

#include <boost/concept_check.hpp>


int parse_into_arguments(std::vector<std::string>& arguments, int &argc, char** &argv);

bool is_wildcard(const std::string& path);

bool is_envariable(const std::string& arg);

int wildcard_matching(const std::string& wildcard, std::vector<std::string>& matched_objects);

std::vector<std::string> split_str_by(const std::string& str, const std::string& delimiter);
std::vector<std::string> split_str_by(const std::string& str, const std::string& delimiter, const std::string& escape);

std::string strip_str_edges(const std::string& str, bool (*predicate) (char str));

bool is_file_executable(const std::string &file);


#endif //MYSHELL_HELPER_FUNCTIONS_H
