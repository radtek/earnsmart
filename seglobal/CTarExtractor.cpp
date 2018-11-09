/* 
 * File:   CTarExtractor.cpp
 * Author: santony
 * 
 * Created on November 2, 2012, 10:43 PM
 */

#include "../seglobaltypedefs.h"
#include "CTarExtractor.h"

#include <libtar.h>
#include <unistd.h>
#include<signal.h>
#include <fcntl.h>
#include <sys/param.h>
#include <signal.h>

namespace se
{
    namespace utils
    {


        CTarExtractor::CTarExtractor() 
        {
        }

        CTarExtractor::CTarExtractor(const CTarExtractor& orig) 
        {
        }

        CTarExtractor::~CTarExtractor() 
        {
        }

        bool CTarExtractor::Extract(CSTR TarFile, CSTR ExtractDir)
        {
            TAR *t;
            if (tar_open(&t, (char*) TarFile, NULL, O_RDONLY, 0, TAR_GNU) == -1)
            {
                return false;
            }

            bool bRet = tar_extract_all(t, (char*) ExtractDir) != 0;

            
            if (tar_close(t) != 0)
            {
                //fprintf(stderr, "tar_close(): %s\n", strerror(errno));
                bRet = false;
            }

            return bRet;

        }
    }
}
