#include "additionals.h"


bool is_wildcard(std::string path){
    bool opened_bracket = false;
    for (auto l: path){
        if ((l == '*') || (l == '?'))
            return true;
        if (l == '[')
            opened_bracket = true;
        if (opened_bracket && (l == ']'))
            return true;
    }
    return false;
}



void wildcard_matching(std::string wildcard, std::vector<std::string> &matched_objects){
    glob_t globbuf;
    glob(wildcard.c_str(), GLOB_DOOFFS, NULL, &globbuf);

    for (int i = 0; i < globbuf.gl_pathc; i++)
        matched_objects.emplace_back(globbuf.gl_pathv[i]);
}