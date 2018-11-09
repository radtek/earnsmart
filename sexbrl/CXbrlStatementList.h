/* 
 * File:   CXbrlStatementList.h
 * Author: santony
 *
 * Created on February 27, 2014, 6:28 AM
 */

#ifndef CXBRLSTATEMENTLIST_H
#define	CXBRLSTATEMENTLIST_H

#include "../seglobal/CROList.h"

using namespace se;
using namespace se::xbrl;
////using namespace log4cxx;

typedef std::map<UID, string> StatementMap;

class CXbrlStatementList : public CROSPList<IXbrlStatement, IXbrlStatementList>
{
    
public:
    CXbrlStatementList(ISession*);
    CXbrlStatementList(const CXbrlStatementList& orig)=delete;
    virtual ~CXbrlStatementList();
    SE_CODE Init(CIK cik);

private:
    StatementMap _map;
    string _cik;
    ////LoggerPtr _logger;
    I16 _floorYear, _ceilYear;
};

#endif	/* CXBRLSTATEMENTLIST_H */

