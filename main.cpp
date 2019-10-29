#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <readline/readline.h>
#include <readline/history.h>

#include "internal_functions/functions.h"


namespace fs = boost::filesystem;


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
//    free(argv);
    argv = (char**) malloc((argc + 1) * sizeof(char*));

    if (argv == nullptr){
        std::cerr << "Unable to allocate array for arguments" << std::endl;
        return EXIT_FAILURE;
    }

    for (i = 0; i < argc; i++){
        argv[i] = (char*) arguments[i].c_str();
    }
    argv[argc] = NULL;
    return EXIT_SUCCESS;
}


int main(int argc, char** argv, char* envp[]) {
    int status = 0;

    // Assume every function returns int (status).
    // In case we need another return-type - pass a pointer to some variable of that type as an argument, and then write results into it.

    std::map <std::string, std::function<int (int argc, char* argv[])>> commands_map;
    commands_map.emplace(
            std::make_pair("mexport", [](int argc, char** argv)-> int {return mexport(argc, argv);} ));
    commands_map.emplace(
            std::make_pair("mpwd", [](int argc, char** argv)-> int {return mpwd(argc, argv);} ));
    commands_map.emplace(
            std::make_pair("mcd", [](int argc, char** argv)-> int {return mcd(argc, argv);} ));


    char* buf;
    // MAIN LOOP
    std::string command = " ";
    std::cout << boost::filesystem::current_path();
    while ((buf = readline(" $ ")) != nullptr) {
        if (strlen(buf) > 0) {
            add_history(buf);
        }

        if (parse_arguments(buf, argc, argv) != EXIT_SUCCESS){
            continue;
        }

        command = argv[0];
//        std::cout << command << std::endl;

         // Action.
         if (commands_map.find(command) != commands_map.end()){
             status = commands_map[command](argc, argv);
             if (command == "mexit"){
                 break;
             }
         } else {
            // Run external command
         }

        std::cout << boost::filesystem::current_path();
    }
    return 0;
}
