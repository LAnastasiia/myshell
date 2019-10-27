#include "functions.h"


// int merrno(){}


// int mpwd(std::string pwd_holder){}


// int mcd(std::string path){}


// int mexit(){}


// int mecho(){}


 int mexport(int argc, char* argv[]){
     /**
     * Set an environment variable (and it's value if given).
     * @arguments
       * number of arguments (expected 1 or 2);
       * pointer to char array containing variable-name and (optional) variable-value;
     * @return status of execution.
     */
     if (argc == 2) {
         return setenv(argv[0], argv[1], 1);

     } else if (argc == 1) {
         return putenv(argv[0]);
     }
     return -1;
}