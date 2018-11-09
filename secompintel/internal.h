/* 
 * File:   internal.h
 * Author: santony
 *
 * Created on December 4, 2013, 8:42 PM
 */

#ifndef INTERNAL_H
#define	INTERNAL_H

#include "../seglobal/CDate.h"
#include <string>
#include <map>
#include <vector>
#include "../seglobal/exceptions.h"
#include <log4cxx/logger.h>

#include <sys/time.h>
#include <sys/resource.h>

using namespace se;
using namespace std;

#define LOGI    LOG4CXX_INFO
#define LOGW    LOG4CXX_WARN
#define LOGE    LOG4CXX_ERROR
#define LOGF    LOG4CXX_FATAL


#define LOGIP(str)    LOG4CXX_INFO(_logger, str)
#define LOGWP(str)    LOG4CXX_WARN(_logger, str)
#define LOGEP(str)    LOG4CXX_ERROR(_logger, str)
#define LOGFP(str)    LOG4CXX_FATAL(_logger, str)



#endif	/* INTERNAL_H */

