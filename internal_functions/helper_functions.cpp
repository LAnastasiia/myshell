#include <map>
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


int wildcard_matching(const std::string& wildcard, std::vector<std::string>& matched_objects){
    glob_t glob_buf;
    memset(&glob_buf, 0, sizeof(glob_buf));

    int glob_status = glob(wildcard.c_str(), GLOB_TILDE, nullptr, &glob_buf);
    if (glob_status == EXIT_SUCCESS) {
        for (size_t i = 0; i < glob_buf.gl_pathc; i++){
            matched_objects.emplace_back(glob_buf.gl_pathv[i]);
        }
        globfree(&glob_buf);
        return EXIT_SUCCESS;

    } else {
        switch (glob_status){
            case GLOB_NOMATCH:
                std::cerr << "No match was found for " << wildcard << " wildcard" << std::endl;
            case GLOB_NOSPACE:
                std::cerr << "Out of memory error in wildcard matching" << std::endl;
            case GLOB_ABORTED:
                std::cerr << "Read error in wildcard matching" << std::endl;
            default:
                std::cerr << "Unhandled error in wildcard matching" << std::endl;
        }
        globfree(&glob_buf);
        return EXIT_FAILURE;
    }
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
    size_t split_index = 0, new_split_index = 0; bool escape_sequence = false;
    std::vector<std::string> elements;

    while ((new_split_index >= split_index) && new_split_index != std::string::npos) {
        if (new_split_index - split_index > 1) {
            elements.emplace_back(strip_str_edges(
                    str.substr(split_index, new_split_index - split_index),
                    [](char s) { return s == ' ' || s == '"'; }));
        }

        split_index = new_split_index;
        escape_sequence = (str.substr(split_index, escape.length()) == escape) && (! escape_sequence);

        if (escape_sequence && (str.find(escape, split_index+1) == std::string::npos)) { return {}; }
        // Fail split if the escape sequence doesn't close.

        new_split_index = (escape_sequence) ?
                          std::max(
                                  (str.find(delimiter, split_index+1) != std::string::npos) * str.find(delimiter, split_index+1),
                                  (str.find(escape, split_index+1) != std::string::npos) * str.find(escape, split_index+1))
                          :
                          std::min(str.find(delimiter, split_index+1), str.find(escape, split_index+1));
    }
    // Add the leftover elements
    if (split_index < str.length() - 1){
        elements.emplace_back(strip_str_edges(
                str.substr(split_index),
                [](char s) { return s == ' ' || s == '"'; }));
    }
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

bool is_envariable(const std::string& arg){
    return arg[0] == '$';
}

bool is_redirect(const std::string& arg){
    for (const auto& l : arg) {
        if (l == '<' || l == '>'){
            return true;
        }
    }
    return false;
}

int parse_redirect(std::vector<std::string>& args, int i, std::map<int,std::string>& cmd_streams){
    for (size_t j = 0; j < args[i].length(); j++){

        if (args[i][j] == '>') {
            // xx > xx
            if (args[i].length() == 1 && (0 < i && i < args.size()-1)) {
                char prev_el = args[i-1][ args[i].length()-1 ];
                if (isdigit(prev_el)){
                    if (prev_el == '1' || prev_el == '2'){
                        cmd_streams[(int) prev_el] = args[i+1];
                    } else return EXIT_FAILURE;
                } else { cmd_streams[1] = args[i+1]; }

            // xx >xx
            } else if (j == 0 && (args[i].length() > 1 && i > 0)) {
                    char prev_el = args[i-1][ args[i].length()-1 ];
                    if (isdigit(prev_el)){
                        if (prev_el == '1' || prev_el == '2'){
                            cmd_streams[(int) prev_el] = std::string {&args[i][j+1]};
                        } else return EXIT_FAILURE;
                    } else { cmd_streams[1] = std::string {&args[i][j+1]}; }

            // xx> xx
            } else if (j == args[i].length()-1 && (i < args.size()-1 && args[i].length() > 1)) {
                char prev_el = args[i][ j-1 ];
                if (isdigit(prev_el)){
                    if (prev_el == '1' || prev_el == '2'){
                        cmd_streams[(int) prev_el] = args[i+1];
                    } else return EXIT_FAILURE;
                } else { cmd_streams[1] = args[i+1]; }

            } else {
                return EXIT_FAILURE;
            }
        } else if (args[i][j] == '<') {
            // xx < xx
            if (args[i].length() == 1 && (0 < i && i < args.size()-1)) {
                cmd_streams[0] = args[i+1];
            // xx <xx
            } else if (j == 0 && (args[i].length() > 1 && i > 0)) {
                cmd_streams[0] = std::string {& args[i][j+1]};
            // xx< xx
            } else if (j > 0 && (args[i].length() > 1 && i < args.size()-1)){
                cmd_streams[0] = args[i+1];
            } else { return EXIT_FAILURE; }
        }
    }
    return EXIT_SUCCESS;
}
