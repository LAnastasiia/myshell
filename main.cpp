#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <cstring>
#include <sys/stat.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>
//#include <bits/fcntl-linux.h>
#include <fcntl.h>

#include "internal_functions/functions.h"
#include "additionals.h"


namespace fs = boost::filesystem;


int execute_cmd(int margc, char ** margv, char* env[], std::string cmd_str, int &status){
    /* Execute a shell command (either internal or external).
     * Return the status of execution. */

    const std::map <std::string, std::function<int (int argc, char* argv[])>> internal_cmds = {
            std::make_pair("mexport", [](int argc, char** argv)-> int {return mexport(argc, argv);} ),
            std::make_pair("mpwd", [](int argc, char** argv)-> int {return mpwd(argc, argv, 1);} ),
            std::make_pair("mcd", [](int argc, char** argv)-> int {return mcd(argc, argv);} ),
            std::make_pair("mecho", [](int argc, char** argv)-> int {return mecho(argc, argv);} )
    };

    // Execute external file.
    if (cmd_str[0] == '.'){
        std::string filename = cmd_str.substr(1);
        if (fs::exists(filename) && is_file_executable(filename)){

            execvp(filename.c_str(), margv);
            execlp(filename.c_str(), margv[0], env);
            std::cerr << "Failed to execute " << filename << " \n\tCode: " << errno << std::endl;
            return EXIT_FAILURE;

        } else {
            std::cerr << "No such file: " << filename << " \n\tCode: " << errno << std::endl;
            return EXIT_FAILURE;
        }
    }


    std::string cmd = cmd_str.substr(0, cmd_str.find(' '));

    if (strcmp(margv[0], "mexit") == 0){
        status = mexit(margc, margv);
        if (status != EXIT_FAILURE) {
            std::cout << boost::filesystem::current_path();
            return EXIT_SUCCESS;

        }else{
            std::cout << boost::filesystem::current_path();
            return EXIT_FAILURE;
        }
    }

    else if (strcmp(cmd.c_str(), "merrno") == 0){
        status = merrno(margc, margv, status);
        std::cout << boost::filesystem::current_path();
        return EXIT_SUCCESS;
    }

    else if (internal_cmds.find(cmd) != internal_cmds.end()){
        status = internal_cmds.at(cmd)(margc, margv);
    }

    // External command.
    else {
        pid_t pid = fork();

        if (pid == 0) {
            execve((char *)cmd.c_str(), margv, env);
            std::cerr << "Parent: Failed to execute " << margv[0] << " \n\tCode: " << errno << std::endl;
            return EXIT_FAILURE;
        }

        else if (pid > 0){
            bool background_exec = (*cmd_str.end() == '&') && (*cmd_str.end()-1 == ' ');
            if (! background_exec) waitpid(pid, &status, 0);

        } else {
            std::cerr << "Failed to fork()" << std::endl;
            return EXIT_FAILURE;
        }
    }
}


int main(int argc, char** argv, char* env[]) {

    int status = 0, mid_status = 0;
    char* buf; int margc; char** margv;


    while ((buf = readline(" $ ")) != nullptr) {

        std::cout << boost::filesystem::current_path();

        // Validate.
        if (strlen(buf) > 0) add_history(buf);

        // Split the pipeline.
        auto cmd_vector = split_str_by(std::string(buf), "|");

        // --> Single command.
        if (cmd_vector.size() == 1) {
            if (parse_arguments(buf, margc, margv) == EXIT_SUCCESS)
                auto cmd_status = execute_cmd(margc, margv, env, cmd_vector[0], status);
            else continue;

        // --> Pipeline.
        } else {
            for (size_t i = 1; i < cmd_vector.size(); i++){

                // Validate arg parsing.
                if (parse_arguments(buf, margc, margv) == EXIT_FAILURE) continue;

                std::string cmd_str = cmd_vector[i];

                // Create a pipe.
                int pds [2];  // Pipe descriptors for ['read', 'write'] endpoints of communication.
                int p = pipe(pds);

                if (p == -1){
                    std::cerr << "Failed to construct a pipe" << std::endl;
                    exit(EXIT_FAILURE);

                } else {
                    switch (pid_t pid1 = fork()) {
                        case -1:
                        {
                            std::cerr << "Failed to fork()" << std::endl;
                            exit(EXIT_FAILURE);
                            break;
                        }

                        case 0: // 1st child
                        {
                            if (close(pds[0]) == -1) {
                                std::cerr << "Failed to close 'read' descriptor of the 1st child process." << std::endl;
                                exit(EXIT_FAILURE);
                            }
                            // Redirect std output to pipe's write endpoint.
                            if (pds[1] != STDOUT_FILENO) {
                                dup2(pds[1], STDOUT_FILENO);
                                close(pds[1]);  // WHY?
                            }
                            mid_status = execute_cmd(margc, margv, env, cmd_str, status);
                            break;
                        }

                        default: // parent
                            if (close(pds[1]) == -1) {
                                std::cerr << "Failed to close 'write' descriptor of the parent process." << std::endl;
                                exit(EXIT_FAILURE);
                            }
                            switch (pid_t pid2 = fork()) {
                                case -1:
                                    std::cerr << "Failed to fork()" << std::endl;
                                    exit(EXIT_FAILURE);

                                case 0: // 2nd child
                                    if (close(pds[1]) == -1){
                                        std::cerr << "Failed to close 'write' descriptor of a child process." << std::endl;
                                        exit(EXIT_FAILURE);
                                    }

                                    void *buf;
                                    // read
                                    // execute_cmd

                                    break;

                                default: // parent
                                    if (close(pds[1]) == -1){
                                        std::cerr << "Failed to close 'write' descriptor of a parent process." << std::endl;
                                        exit(EXIT_FAILURE);
                                        // write to child's output stream
                                    }
                                    break;
                            }
                            break;
                    }
                }

                if (mid_status) continue;
            }
        }


        std::cout << boost::filesystem::current_path();
    }
    return 0;
}