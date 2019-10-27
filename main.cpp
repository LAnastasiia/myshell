#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <hash_map>

#include "internal_functions/functions.h"

namespace fs = boost::filesystem;

int main(int argc, char *argv[], char* envp[]) {

    // SET-UP
    std::map<std::string, std::string> descriptions_map;
    descriptions_map.emplace(std::make_pair("merrno", "..."));
    descriptions_map.emplace(std::make_pair("mpwd", "..."));
    descriptions_map.emplace(std::make_pair("mcd", "..."));
    descriptions_map.emplace(std::make_pair("mexit", "..."));
    descriptions_map.emplace(std::make_pair("mecho", "..."));
    descriptions_map.emplace(std::make_pair("mexport", "..."));

    // Assume every function returns int (status).
    // In case we need another return-type - pass a pointer to some variable of that type as an argument, and then write results into it.

    std::map <std::string, std::function<int (int argc, char* argv[])>> commands_map;
    commands_map.emplace(
            std::make_pair("mexport", [](int argc, char* argv[])-> int {return mexport(argc, argv);} ));


    // MAIN LOOP
    std::string command = " ";
    while (command != "exit") {
        std::cout << boost::filesystem::current_path() << std::endl;
        std::cin >> command;

        // parse options

        if (command == "mexport") {
            // Help.
            if (descriptions_map.find(command) != descriptions_map.end())
                std::cout << descriptions_map["mexport"] << std::endl;

            // Action.
            if (commands_map.find(command) != commands_map.end()){

                // test
                char *args[] = {
                        (char *) "some_var",
                        (char *) "some_val",
                        nullptr
                };
                // test
                std::cout << std::to_string(commands_map["mexport"](2, args)) << std::endl;
            }

        } else {
            // Run external command
        }
    }
    return 0;
}
