#include "helper_functions.h"


bool is_wildcard(const std::string& path){
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


void wildcard_matching(const std::string& wildcard, std::vector<std::string> &matched_objects){
    glob_t globbuf;
    glob(wildcard.c_str(), GLOB_DOOFFS, NULL, &globbuf);

    for (size_t i = 0; i < globbuf.gl_pathc; i++)
        matched_objects.emplace_back(globbuf.gl_pathv[i]);

    globfree(&globbuf);
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


std::vector<std::string> split_str_by(const std::string& str, const std::string& delimiter, const std::string& escape){
    size_t split_index = 0, new_split_index = 0;
    std::vector<std::string> elements;
    bool escape_sequence = false;

    while (new_split_index != std::string::npos) {
        if (new_split_index > split_index) {
            elements.emplace_back(strip_str_edges(
                    str.substr(split_index, new_split_index - split_index),
                    [](char s) { return s == ' ' || s == '"'; })
            );
            split_index = (escape_sequence) ? new_split_index + delimiter.length() : new_split_index + escape.length();
        }
        escape_sequence = (str.substr(new_split_index, escape.length()) == escape) && (! escape_sequence);
        new_split_index = (escape_sequence) ?
                          std::max(str.find(delimiter, split_index), str.find(escape, split_index))
                          :
                          std::min(str.find(delimiter, split_index), str.find(escape, split_index));
    }
    // Add the leftover elements
    elements.emplace_back(strip_str_edges(str.substr(split_index), [](char s) { return s == ' ';}));
    return elements;
}


int parse_into_arguments(std::vector<std::string>& arguments, int& argc, char**& argv){
    argc = static_cast<int>(arguments.size());
    argv = (char**) malloc((argc + 1) * sizeof(char*));

    if (argv == nullptr){
        std::cerr << "Unable to allocate array for arguments" << std::endl;
        return EXIT_FAILURE;
    }
    for (int i = 0; i < argc; i++){
        argv[i] = (char*) arguments[i].data();
    }
    argv[argc] = nullptr;
    return EXIT_SUCCESS;
}


bool is_file_executable(const std::string &file){
    return ! access (file.c_str(), X_OK);
}