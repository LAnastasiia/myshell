#ifndef MYLS_COMPARARTORS_H
#define MYLS_COMPARARTORS_H


#include "additionals.h"
#include <iostream>
#include <set>
#include <vector>
#include <sys/stat.h>

#include "my_config.h"


// 0 -- a == b
// -1 -- a < b
// 1 -- a > b
bool bySize(const myStat& a, const myStat& b);

bool byTimeOfLastModification(const myStat& a, const myStat& b);

bool byName(const myStat& a, const myStat& b);

bool directoriesFirst(const myStat& a, const myStat& b);

bool specialFilesSeparately(const myStat& a, const myStat& b);

//std::function<bool(const myStat& a, const myStat& b)> build_comparator(myConfig &config);

bool unsorted(const myStat& a, const myStat& b);


#endif //MYLS_COMPARARTORS_H
