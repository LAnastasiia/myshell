/* Definition of a helper class to store configurations for different versions of myShell. */

#ifndef MYSHELL_MYSHELLCONFIG_H
#define MYSHELL_MYSHELLCONFIG_H


#include <map>
#include <functional>
#include <vector>
#include "internal_functions/functions.h"

class myShellConfig {
public:
    const std::map <std::string, std::function<int (int argc, char* argv[])>> internal_cmd_map;
    const std::vector <std::string> external_cmd_vec;
    // The root path for external commands' modules; assigned only once at runtime.
    std::string bin_path;

    myShellConfig() : internal_cmd_map({ std::make_pair("mcd", [](int argc, char** argv)-> int {return mcd(argc, argv);} ),
                                            std::make_pair("mecho", [](int argc, char** argv)-> int {return mecho(argc, argv);} ),
                                            std::make_pair("mpwd", [](int argc, char** argv)-> int {return mpwd(argc, argv, 1);} ),
                                            std::make_pair("mexit", [](int argc, char** argv)-> int {return mexit(argc, argv);} ),
                                            std::make_pair("mexport", [](int argc, char** argv)-> int {return mexport(argc, argv);} )}),
                      external_cmd_vec ({"mycat", "myls", "mymkdir"}),
                      bin_path ("")
                      {}

    bool has_internal_cmd(const std::string& cmd);
    bool has_external_cmd(const std::string& cmd);
    bool has_cmd(const std::string& cmd);
    int set_path_to_external_cmd_modules(const std::string& proc_path);
};


#endif //MYSHELL_MYSHELLCONFIG_H
