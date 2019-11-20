#include "comparartors.h"


bool bySize(const myStat& a, const myStat& b){
//    if (a.fileStat.st_size < b.fileStat.st_size)
//        return -1;
//    else if (a.fileStat.st_size > b.fileStat.st_size)
//        return 1;
    return a.fileStat.st_size < b.fileStat.st_size;
}

bool byTimeOfLastModification(const myStat& a, const myStat& b){
//    if (a.fileStat.st_mtimespec.tv_sec < b.fileStat.st_mtimespec.tv_sec)
//        return -1;
//    else if (a.fileStat.st_mtimespec.tv_sec > b.fileStat.st_mtimespec.tv_sec)
//        return 1;
    return a.fileStat.st_mtimespec.tv_sec < b.fileStat.st_mtimespec.tv_sec;
}

bool byName(const myStat& a, const myStat& b){
    return a.filename.compare(b.filename) < 0;
}

bool directoriesFirst(const myStat& a, const myStat& b){
//    if (S_ISDIR(a.fileStat.st_mode)){
//        if (S_ISDIR(b.fileStat.st_mode)){
//            return 0;
//        } else
//            return -1;
//    }
    return S_ISDIR(a.fileStat.st_mode);
}

bool specialFilesSeparately(const myStat& a, const myStat& b){
//    if (S_ISREG(a.fileStat.st_mode)){
//        if (S_ISREG(b.fileStat.st_mode)){
//            return 0;
//        } else{
//            return -1;
//        }
//    }
    return S_ISREG(a.fileStat.st_mode);
}

bool unsorted(const myStat& a, const myStat& b){
    return true;
}



//std::function<bool(const myStat& a, const myStat& b)> build_comparator(myConfig &config){
//    typedef int (*pfunc)(const myStat&, const myStat&);
//    std::map<char, pfunc> funcMap;
//    funcMap.insert(std::pair<char, pfunc>('N', byName));
//    funcMap.insert(std::pair<char, pfunc>('U', unsorted));
//    funcMap.insert(std::pair<char, pfunc>('S', bySize));
//    funcMap.insert(std::pair<char, pfunc>('t', byTimeOfLastModification));
//    funcMap.insert(std::pair<char, pfunc>('D', directoriesFirst));
//    funcMap.insert(std::pair<char, pfunc>('s', specialFilesSeparately));
//    std::vector<char> sorting = config.sorting;
//    std::function<bool(const myStat& a, const myStat& b)> x1 = [&sorting, &funcMap](const myStat& a, const myStat& b) -> bool{
//        for (auto rit = sorting.rbegin();
//             rit != sorting.rend(); ++rit ) {
//            if (funcMap[*rit](a, b) != 0)
//                return funcMap[*rit](a, b) < 0;
//        }
//        return true;
//    };
//    return x1;
//}
