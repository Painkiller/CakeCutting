#ifndef UTIL_H
#define UTIL_H
#include "shared.h"

namespace util
{
    int fact(int n);
    bool isLogEnabled();
    void enableLog(bool var);
    
    static bool log_enabled;
}
#endif // UTIL_H
