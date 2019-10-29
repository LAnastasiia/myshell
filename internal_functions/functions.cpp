#include "functions.h"


int mpwd(int argc, char** argv){
    boost::program_options::options_description desc{"Options"};
    try{
        desc.add_options()("help,h", "Help screen");

        boost::program_options::variables_map vm;
        store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (vm.count("help")){
            std::cout << "Usage: mpwd [-h|--help]" << std::endl;
            std::cout << "Print current path." << std::endl;
            std::cout << desc << std::endl;
            return EXIT_SUCCESS;
        }
    }catch (const boost::program_options::error &ex){
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::cout << cwd << std::endl;
    return EXIT_SUCCESS;
}


int mcd(int argc, char** argv){
    boost::program_options::options_description desc{"Options"};
    try{
        desc.add_options()("help,h", "Help screen");
        boost::program_options::variables_map vm;
        store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (vm.count("help")){
            std::cout << "Usage: mcd <path> [-h|--help]" << std::endl;
            std::cout << "Change current location to <path>." << std::endl;
            std::cout << desc << std::endl;
            return EXIT_SUCCESS;
        }
    }catch (const boost::program_options::error &ex){
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }

    if (argc < 2){
        std::cerr << "To few arguments for mcd command. See [-h|--help]." << std::endl;
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++){
        if (argv[i][0] != '-'){
            int st = chdir(argv[i]);
            if (st != 0){
                std::cerr << "Failed to move to " << argv[i] << std::endl;
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
