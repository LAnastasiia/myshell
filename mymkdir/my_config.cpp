#include "my_config.h"


void help_option(){
    std::cout << "Create new directory." << std::endl;
    std::cout << "Usage: mymkdir [-p] <dirname>" << std::endl;
    std::cout << "-p -- create all transitional directories. If directory exists -- doesn't raise error." << std::endl;
    std::cout << "-h|--help -- help screen." << std::endl;
}