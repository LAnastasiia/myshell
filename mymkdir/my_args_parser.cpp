#include "my_args_parser.h"


int parse_arguments(int argc, char** argv, myConfig &config){
    // argv[0] -- current directory
    for(int i = 1; i < argc; i++){
        size_t len = strlen(argv[i]);
        if (strncmp(argv[i], "-", 1) == 0){
            if (len == 2){
                switch(argv[i][1]){
                    case 'h':
                        config.help = true;
                        break;
                    case 'p':
                        config.p = true;
                        break;
                    default:
                        std::cerr << "Unknown option: " << argv[i][1] << std::endl;
                        return -1;
                }
            } else if (strncmp(argv[i], "--help", 6) == 0){
                config.help = true;
            } else{
                std::cerr << "Unknown option: " << argv[i] << std::endl;
                return -3;
            }
        } else{
            config.dirname = argv[i];
        }
    }

    return 0;
}
