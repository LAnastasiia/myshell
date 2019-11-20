/* Definitions of helper methods to check if a command is defined in this version of myShell. */

#include "myShellConfig.h"

#include <sys/types.h>
#include <sys/stat.h>

struct stat info;

bool myShellConfig::has_internal_cmd(const std::string& cmd){
    return this->internal_cmd_map.find(cmd) != this->internal_cmd_map.end();
}

bool myShellConfig::has_external_cmd(const std::string& cmd){
    return  std::find(this->external_cmd_vec.begin(), this->external_cmd_vec.end(), cmd) != this->external_cmd_vec.end();
}

bool myShellConfig::has_cmd(const std::string& cmd){
    return this->has_internal_cmd(cmd) || this->has_external_cmd(cmd);
}

int myShellConfig::set_path_to_external_cmd_modules(const std::string& proc_path){
    if (this->bin_path.empty() && (stat(proc_path.c_str(), &info) == 0 && S_ISDIR(info.st_mode))) {
        this->bin_path = proc_path;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
