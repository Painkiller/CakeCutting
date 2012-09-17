#include "util.h"

// bool util::log_enabled;

int util::fact (int n) 
{ 
    int r = 1; 
    for (int i = 2; i <= n; i++) 
    {
	r *= i; 
    }
    return r; 
}

bool util::isLogEnabled()
{
    if(log_enabled)
	return true;
    return false;
}

void util::enableLog(bool var)
{
    log_enabled = var;
}
