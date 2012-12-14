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

void util::print_in_file(int id, MethodType method, float res)
{
    switch(method)
    {
	case CC_METHOD:
	{
	    switch(id)
	    {
		case 0:
		  CC_A << res << " " << std::endl;
		  break;
		case 1:
		  CC_B << res << " " << std::endl;
		  break;
	    }
	}
	break;
	case SP_METHOD:
	{
	    switch(id)
	    {
		case 0:
		  SP_A << res << " " << std::endl;
		  break;
		case 1:
		  SP_B << res << " " << std::endl;
		  break;
	    }
	}
	break;
	case EP_METHOD:
	{
	    switch(id)
	    {
		case 0:
		  EP_A << res << " " << std::endl;
		  break;
		case 1:
		  EP_B << res << " " << std::endl;
		  break;
		case 2:
		  EP_C << res << " " << std::endl;
		  break;
	    }
	}
	break;
    }
}
