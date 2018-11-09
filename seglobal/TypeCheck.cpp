
#include "../seglobaltypedefs.h"
#include <string.h>
#include <ctype.h>

namespace se
{
    BOOL _typeCheck::IsNumber(const char* s)
    {
        int len = strlen(s);
        for (int i=0; i!=len; ++i)
        {
            if (!isdigit(s[i]))
                return false;
        }
        return true;
    }
}

