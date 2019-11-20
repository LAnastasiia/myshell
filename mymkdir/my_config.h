#ifndef MYMKDIR_MY_CONFIG_H
#define MYMKDIR_MY_CONFIG_H

#include <iostream>
#include <string>


struct myConfig{
    // -h, --help | Help option
    bool help = false;
    // -p | create all transitional directories
    bool p = false;
    // <dirname>
    std::string dirname;
};

void help_option();


#endif //MYMKDIR_MY_CONFIG_H
