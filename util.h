#ifndef UTIL_H
#define UTIL_H
#include "shared.h"
#include <fstream>

namespace util
{
    int fact(int n);
    bool isLogEnabled();
    void enableLog(bool var);
    void print_in_file(int id, MethodType method, float res, float perc);
    static bool log_enabled;
    static std::ofstream CC_A("cc_method_A"), CC_B("cc_method_B"), 
		  SP_A("sp_method_A"), SP_B("sp_method_B"),
		  EP_A("ep_method_A"), EP_B("ep_method_B"), EP_C("ep_method_C"),
		  CC_A_P("cc_method_A_P"), CC_B_P("cc_method_B_P"), 
		  SP_A_P("sp_method_A_P"), SP_B_P("sp_method_B_P"),
		  EP_A_P("ep_method_A_P"), EP_B_P("ep_method_B_P"), EP_C_P("ep_method_C_P");
}
#endif // UTIL_H
