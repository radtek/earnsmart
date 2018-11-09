/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "internal.h"
#include "CMathApi.h"

namespace se
{
    namespace math
    {
        GSLERRHANDLER currentGslErrHandler = nullptr;
        

        SE_CODE MATHEXP MathOpen(ISession* ps, IMathApi** ppOut)
        {
            CMathApi::__loadMessages__(
            {
                CMathApi::CodeMessageItem(E_ANNTRAINEDRECORDNOTFOUND, "A ANN trained record not found."),
                CMathApi::CodeMessageItem(E_GslMemoryError, "Failed to initialize a GSL memory data structure. It could be due to an 'Out of memory' situation.")
            });
            
            if (!ps)
            {
                BERR << "Session not provided";
                return E_InvalidArg;
            }
            return CObject<CMathApi>::Create(ps, ppOut);
        }
        
        void Gsl_ErrorHandler(const char* reason, const char* file, int line, int gsl_errno)
        {
            BERR << "Reason: " << reason << "\n";
            BERR << "Code: " << gsl_errno << "\n";
            BERR << "File(line): " << file << "(" << line << ")" << "\n";
        }
    }
}

