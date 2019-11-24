#include <algorithm>
#include <boost/concept_check.hpp>
#include "additionals.h"


int parse_arguments(char* buf, int &argc, char** &argv){
    std::vector<std::string> arguments;
    std::vector<char> word;
    bool in_quotes = false;
    int i = 0;
    while (buf[i] != '\0'){
        if ((buf[i] == '"') || (isspace(buf[i]) && (!in_quotes))){
            if (buf[i] == '"'){
                in_quotes = !in_quotes;
            }
            arguments.emplace_back((word.begin()), word.end());
            word.clear();
        } else{
            word.push_back(buf[i]);
        }
        i++;
    }
    if (!word.empty()){
        arguments.emplace_back((word.begin()), word.end());
    }

    argc = static_cast<int>(arguments.size());
    argv = (char**) malloc((argc + 1) * sizeof(char*));

    if (argv == nullptr){
        std::cerr << "Unable to allocate array for arguments" << std::endl;
        return EXIT_FAILURE;
    }

    for (i = 0; i < argc; i++){
        argv[i] = (char*) arguments[i].c_str();
    }
    argv[argc] = nullptr;
    return EXIT_SUCCESS;
}


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


std::string strip_str_edges(const std::string& str, bool (*predicate) (char str)){
    auto new_begin = std::find_if_not(str.begin(), str.end(), predicate);
    auto new_end = std::find_if_not(str.rbegin(), str.rend(), predicate);

    return std::string(new_begin, new_end.base());
}


std::vector<std::string> split_str_by(const std::string& str, const std::string& delimiter){
    size_t split_index = 0, new_split_index = 0;

    std::vector<std::string> elements;
    while ((new_split_index = str.find(delimiter, split_index)) && new_split_index != std::string::npos) {
        elements.emplace_back(
                strip_str_edges(
                        str.substr(split_index, new_split_index-split_index),
                        [](char s) { return s == ' ';})
                        );
        split_index = new_split_index + delimiter.length();
    }
    elements.emplace_back(
            strip_str_edges(
                    str.substr(split_index),
                    [](char s) { return s == ' ';})
                    );
    return elements;
}

bool is_file_executable(const std::string &file){
    return ! access (file.c_str(), X_OK);
}