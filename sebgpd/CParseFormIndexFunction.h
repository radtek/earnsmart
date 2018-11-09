/* 
 * File:   CParseFormIndexFunction.h
 * Author: santony
 *
 * Created on October 12, 2012, 6:17 PM
 */

#ifndef CPARSEFORMINDEXFUNCTION_H
#define	CPARSEFORMINDEXFUNCTION_H

#include "IFunctions.h"

#include <string>
#include <vector>

#include <mysql++.h>
#include "../external/mariadbpp/ssqls.h"
#include "../senet/CFtpClient.h"
#include <zthread/Runnable.h>

using namespace std;
using namespace mysqlpp;
using namespace se::net;
using namespace ZThread;

struct rfeedstats;

class CParseFormIndexFunction : public Runnable, public CTimedFunction
{
public:
    CParseFormIndexFunction(ISession* ps, CSTR formType);
    virtual ~CParseFormIndexFunction();
    
    void run();
    
private:
    ErrorId DownloadFile(CFtpClient& ftp, I16 year, I16 quarter, string& outlocalfile);
    ErrorId ProcessForm(I16 year, I16 quarter, const string& filepath);
    
private:
    SP<ISession> _pSession;
    
    typedef vector<string> FORMTYPES;
    typedef map<string, I64> FORMTYPESMAP;
    FORMTYPESMAP _formTypes;
    string _tmpdir;
};

#endif	/* CPARSEFORMINDEXFUNCTION_H */

