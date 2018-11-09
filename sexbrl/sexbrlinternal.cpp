/* 
 * File:   sexbrlinternal.cpp
 * Author: santony
 * 
 * Created on July 1, 2012, 5:33 PM
 */

#include "sexbrlinternal.h"
#include <stddef.h>
#include "CXbrl.h"
#include "../seglobal/CFunctionCache.h"

#include <Poco/Mutex.h>
#include <Poco/ScopedLock.h>


#ifdef	__cplusplus
extern "C" {
#endif
    
    namespace se
    {
        namespace xbrl
        {
            ErrorId XBRLEXP XbrlOpen(ISession* ps, IXbrl** ppGateway)
            {
                if (!ps)
                {
                    BERR << "Session not provided";
                    return E_InvalidArg;
                }
                return CObject<CXbrl>::Create(ps, ppGateway);
                
            }
        }
    }

#ifdef	__cplusplus
}
#endif
//
//    /** error messages **/
//
//#define MSGSTRFIELD(line) MSGSTRFIELD1(line)
//#define MSGSTRFIELD1(line) str##line
//static const union msgstr_t {
//struct {
//#define _S(n, s) char MSGSTRFIELD(__LINE__)[sizeof(s)];
//#include "messages.h"
//#undef _S
//};
//char str[0];
//} msgstr = { {
//#define _S(n, s) s,
//#include "messages.h"
//#undef _S
//} };
//static const unsigned int msgidx[] = {
//#define _S(n, s) [n] = offsetof(union msgstr_t, MSGSTRFIELD(__LINE__)),
//#include "messages.h"
//#undef _S
//};
//const char *errstr (int nr) {
//return msgstr.str + msgidx[nr];
//}
//
