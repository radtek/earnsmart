/* 
 * File:   CProcessEodData.h
 * Author: santony
 *
 * Created on September 30, 2015, 1:15 PM
 */

#ifndef CPROCESSEODDATA_H
#define	CPROCESSEODDATA_H

#include "IFunctions.h"
#include <zthread/Runnable.h>
#include "../seeoddata/seeoddata.h"
#include <vector>

using namespace std;
using namespace ZThread;
using namespace se;
using namespace se::eoddata;


class CProcessEodDataQuotes :
    public Runnable,
    public CTimedFunction
{
public:
    CProcessEodDataQuotes(ISession* ps, const string& exchanges);
    CProcessEodDataQuotes(const CProcessEodDataQuotes& orig) = delete;
    virtual ~CProcessEodDataQuotes();

    virtual void run();

private:
    vector<string> _exchanges ;
    RP<IEODData> pEodData;
    SP<ISession> _pSession;

};

class CProcessEodDataHistory :
    public Runnable,
    public CTimedFunction
{
public:
    CProcessEodDataHistory(ISession* ps, const string& exchanges);
    CProcessEodDataHistory(const CProcessEodDataHistory& orig) = delete;
    virtual ~CProcessEodDataHistory();

    virtual void run();

private:
    vector<string> _exchanges ;
    RP<IEODData> pEodData;
    SP<ISession> _pSession;

};

#endif	/* CPROCESSEODDATA_H */

