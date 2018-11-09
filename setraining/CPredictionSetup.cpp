/* 
 * File:   CPredictionSetup.cpp
 * Author: santony
 * 
 * Created on May 12, 2014, 9:17 AM
 */


#include "internal.h"
#include "CPredictionSetup.h"
#include "../seglobal/exceptions.h"
#include "../seeconomy/seeconomapi.h"
#include "../sexbrl/sexbrlapi.h"
#include "../semysql/CSEConnections.h"
#include "CTrainingAPI.h"
#include "../seglobal/globallogger.h"

using namespace mysqlpp;
using namespace se::data;
using namespace se::economy;
using namespace se::xbrl;

namespace se
{
    namespace training
    {
        
        #define SEDBCONN CSEConnections::GetMain()

        bool __attrib::operator<(const __attrib& right) const
        {
            U32 ul = Identifier;
            U32 ur = right.Identifier;
            U32 sl = (U32)Source;
            U32 sr = (U32)right.Source;
            return (ul << 8 | sl) < (ur << 8 | sr );
        }
        
        bool __attrib::operator >(const __attrib& right) const
        {
            U32 ul = Identifier;
            U32 ur = right.Identifier;
            U32 sl = (U32)Source;
            U32 sr = (U32)right.Source;
            return (ul << 8 | sl) > (ur << 8 | sr );
        }

        bool __attrib::operator<=(const __attrib& right) const
        {
            U32 ul = Identifier;
            U32 ur = right.Identifier;
            U32 sl = (U32)Source;
            U32 sr = (U32)right.Source;
            return (ul << 8 | sl) <= (ur << 8 | sr );
        }
        
        bool __attrib::operator >=(const __attrib& right) const
        {
            U32 ul = Identifier;
            U32 ur = right.Identifier;
            U32 sl = (U32)Source;
            U32 sr = (U32)right.Source;
            return (ul << 8 | sl) >= (ur << 8 | sr );
        }
        
        bool __attrib::operator ==(const __attrib& right) const
        {
            U32 ul = Identifier;
            U32 ur = right.Identifier;
            U32 sl = (U32)Source;
            U32 sr = (U32)right.Source;
            return (ul << 8 | sl) == (ur << 8 | sr );
        }

        CPredictionSetup::CPredictionSetup(ISession* ps):CObjectRoot(ps) 
        {
        }

        CPredictionSetup::~CPredictionSetup() 
        {
        }

        void CPredictionSetup::Serialize(ISerializedData<IString>* pIn) 
        {
            pIn->Add("Id", get_Id());
            pIn->Add("Name", get_Name());
            pIn->Add("Algo", get_Algo());
            pIn->Add("Desc", get_Desc());
        }
        
        CSTR CPredictionSetup::get_Name()
        {
            return _name.c_str();
        }
        
        CSTR CPredictionSetup::get_Algo()
        {
            return _algo.c_str();
        }
        
        CSTR CPredictionSetup::get_Desc()
        {
            return _desc.c_str();
        }
        
        void CPredictionSetup::set_Name(const char* v)
        {
            _name = v;
        }
        
        void CPredictionSetup::set_Algo(const char* v)
        {
            _algo = v;
        }
        
        void CPredictionSetup::set_Desc(const char* v)
        {
            _desc = v;
        }

        const TRAINING_RESULT& CPredictionSetup::get_ResultInfo() 
        {
            return _resultinfo;
        }

        void CPredictionSetup::set_ResultInfo(const TRAINING_RESULT& v) 
        {
            _resultinfo = v;
        }

        
        long unsigned int CPredictionSetup::get_TrainingData(DATAPERIOD periodFrom, DATAPERIOD periodTo, IExTrainingTable** ppOut) 
        {
            //LoggerPtr _logger = Logger::getLogger("se::training::CPredictionSetup::get_TrainingData");
            
            RTRY
                if (!_pExTable)
                {                
                    Poco::ScopedLock<Poco::Mutex> g(CTrainingAPI::_trainingLock);
                    if (!_pExTable)
                    {                
                        SP<ITrainingTableList> ptblList;
                        ITrainingTableList::Create(get_Session(), &ptblList);

                        SP<IEconomyApi> peconApi;
                        SP<IXbrl> pxbrl;
                        RETURNIFFAILED(EconOpen(get_Session(), &peconApi));
                        RETURNIFFAILED(XbrlOpen(get_Session(), &pxbrl));

                        CSmartConnection conn(SEDBCONN);
                        Query q = conn->query();
                        q << "SELECT * FROM sa.se_trainingsets_attribs where set_id = " << get_Id();
                        for (const Row& r : q.store())
                        {
                            string source = (CSTR)r["source"];
                            UID attribid = r["attrib_id"];
                            SP<ITrainingTable> ptbl;
                            if (source == "E")
                            {
                               // LOGI(_logger, "Building economic training table for eid " << attribid);
                                DATAPERIOD dpEFrom = periodFrom;
                                if (dpEFrom.IsEmpty())
                                {
                                    dpEFrom.Day = 1;
                                    dpEFrom.Month = 1;
                                    dpEFrom.Year = 2001;
                                }
                                if (ISOK(peconApi->get_EconomicIndicatorTrainingTable(attribid, 0, 0, dpEFrom, periodTo, &ptbl)))
                                    ptblList->Add(ptbl);
                            }
                            else if (source == "P")
                            {
                               // LOGI(_logger, "Building performance training table for perf id " << attribid);
#ifdef __DEBUG__
                                if (attribid == 26)
                                {
                                    string s = "b";
                                }
#endif
                                OmniType prodId = this->get_Property("ProductId");
                                if (!prodId.IsEmpty())
                                {
                                    RatioComplexity complexity = RatioComplexity::SingleVariable;
                                    q = conn->query();
                                    q << "select check_PerfRatioComplexity(" << attribid << ")";
                                    auto sqrcmplx = q.store();
                                    if (sqrcmplx.size())
                                        complexity = (RatioComplexity)(int)sqrcmplx[0][0];
                                    if (complexity == RatioComplexity::SingleVariable)
                                    {
                                        if (ISOK(pxbrl->GetPerformanceTrainingData((UID)prodId, attribid, periodFrom, periodTo, &ptbl)))
                                            ptblList->Add(ptbl);
                                    }
                                    else 
                                    {
                                        SP<ITrainingTableList> pComplexTableList;
                                        if ( ISOK(pxbrl->GetPerformanceTrainingData( (UID)prodId, attribid, periodFrom, periodTo, &pComplexTableList ) ) )
                                        {
                                            for (ITrainingTable* t : *pComplexTableList)
                                            {
                                                ptblList->Add(t);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        q = conn->query();
                        q << "SELECT * FROM sa.se_trainingsets_result where set_id = " << get_Id();
                        StoreQueryResult sqr = q.store();
                        
                        if (!sqr.size())
                        {
                           // LOGE(_logger, "Not training records for company: " << get_Id());
                            return E_TrainingDataNoResult;
                        }

                        const Row& r = sqr[0];
                        string source = (CSTR)r["source"];
                        UID attribid = r["result_id"];
                        SP<ITrainingTable> pResult;
                        if (source == "E")
                        {
                            LOGRETURNIFFAILED(peconApi->get_EconomicIndicatorTrainingTable(attribid, 0, 0, periodFrom, periodTo, &pResult), "Error getting economic training record - Result section");
                        }
                        else if (source == "P")
                        {
                            const OmniType &prodId = this->get_Property("ProductId");
                            if (!prodId.IsEmpty())
                                LOGRETURNIFFAILED(pxbrl->GetPerformanceTrainingData((UID)prodId, attribid, periodFrom, periodTo, &pResult), "Error getting perf training record - Result section");
                        }
                        LOGRETURNIFFAILED(IExTrainingTable::ExtendTables(get_Session(), ptblList, pResult, &_pExTable), "Failed to extend training tables.");
                    }
                }
                _pExTable.CopyTo(ppOut);
                
                return S_OK;
                
            RCATCH("get_TrainingData")
                
        }
        
        
        SE_CODE ECONEXP IPredictionSetup::Create(ISession* ps, IPredictionSetup** ppOut)
        {
            return CObject<CPredictionSetup>::Create(ps, ppOut);
        }
        
        ///////////////////////////////CPredictionSetupList//////////////////////////
        CPredictionSetupList::CPredictionSetupList(ISession* ps) : CSPList<IPredictionSetup, IPredictionSetupList>(ps)
        {
            
        }
        
        CPredictionSetupList::~CPredictionSetupList()
        {
            
        }

        void CPredictionSetupList::Add(IPredictionSetup* item) 
        {
            _map.insert(make_pair(item->get_ResultInfo(), item));
            CSPList<IPredictionSetup, IPredictionSetupList>::Add(item);
        }


        long unsigned int CPredictionSetupList::FindByResult(const TRAINING_RESULT& result, IPredictionSetup** ppOut) 
        {
            auto find = _map.find(result);
            if (find != _map.end())
            {
                find->second.CopyTo(ppOut);
                return S_OK;
            }
            return E_NotFound;
        }


        SE_CODE ECONEXP IPredictionSetupList::Create(ISession* ps, IPredictionSetupList** ppOut)
        {
            return CObject<CPredictionSetupList>::Create(ps, ppOut);
        }

    }
}