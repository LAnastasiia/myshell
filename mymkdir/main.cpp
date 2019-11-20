#include <iostream>
#include <vector>

#include <boost/filesystem.hpp>

#include "my_args_parser.h"
#include "my_config.h"


int main(int argc, char* argv[]) {
    myConfig config;
    parse_arguments(argc, argv, config);

    if (config.help){
        help_option();
        return 0;
    }

    if (boost::filesystem::exists(config.dirname)){
        std::cerr << "Entered directory already exists." << std::endl;
        return -1;
    }
    std::vector<boost::filesystem::path> to_create;
    boost::filesystem::path needed_path = boost::filesystem::path(config.dirname);
    boost::filesystem::path current_path = boost::filesystem::path(".");

    for (auto& part: needed_path.parent_path()){
        current_path = boost::filesystem::path(current_path.string() + boost::filesystem::path::preferred_separator +  part.string());
        if (!boost::filesystem::exists(current_path)){
            if (config.p){
                to_create.push_back(current_path);
            } else {
                std::cerr << "Transitional directory does not exists: " << part << std::endl;
                return -2;
            }
        }
    }

    to_create.push_back(needed_path);

    for (auto& dir: to_create){
        boost::filesystem::create_directory(dir);
    }
    return 0;
}