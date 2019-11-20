#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <cctype>
#include <iostream>
#include <cstring>

#include <boost/filesystem.hpp>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>

#include "internal_functions/helper_functions.h"
#include "myShellConfig.h"

namespace fs = boost::filesystem;


struct cmdConfig {
    bool background_execution = false;
    std::map<int, std::string> redirect_map = {};
};


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


int execute_cmd(int margc, char* margv[], char* env[], myShellConfig& config, cmdConfig cmd_config){

    // Executable.
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

    // Internal.
    if (config.has_internal_cmd(cmd)){
        return config.internal_cmd_map.at(cmd)(margc, margv);
    }
        // External.
    else {
        pid_t pid = fork();
        switch (pid) {
            case (-1): {
                std::cout << "Failed to fork()" << std::endl;
                return EXIT_FAILURE;
            }
            case (0): {
                for (const auto& p : cmd_config.redirect_map){
                    int fd = open(p.second.c_str(), (p.first == 0) ? O_RDONLY : O_WRONLY); dup2(fd, p.first);
                    if (fd == -1) return EXIT_FAILURE;
                }

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
                if (! cmd_config.background_execution){
                    waitpid(pid, &child_status, 0);
                    if (child_status == EXIT_FAILURE)
                        std::cout << "Command failed: " << cmd << " Exit code: " << child_status << std::endl;
                }
                return EXIT_SUCCESS;
        }
    }
}


int prepare_cmd_args(std::vector<std::string>& cmd_argv, cmdConfig& cmd_config){
    /*
     * Prepare command arguments for execution call (e.g. process wildcards and stream redirection).
     * Modifies its both arguments.
     * */
    if (cmd_argv.empty()){ return EXIT_FAILURE; }
    int redirect_index = 0;
    for (size_t i = 0; i < cmd_argv.size(); i++ ) {
        if (is_wildcard(cmd_argv[i])) {
            std::vector<std::string> matched_args;
            if (wildcard_matching(cmd_argv[i], matched_args) == EXIT_SUCCESS) {
                cmd_argv.erase(cmd_argv.begin() + i);
                cmd_argv.insert(cmd_argv.end(), matched_args.begin(), matched_args.end());
            } else {
                std::cout << "Couldn't match the wildcard in " << cmd_argv[i] << std::endl << boost::filesystem::current_path();
                return EXIT_FAILURE;
            }

        } else if (is_envariable(cmd_argv[i])){
            auto envar = getenv(cmd_argv[i].substr(1).c_str());
            if (envar) {
                cmd_argv[i] = std::string {envar};
            }
        }
        if (is_redirect(cmd_argv[i])){
            if (parse_redirect(cmd_argv, i, cmd_config.redirect_map) == EXIT_SUCCESS){
                redirect_index = i;
            } else {
                std::cout << "Couldn't parse redirect in " << cmd_argv[i] << std::endl << boost::filesystem::current_path();
                return EXIT_FAILURE;
            }
        }
    }
    if (cmd_argv.size() > 1 && cmd_argv[1][0] == '&'){
        cmd_config.background_execution = true;
        if (cmd_argv[1].length() > 1) {
            cmd_argv[1] = cmd_argv[0].substr(1);
        } else {
            cmd_argv.erase(cmd_argv.begin() + 1);
        }
    }
    if (redirect_index > 0) {
        cmd_argv.erase(cmd_argv.begin()+redirect_index, cmd_argv.end());
    }
    return EXIT_SUCCESS;
}


int main(int argc, char** argv, char* env[]) {

    myShellConfig config;
    std::string path{argv[0]};
    path = path.substr(0, path.find_last_of('/'));

    if (config.set_path_to_external_cmd_modules(path) == EXIT_FAILURE) {
        std::cout << "Unable to get initial pwd: " << path << " is not a valid directory. " << std::endl;
        return EXIT_FAILURE;
    }

    int cmd_status = -1;
    char *buf;

    std::cout << boost::filesystem::current_path();

    while ((buf = readline("$")) != nullptr) {
        if (strlen(buf) == 0) {
            std::cout << boost::filesystem::current_path();
            continue;
        }

        add_history(buf);

        auto cmd_vec = split_str_by(std::string(buf), "|");  // Split into commands.

        // Execute a single command.
        if (cmd_vec.size() == 1) {
            int margc;
            char **margv;
            cmdConfig cmd_config;

            auto cmd_str = strip_str_edges(cmd_vec[0], [](char s) { return s == ' '; });
            if (cmd_str.empty()) continue;

            auto args = split_str_by(cmd_str, " ", "\"");  // Split into arguments.
            if (prepare_cmd_args(args, cmd_config) != EXIT_SUCCESS) {
                std::cout << "Error processing arguments " << cmd_vec[0] << std::endl
                          << boost::filesystem::current_path();
                continue;
            }
            if (parse_into_argv(args, margc, margv) != EXIT_SUCCESS) {
                std::cout << "Couldn't pass arguments of " << cmd_vec[0] << std::endl;
                std::cout << boost::filesystem::current_path();
                continue;
            }
            cmd_status = execute_cmd(margc, margv, env, config, cmd_config);
            if (cmd_status == EXIT_FAILURE) {
                std::cout << "Failed " << cmd_vec[0] << std::endl << boost::filesystem::current_path();
                continue;
            }
            free(margv);
            margc = 0;

        } else {

            /*
             * At each iteration we are working with 2 child processes:
             * - 1 - writes its output into the pipe
             * - 2 - gets its input from the pipe - child of executor :)
             *
             * */

            pid_t pid_init = fork();
            int i = 0;
            while (i + 1 < cmd_vec.size()) {
                if (pid_init == 0) {
                    int pds[2];
                    int p = pipe(pds);
                    if (p == -1) {
                        std::cout << "Failed to construct a pipe" << std::endl;
                        exit(EXIT_FAILURE);
                    }

                    pid_t pid_next = fork();
                    if (pid_next == -1) break;
                    else {
                        if (pid_next == 0) {
                            // reader
                            if (close(pds[1]) == -1) {
                                std::cout << "Failed to close 'write' descriptor for reading." << std::endl;
                                break;
                            }
                            if (pds[0] != STDIN_FILENO) {
                                dup2(pds[0], STDIN_FILENO);
                            }
                            int init_status;
                            waitpid(pid_init, &init_status, 0);
                            if (WEXITSTATUS(init_status) == EXIT_SUCCESS) {
                                i++;
                                pid_init = pid_next;
                            } else { _exit(EXIT_FAILURE); }

                        } else { // writer
                            if (close(pds[0]) == -1) {
                                std::cout << "Failed to close 'read' descriptor for writing." << std::endl;
                                _exit(EXIT_FAILURE);
                            }
                            if (pds[1] != STDOUT_FILENO) {
                                dup2(pds[1], STDOUT_FILENO);

                                int margc;
                                char **margv;
                                cmdConfig cmd_config;

                                auto cmd_str = strip_str_edges(cmd_vec[i], [](char s) { return s == ' '; });
                                if (cmd_str.empty()) _exit(EXIT_FAILURE);

                                auto args = split_str_by(cmd_str, " ", "\"");  // Split into arguments.
                                if (prepare_cmd_args(args, cmd_config) == EXIT_SUCCESS) {
                                    if (parse_into_argv(args, margc, margv) != EXIT_SUCCESS) {
                                        cmd_status = execute_cmd(margc, margv, env, config, cmd_config);
                                    }
                                }
                                close(pds[1]);
                                free(margv);
                                margc = 0;
                                _exit(cmd_status);
                            }
                        }
                    }
                }
            }

        }
        std::cout << boost::filesystem::current_path();
    }
    return EXIT_SUCCESS;
}

// ToDo:

// (!) script execution current process

// (O) add merrno
// (-) replace strip_str_edges' args with <template> and pass lambda with captured strip-char (either delimiter or escape).
