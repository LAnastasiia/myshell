#include "functions.h"

int parse_options(boost::program_options::options_description &desc,
        boost::program_options::variables_map &vm, int argc, char* argv[]){
    try
    {
        desc.add_options()("help,h", "Help screen");

        store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        notify(vm);

        return EXIT_SUCCESS;
    }
    catch (const boost::program_options::error &ex)
    {
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }
}

// int merrno(){}


 int mpwd(int argc, char** argv){
     boost::program_options::options_description desc{"Options"};
     boost::program_options::variables_map vm;

     if (parse_options(desc, vm, argc, argv) == EXIT_SUCCESS){
         if (vm.count("help")){
             std::cout << "Usage: mpwd [-h|--help]" << std::endl;
             std::cout << "Print current path." << std::endl;
             std::cout << desc << std::endl;
             return EXIT_SUCCESS;
         }

         char cwd[1024];
         getcwd(cwd, sizeof(cwd));
         std::cout << cwd << std::endl;
         return EXIT_SUCCESS;

     } else{
         std::cerr << "Unable to parse arguments for command mpwd." << std::endl;
         return EXIT_FAILURE;
     }
}


 int mcd(int argc, char** argv){
     boost::program_options::options_description desc{"Options"};
     boost::program_options::variables_map vm;

     if (parse_options(desc, vm, argc, argv) == EXIT_SUCCESS){
         if (vm.count("help")){
             std::cout << "Usage: mcd <path> [-h|--help]" << std::endl;
             std::cout << "Change current location to <path>." << std::endl;
             std::cout << desc << std::endl;
             return EXIT_SUCCESS;
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

     } else{
         std::cerr << "Unable to parse arguments for command mpwd." << std::endl;
         return EXIT_FAILURE;
     }
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