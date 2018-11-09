/* 
 * File:   CFinancialStatementPresentation.cpp
 * Author: santony
 * 
 * Created on January 27, 2014, 7:08 PM
 */

#include "sexbrlinternal.h"
#include "CXbrlStatementList.h"
#include "CFinancialStatementPresentation.h"
#include "../semain/semain.h"
#include "../seglobal/exceptions.h"
#include "CFinancialItem.h"

using namespace se::threading;

#define SEDBCONN        se::data::CSEConnections::GetMain()

template<typename T>
struct CompareDataXbrls
{
    T t;
    CompareDataXbrls(){}
    CompareDataXbrls(const T& c)
    {
        t = c;
    }
    
    bool operator()(const T& rowa, const T& rowb) const
    {
        return rowa.xbrl_id < rowb.xbrl_id;
    }

    bool operator()(const T& other) const
    {
        return t.xbrl_id == other.xbrl_id;
    }
} ;


CFinancialStatementPresentation::CFinancialStatementPresentation() // : _logger(Logger::getLogger("se.xbrl.financialstatementview"))
{
    
}

CFinancialStatementPresentation::CFinancialStatementPresentation(const CFinancialStatementPresentation& orig) 
{
    
}

CFinancialStatementPresentation::~CFinancialStatementPresentation() 
{
    
}

long unsigned int CFinancialStatementPresentation::Init(CIK cik) 
{
    try
    {
        CSmartConnection conn(SEDBCONN);
        // get presentation tree
        Query q = conn->query();
        q << "call list_FinancialStatements(" << cik << ", 0);";
        for (const Row& r : q.store())
        {
            _statements.insert(make_pair((UID)r["statement_id"], (string)r["title"]));
        }
        while (q.more_results()) q.store_next();
        return S_OK;
    }
    CATCHCLAUSE("Init")  ;
}

long unsigned int CFinancialStatementPresentation::BuildStatementOnDemand(unsigned long statementId) 
{
    auto it = _graphs.find(statementId);
    if (it == _graphs.end())
    {
        CSmartConnection conn(SEDBCONN);
        // get presentation tree
        Query q = conn->query();
        q << "call get_PresentationTables(" << cik << ", " << statementId << ");";
    
    }
    return S_OK;
}



long unsigned int CFinancialStatementPresentation::get_Statement(STATEMENTTYPES statementType, IXbrlStatement**) 
{
    return S_OK;
}

long unsigned int CFinancialStatementPresentation::get_Statements(IXbrlStatementList**) 
{
    return S_OK;
}


void CFinancialStatementPresentation::Serialize(ISerializedData<IString>* pIn) 
{
}

void CFinancialStatementPresentation::SubmitToDb() 
{
    class CTask : public CObjectRoot, public ITask
    {
        SP<CFinancialStatementPresentation> _o;
        
    public:
        CTask()
        {
        }
        
        SE_CODE Init(CFinancialStatementPresentation* o)
        {
            _o = 0;
            return S_OK;
        }
        
        virtual void Execute()
        {
            
        }
        
        virtual ~CTask()
        {
            
        }
    };
    
    
    SP<IApp> pApp;
    THROWIFFAILED(SEOpenApp(&pApp), "Failed to create application instance.");
    SP<IThreadPool> ptp;
    THROWIFFAILED(pApp->get_ThreadPool(TPT_ParallelPool, &ptp), "Failed to create threadpool instance.");
    SP<ITask> task;
    THROWIFFAILED(CObject<CTask>::Create(nullptr, this, &task), "Failed to create a task instance.");
    ptp->RunTask(task);
}



