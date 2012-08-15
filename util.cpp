#include "util.h"

int util::fact (int n) 
{ 
    int r = 1; 
    for (int i = 2; i <= n; i++) 
    {
	r *= i; 
    }
    return r; 
}