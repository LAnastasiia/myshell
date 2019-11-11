#ifndef MYSHELL_ADDITIONALS_H
#define MYSHELL_ADDITIONALS_H

#include <iostream>
#include <glob.h>
#include <string>
#include <vector>

bool is_wildcard(std::string path);

void wildcard_matching(std::string wildcard, std::vector<std::string> &matched_objects);

#endif //MYSHELL_ADDITIONALS_H
