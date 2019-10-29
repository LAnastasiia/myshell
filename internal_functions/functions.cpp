#include "functions.h"


bool help_option_enabled(int argc, char** argv){
    for (int i = 1; i < argc; i++){
        if (argv[i][0] == '-'){
            if ((strcmp(argv[i], "-h") == 0) or (strcmp(argv[i], "--help") == 0))
                return true;
            else {
                std::cerr << "Unrecognized option: " << argv[i] << std::endl;
                return false;
            }
        }
    }
    return false;
}


int mpwd(int argc, char** argv){
    if (help_option_enabled(argc, argv)){
        std::cout << "Usage: mpwd [-h|--help]" << std::endl;
        std::cout << "Print current path." << std::endl;
        return EXIT_SUCCESS;
    }

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::cout << cwd << std::endl;
    return EXIT_SUCCESS;
}


int mcd(int argc, char** argv){
    std::cout << "Before" << std::endl;
    for(int i = 0; i < argc; i++ ){
        std::cout << argv[i] << std::endl;
    }

    if (help_option_enabled(argc, argv)){
        std::cout << "Usage: mcd <path> [-h|--help]" << std::endl;
        std::cout << "Change current location to <path>." << std::endl;
        return EXIT_SUCCESS;
    }

    std::cout << "After" << std::endl;
    for(int i = 0; i < argc; i++ ){
        std::cout << argv[i] << std::endl;
    }

    if (argc < 2){
        std::cerr << "To few arguments for mcd command. See [-h|--help]." << std::endl;
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++){
        if (argv[i][0] != '-'){
            int st = chdir(argv[i]);
            if (st != 0){
                std::cerr << "Failed to move to " << argv[i-1] << std::endl;
                return EXIT_FAILURE;
            } else{
                std::cout << argv[i] << std::endl;
                return EXIT_SUCCESS;
            }
        }
    }

    std::cerr << "Haven't found path among arguments." << std::endl;
    return EXIT_FAILURE;
}




// int mexit(){}


// int mecho(){}


int mexport(int argc, char** argv){
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


// int merrno(){}
