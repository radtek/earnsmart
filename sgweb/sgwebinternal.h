/* 
 * File:   sgwebinternal.h
 * Author: santony
 *
 * Created on October 1, 2013, 8:15 AM
 */

#ifndef SGWEBINTERNAL_H
#define	SGWEBINTERNAL_H

#include "../ReturnCodes.h"
#include "../seglobal/Object.h"
#include "../secommon.h"
#include "../seglobal/IObject.h"
//#include <log4cxx/logger.h>
#include <boost/algorithm/string.hpp>

//using namespace log4cxx;
using namespace boost;

//extern log4cxx:://LoggerPtr logger;

#define LOGI    //LOG4CXX_INFO
#define LOGW    //LOG4CXX_WARN
#define LOGE    //LOG4CXX_ERROR
#define LOGF    //LOG4CXX_FATAL


#define LOGIP(str)    //LOG4CXX_INFO(_logger, str)
#define LOGWP(str)    //LOG4CXX_WARN(_logger, str)
#define LOGEP(str)    //LOG4CXX_ERROR(_logger, str)
#define LOGFP(str)    //LOG4CXX_FATAL(_logger, str)


#define SMTPWRITE(msg)      \
    std::ostringstream ostr;        \
    ostr << msg;                        \
    set_Error(ostr.str());  

#endif	/* SGWEBINTERNAL_H */

