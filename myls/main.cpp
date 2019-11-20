#include <iostream>
#include <map>

#include <boost/filesystem.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "my_config.h"
#include "my_arg_parser.h"
#include "additionals.h"
#include "comparartors.h"


void process_directory(myConfig &config, std::string &path, struct stat &dir_stat);

void print_object(myConfig &config, std::string &object_name, struct stat &object_stat){
    // Object is a directory
    if (S_ISDIR(object_stat.st_mode)){
        std::cout << "/";
    } else if (config.specify_type){
        if ((object_stat.st_mode & S_IEXEC) != 0){
            // Object is executable
            std::cout << "*";
        } else if(S_ISLNK(object_stat.st_mode)){
            // Object is symbolic link
            std::cout << "@";
        } else if(S_ISSOCK(object_stat.st_mode)) {
            // Object is socket
            std::cout << "=";
        } else if(S_ISFIFO(object_stat.st_mode)){
            std::cout << "|";
        } else if (!S_ISREG(object_stat.st_mode)){
            // Others
            std::cout << "?";
        }
    }
    // name
    std::cout << object_name << "\t";
    if (config.detailed_info){
        // size, date and time of last modification
        char date_string[20];
        char time_string[20];
        convert_time(static_cast<unsigned long>(object_stat.st_mtimespec.tv_sec), date_string, time_string);
        std::cout << object_stat.st_size << "\t" << date_string << " " << time_string << std::endl;
    }
}


void process_objects(myConfig &config, std::vector<std::string> &objects, bool withoutParentDirectory){
    std::vector<myStat> vector_object_info;
    for (auto &object: objects){
        if (boost::filesystem::exists(object)){
            struct stat object_stat;
            if (stat(object.c_str(), &object_stat) == 0){
                myStat newMyStat;
                newMyStat.filename = object;
                newMyStat.fileStat = object_stat;
                vector_object_info.emplace_back(newMyStat);
            } else{
                std::cerr << "Unable to get stat: " << object << std::endl;
                std::cout << errno << std::endl;
            }
        } else{
            std::cerr << "File does not exists: " << object << std::endl;
        }
    }

    for (auto &sortFun: config.sorting){
        std::sort(vector_object_info.begin(), vector_object_info.end(),
                [&sortFun](const myStat& a, const myStat& b) -> bool{return sortFun(a, b);});
    }

    if (config.reversed_order){
        std::reverse(std::begin(vector_object_info), std::end(vector_object_info));
    }

    for (auto &ob: vector_object_info){
        if ((S_ISDIR(ob.fileStat.st_mode)) && config.iter_recursively){
            process_directory(config, ob.filename, ob.fileStat);
        } else{
            if (withoutParentDirectory){
                boost::filesystem::path p(ob.filename);
                ob.filename = p.filename().string();
            }
            print_object(config, ob.filename, ob.fileStat);
        }
    }
}


void process_directory(myConfig &config, std::string &path, struct stat &dir_stat){
    if (!config.detailed_info){
        std::cout << " " << std::endl;
    }
    std::vector<std::string> contents;
    directory_contents(path, contents);
    print_object(config, path, dir_stat);
    std::cout << ":";
    if (!config.detailed_info){
        std::cout << " " << std::endl;
    }
    process_objects(config, contents, true);
}


void process_wildcard(myConfig &config, const std::string &wildcard){
    std::vector<std::string> wildcards_objects;
    wildcard_matching(wildcard, wildcards_objects);
    std::cout << wildcard << std::endl;
    if (!wildcards_objects.empty())
        process_objects(config, wildcards_objects, true);
    else
        std::cerr << "Unable to get matching for this wildcard: " << wildcard <<std::endl;
}


int main(int argc, char** argv){
    myConfig config;
    if (parse_arguments(argc, argv, config) != 0){
        std::cerr << "Unable to parse arguments properly." << std::endl;
        return -1;
    }

    if (config.help){
        help_option();
        return 0;
    }

    if (config.objects.empty()){
        config.objects.emplace_back(".");
    }

    for (auto &object: config.objects){
        if (is_wildcard(object)){
            process_wildcard(config, object);
        } else {
            if (boost::filesystem::exists(object)) {
                struct stat object_stat;
                if (stat(object.c_str(), &object_stat) == 0) {
                    if (boost::filesystem::is_directory(object)) {
                        process_directory(config, object, object_stat);
                    } else {
                        print_object(config, object, object_stat);
                    }
                } else {
                    std::cerr << "Unable to get stat of: " << object << std::endl;
                }
            } else {
                std::cerr << "File does not exist: " << object << std::endl;
            }
        }
    }
    return 0;
}