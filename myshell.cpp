#include <iostream>
#include <cstring>
#include <sys/wait.h>

#include <boost/filesystem.hpp>
#include <readline/readline.h>
#include <readline/history.h>

#include "internal_functions/helper_functions.h"
#include "myShellConfig.h"

namespace fs = boost::filesystem;


int execute_file(const std::string& cmd_str, char ** margv, char **env){
    /* Implementation of '.' command - executing a pre-compiled source file. */
    std::string filename = cmd_str.substr(1);
    if (fs::exists(filename) && is_file_executable(filename)) {
        execve(filename.c_str(), margv, env);

        std::cout << "Failed to call execve on " << filename << " | Code: " << errno << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "No such file: " << filename << " | Code: " << errno << std::endl;
    return EXIT_FAILURE;
}


int execute_cmd(int margc, char* margv[], char* env[], myShellConfig& config){
    /* Execute a shell command (either internal or external).
     * Return the status of execution. */
    if (*margv[0] == '.'){
        execute_file(margv[0], margv, env);
    }
    std::string cmd = margv[0];

    // Assignment handling (VAR=VAL).
    if (cmd.find('=') != std::string::npos){
        int argc = 2;
        char** argv = (char**) malloc((argc + 1) * sizeof(char*));
        argv[0] = strdup("mexport");
        argv[1] = strdup(cmd.c_str());
        argv[argc] = nullptr;
        return config.internal_cmd_map.at("mexport")(argc, argv);
    }

    if (config.has_internal_cmd(cmd)){
        return config.internal_cmd_map.at(cmd)(margc, margv);
    }
    else {
       pid_t pid = fork();

       switch (pid) {
           case (-1): {
               std::cout << "Failed to fork()" << std::endl;
               return EXIT_FAILURE;
           }
           case (0): {
               if (config.has_external_cmd(cmd)){
                   execve((config.bin_path + '/' + cmd).c_str(), margv, env);
               } else {
                   execvpe(cmd.c_str(), margv, env);
               }
                // Next code is only reachable iff exec failed.
               std::cout << "Failed to execute " << cmd << ". | Code: " << errno << std::endl;
               return EXIT_FAILURE;
           }
           default:
               int child_status;
               waitpid(pid, &child_status, 0);
               if (child_status == EXIT_FAILURE)
                   std::cout << "Command failed: " << cmd << " Exit code: " << child_status << std::endl;
       }
    }
}


int main(int argc, char** argv, char* env[]) {

    myShellConfig config;
    std::string path{argv[0]};
    path = path.substr(0, path.find_last_of('/'));

    if ( config.set_path_to_external_cmd_modules(path) == EXIT_FAILURE ){
        std::cout << "Unable to get initial pwd: " << path << " is not a valid directory. " << std::endl;
        return EXIT_FAILURE;
    }

    int cmd_status = 0;
    char *buf;
    int margc; char** margv;

    std::cout << boost::filesystem::current_path();

    while ((buf = readline("$")) != nullptr) {
        if (strlen(buf) == 0){
            std::cout << boost::filesystem::current_path(); continue;
        }

        add_history(buf);

        auto cmd_vec = split_str_by(std::string(buf), "|");  // Split into commands.

        // Execute a single command.
        if (cmd_vec.size() == 1) {
            auto cmd_str = strip_str_edges(cmd_vec[0], [](char s) { return s == ' ';});

            auto args = split_str_by(cmd_str, " ", "\"");  // Split into arguments.
            if (args.empty()) {
                std::cout << "Bad input. " << std::endl << boost::filesystem::current_path();
                continue;
            }
            for (size_t i = 0; i < args.size(); i++ ){
                if (is_wildcard(args[i])){
                    std::vector<std::string> matched_args;
                    if (wildcard_matching(args[i], matched_args) == EXIT_SUCCESS){
                        args.erase(args.begin()+i);
                        args.insert(args.end(), matched_args.begin(), matched_args.end());
                    } else {
                        std::cout << "Couldn't match the wildcard in " << args[i] << std::endl << boost::filesystem::current_path();
                        continue;
                    }
                }
                else if (is_envariable(args[i])){
                    auto envar = getenv(args[i].substr(1).c_str());
                    if (envar) {
                        args[i] = std::string {envar};
                    }
                }
            }

            if (parse_into_arguments(args, margc, margv) == EXIT_SUCCESS){
                cmd_status = execute_cmd(margc, margv, env, config);
                if (cmd_status == EXIT_FAILURE) {
                    std::cout << "Failed " << cmd_vec[0] << std::endl << boost::filesystem::current_path();
                    continue;
                }

            } else {
                std::cout << "Couldn't parse the arguments of " << cmd_vec[0] << std::endl;
                std::cout << boost::filesystem::current_path();
                continue;
            }
            free(margv); margc = 0;
        }
        std::cout << boost::filesystem::current_path();
    }
    return EXIT_SUCCESS;
}


// ToDo:
// (O) add merrno
// (-) replace strip_str_edges' args with <template> and pass lambda with captured strip char (delimiter or escape).
