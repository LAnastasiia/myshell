#ifndef MYLS_ADDITIONALS_H
#define MYLS_ADDITIONALS_H

#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <sys/stat.h>

#include <glob.h>

#include "boost/filesystem.hpp"

struct myStat{
    std::string filename;
    struct stat fileStat;
};

void convert_time(unsigned long seconds, char *date_string, char *time_string);

int directory_contents(std::string &path, std::vector<std::string> &contents);

bool is_wildcard(std::string path);

void wildcard_matching(std::string wildcard, std::vector<std::string> &matched_objects);

#endif //MYLS_ADDITIONALS_H
