#include "functions.h"

// 1 - have help option
// 0 - don't have
// -1 - contain unrecognized options
int help_option_enabled(int argc, char** argv){
    bool found = false;
    for (int i = 1; i < argc; i++){
        if (argv[i][0] == '-'){
            if ((strcmp(argv[i], "-h") == 0) or (strcmp(argv[i], "--help") == 0))
                found = true;
            else {
                std::cerr << "Unrecognized option: " << argv[i] << std::endl;
                return -1;
            }
        }
    }
    return found ? 1 : 0;
}


int merrno(int argc, char** argv, int &my_errno){
    /** Print code of execution of the last command. */
    int help_option = help_option_enabled(argc, argv);
    if (help_option == 1){
        std::cout << "Usage: merrno [-h | --help]" << std::endl;
        std::cout << "Print code of execution of the last command." << std::endl;
        return EXIT_SUCCESS;
    } else if (help_option == -1)
        return EXIT_FAILURE;
    std::cout << my_errno << std::endl;
    return EXIT_SUCCESS;
}


int mpwd(int argc, char** argv, int fd){
    /** Output current path. */
    int help_option = help_option_enabled(argc, argv);
    if (help_option == 1){
        std::cout << "Usage: mpwd [-h|--help]" << std::endl;
        std::cout << "Print current path." << std::endl;
        return EXIT_SUCCESS;
    } else if (help_option == -1){
        return EXIT_FAILURE;
    }

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));


    std::cout << cwd << std::endl;
    return EXIT_SUCCESS;
}


int mcd(int argc, char** argv){
    /** Change current location to <path>. */
    int help_option = help_option_enabled(argc, argv);
    if (help_option == 1){
        std::cout << "Usage: mcd <path> [-h|--help]" << std::endl;
        std::cout << "Change current location to <path>." << std::endl;
        return EXIT_SUCCESS;
    } else if (help_option == -1)
        return EXIT_FAILURE;

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


 int mexit(int argc, char** argv){
     /** Exit from the shell. */
     int help_option = help_option_enabled(argc, argv);
    if (help_option == 1){
        std::cout << "Usage: mexit [-h|--help]" << std::endl;
        std::cout << "Exit from the shell." << std::endl;
        return EXIT_SUCCESS;
    } else if(help_option == -1){
        return EXIT_FAILURE;
    }

    if (argc > 2){
        std::cerr << "To many arguments for mexit command. See [-h|--help]." << std::endl;
        return EXIT_FAILURE;
    }

     for (int i = 1; i < argc; i++){
         if (argv[i][0] != '-'){
             int x = std::stoi(argv[i]);
            std::cout << "EE" << std::endl;
             exit(x);
         }
     }
     exit(0);
}


int mecho(int argc, char** argv){
    /** Print value of an envariable or it's name if such envariable exists. */
    int help_option = help_option_enabled(argc, argv);
    if (help_option == 1) {
        std::cout << "Usage: mecho [-h | --help] [text|$<var_name>] [text|$<var_name>] ..." << std::endl;
        std::cout << "Print value of an envariable or name if such variable exists." << std::endl;
        return EXIT_SUCCESS;
    } else if (help_option == -1){
        return EXIT_FAILURE;

    } else {
        for (int i = 1; i < argc; i++){
            if (argv[i][0] == '$') {
                if (getenv(argv[i]+1) != nullptr){
                    std::cout << getenv(argv[i]+1) << " ";
                }
            } else {
                std::cout << argv[i] << " ";
            }
        }
        std::cout << std::endl;
        return EXIT_SUCCESS;
    }
}



int mexport(int argc, char** argv){
    /** Set an environment variable (and it's value if given). */
    int help_option = help_option_enabled(argc, argv);
    if (help_option == 1) {
        std::cout << "Usage: mexport [-h | --help] <var_name>[=VAL]" << std::endl;
        std::cout << "Put a new envariable or update existing one. Value is also assigned if given." << std::endl;
        return EXIT_SUCCESS;
    } else if (help_option == -1){
        return EXIT_FAILURE;

    } else {
        if (argc == 2) {
            for (size_t i = 0; *(argv[1]+i) != '\0'; i++){
                if (argv[1][i] == '=' && argv[1][i+1] != '\0'){
                    // crop
                    std::string envariable (argv[1]);
                    envariable = envariable.substr(0, i);
                    // Export envariable with a value.
                    return setenv(envariable.c_str(), &argv[1][i+1], 1);
                }
            }
            return putenv(argv[1]);

        } else return EXIT_FAILURE;
    }
}


