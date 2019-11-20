#include "my_arg_parser.h"


int parse_arguments(int argc, char** argv, myConfig &config){
    // TODO: take to attention name of files in quotes
    int i;
    bool only_files = false;
    std::set<char> sorting_methods = {'U', 'S', 't', 'D', 's', 'N'};

    typedef bool (*pfunc)(const myStat&, const myStat&);
    std::map<char, pfunc> funcMap;
    funcMap.insert(std::pair<char, pfunc>('N', byName));
    funcMap.insert(std::pair<char, pfunc>('U', unsorted));
    funcMap.insert(std::pair<char, pfunc>('S', bySize));
    funcMap.insert(std::pair<char, pfunc>('t', byTimeOfLastModification));
    funcMap.insert(std::pair<char, pfunc>('D', directoriesFirst));
    funcMap.insert(std::pair<char, pfunc>('s', specialFilesSeparately));
    config.sorting.push_back(byName);

    // argv[0] -- current directory
    // argv[1] -- ./myls
    for(i = 2; i < argc; i++){
            size_t len = strlen(argv[i]);
            if (strncmp(argv[i], "-", 1) == 0){
                if (len == 2){
                    switch(argv[i][1]){
                        case 'l':
                            config.detailed_info = true;
                            break;
                        case 'r':
                            config.reversed_order = true;
                            break;
                        case 'F':
                            config.specify_type = true;
                            break;
                        case 'R':
                            config.iter_recursively = true;
                            break;
                        case '-':
                            only_files = true;
                            break;
                        default:
                            std::cerr << "Unknown option: " << argv[i][1] << std::endl;
                            return -1;
                    }
                    if (only_files)
                        break;
                } else if (strncmp(argv[i], "--sort", 6) == 0){
                    // 6 -- '='
                    for (int j = 7; j < len; j++){
                        if (sorting_methods.find(argv[i][j]) != sorting_methods.end()){
                            config.sorting.emplace_back(funcMap[argv[i][j]]);
                        } else{
                            std::cerr << "Unknown sorting method: " << argv[i][j] << std::endl;
                            return -2;
                        }
                    }
                } else{
                    std::cerr << "Unknown option: " << argv[i] << std::endl;
                    return -3;
                }
            } else{
               config.objects.emplace_back(argv[i]);
            }
    }

    // all further arguments are files or directories
    for (; i < argc; i++){
        config.objects.emplace_back(argv[i]);
    }

    return 0;
}