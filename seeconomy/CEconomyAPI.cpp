/* 
 * File:   CEconomyAPI.cpp
 * Author: santony
 * 
 * Created on January 23, 2014, 9:31 AM
 */


#include "seecconinternal.h"
#include "CEconomyAPI.h"
#include "CIndicatorCategory.h"
#include "../seglobal/exceptions.h"
#include "CEconomicIndicator.h"
#include "CEconomicNewsFeeds.h"
#include "CEconomicNewsStorageProvider.h"
#include "../semysql/CMyqlUtilities.h"
#include "CEconomicIndicatorFeeder.h"
#include <tuple>
#include <stdio.h>
#include <gsl/gsl_fit.h>
#include "../semain/semain.h"
#include <Poco/ScopedLock.h>
#include "../seglobal/CFunctionCache.h"

using namespace se::data;


namespace se
{
    namespace economy
    {
        
        #define QUERYFIELDLOGIC        \
                        if (lgx == LGX_And)             \
                        {                               \
                            if (!bWhere)                \
                            {                           \
                                query << " where ";     \
                                bWhere = true;          \
                            }                           \
                            else                        \
                                query << " and ";       \
                        }                               \
                        else                            \
                        {                               \
                            if (!bWhere)                \
                            {                           \
                                query << " where ";     \
                                bWhere = true;          \
                            }                           \
                            else                        \
                                query << " or ";        \
                        }            

        Poco::Mutex CEconomyAPI::_lock;
       
        //NamedObjectNamingType IEconomyApi::Name = 0xC;
        
        NamedObjectNamingType CEconomyAPI::get_TypeId() 
        {
            return IEconomyApiName;
        }

        CEconomyAPI::CEconomyAPI(ISession* ps) : CObjectRoot(ps)
        {
        }

        CEconomyAPI::~CEconomyAPI() 
        {
        }

        SE_CODE CEconomyAPI::get_EconomicIndicator(UID id, IEconomyIndicator** ppOut)
        {
            return getIndicator(id, ppOut);
        }
        
        SE_CODE CEconomyAPI::set_EconomicIndicator(IEconomyIndicator* pInOut)
        {
            if (!pInOut)
                return E_InvalidArg;

            CSmartConnection conn(SEDBCONN);
            
            try
            {
                Query q = conn->query();

                if (pInOut->get_Id() > 0)
                {
                    q << "UPDATE `sa`.`se_eco_indicators` " ;
                    q << "SET " ;
                    q << "`Name` = " << quote << pInOut->get_Name();
                    q << ", `code` = " << quote << pInOut->get_Code();
                    q << ", `Description` = " << quote << pInOut->get_Description();
                    q << ", `TableName` = " << quote << pInOut->get_TableName();
                    q << ", `PublishPeriod` = " << quote << pInOut->get_PublishPeriod();
                    q << ", `copyright` = " << quote << pInOut->get_CopyRight();
                    q << ", `SourceLink` = " << quote << pInOut->get_Source();
                    q << ", `downloadlink` = " << quote << pInOut->get_DownloadLink();
                    q << ", `ReleaseDates` = " << quote << pInOut->get_ReleaseDates() ;
                    q << ", `Parent_id` = 0 " ;
                    q << ", `Category` = " << pInOut->get_CategoryId() ;
                    // q << ", `LastUpdated` = CURRENT_TIMESTAMP " ;
                    q << ", `product_display` = " << quote << pInOut->get_IsDisplayed() ? "1" : "0" ;
                    q << ", `charttype` = 'Line'";
                    q << ", `seasonal` = " << quote << pInOut->get_IsSeasonal() ? "1" : "0" ;
                    q << ", `determiner` = " << (I16)pInOut->get_Determiner();
                    q << " WHERE `ei_id` = " << pInOut->get_Id();
                   // LOGIP(q.str());
                    q.execute();
                }
                else
                {
                    q << "INSERT INTO `sa`.`se_eco_indicators` ";
                    q << "( ";
                    q << "`Name`, ";
                    q << "`code`, ";
                    q << "`Description`, ";
                    q << "`TableName`, ";
                    q << "`PublishPeriod`, ";
                    q << "`copyright`, ";
                    q << "`SourceLink`, ";
                    q << "`downloadlink`, ";
                    q << "`ReleaseDates`, ";
                    q << "`Parent_id`, ";
                    q << "`Category`, ";
                    q << "`product_display`, ";
                    q << "`charttype`, ";
                    q << "`seasonal`, ";
                    q << "`determiner`) ";
                    q << "VALUES (";
                    q << quote << pInOut->get_Name();
                    q << quote << pInOut->get_Code();
                    q << ", " << quote << pInOut->get_Description();
                    q << ", " << quote << pInOut->get_TableName();
                    q << ", " << quote << pInOut->get_PublishPeriod();
                    q << ", " << quote << pInOut->get_CopyRight();
                    q << ", " << quote << pInOut->get_Source();
                    q << ", " << quote << pInOut->get_DownloadLink();
                    q << ", " << quote << pInOut->get_ReleaseDates() ;
                    q << ", 0 "; 
                    q << ", " << pInOut->get_CategoryId() ;
                    q << ", " << quote << pInOut->get_IsDisplayed() ? "1" : "0" ;
                    q << ", 'Line' ";
                    q << ", " << quote << pInOut->get_IsSeasonal() ? "1" : "0" ;
                    q << ", " << (I16)pInOut->get_Determiner();
                    q << "); ";
                   // LOGIP(q.str());
                    q.execute();
                    pInOut->set_Id(q.insert_id());
                }
                return S_OK;
            }
            CATCHCLAUSE("set_EconomicIndicator")
        }

        SE_CODE CEconomyAPI::get_EconomicIndicators(
                IEconomyIndicatorList** ppOut,
                QUERYFUNC(IEconomyIndicator) criteria
                )
        {
            try
            {
                CSmartConnection conn(SEDBCONN);
                Query query = conn->query();
                if (!criteria)
                    query << "select * from se_eco_indicators order by Name";
                else
                {
                    query << "select * from se_eco_indicators ";
                    bool bWhere = false;
                    bool bresult = false;
                    LGX lgx = LGX_None;
                    CSTR sret = criteria(IEconomyIndicator::QUERYFLDS::CategoryId, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        query << "Category = " << sret;
                    }
                    bresult = false;
                    lgx = LGX_None;
                    sret = criteria(IEconomyIndicator::QUERYFLDS::NameLike, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        query << "Name like " << quote << "%" << sret << "%";
                    }                
                    bresult = false;
                    lgx = LGX_None;
                    sret = criteria(IEconomyIndicator::QUERYFLDS::Displayed, lgx, bresult);
                    if (bresult && strlen(sret))
                    {
                        QUERYFIELDLOGIC;
                        query << "product_display = " << quote << sret ;
                    }                
                    query << " order by Name";
                   // LOGIP(query.str());
                }
                THROWIFFAILED(CObject<CEconomyIndicatorList>::Create(get_Session(), ppOut), "Failed to execute indicator category list query.");
                for(Row r : query.store())
                {
                    SP<IEconomyIndicator> p;
                    THROWIFFAILED(getIndicator(r, &p), "Failed to get indicator category");
                    (*ppOut)->Add(p);
                }
                return S_OK;
            }
            CATCHCLAUSE("get_EconomicIndicators");
        }

        SE_CODE CEconomyAPI::get_IndicatorCategories(IIndicatorCategoryList** ppOut)
        {
            try
            {
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                q << "select * from se_eco_indicatorcategories order by category_name";
                THROWIFFAILED(CObject<CIndicatorCategoryList>::Create(get_Session(), ppOut), "Failed to execute indicator category list query.");
                auto result = q.store();
                for(Row r : result)
                {
                    SP<IIndicatorCategory> p;
                    THROWIFFAILED(getIndicatorCategory(r, &p), "Failed to get indicator category");
                    (*ppOut)->Add(p);
                }
                return S_Success;
            }
            CATCHCLAUSE("getIndicatorCategory");
        }

        SE_CODE CEconomyAPI::get_IndicatorCategory(UID id, IIndicatorCategory** ppOut)
        {
            return getIndicatorCategory(id, ppOut);
        }
        
        EIVAL CEconomyAPI::get_EconomicIndicatorLatestValue(unsigned long id)
        {
            try
            {
                // execute procedure
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                try
                {
                    q << "call latest_EconomicData(" << id << ", 1, 0)";
                    auto qr = q.store();
                    if (!qr.size())
                        throw seexception(E_NotFound, "Not latest value for indicator.");

                    sql_date period = qr[0]["period"];
                    DATAPERIOD dp;
                    dp.Year = period.year();
                    dp.Month = period.month();
                    dp.Day = period.day();
                    R8 value = qr[0]["value"];

                    while (q.more_results())
                        q.store_next();

                    auto tpl = std::make_tuple(dp, value);
                    return tpl;
                }
                catch(...)
                {
                    while (q.more_results())
                        q.store_next();
                    
                    throw ;
                }
            
            }
            catch(const seexception& ex)
            {
               // LOGEP(ex.what());
                throw ex;
            }
            catch(const std::exception& ex)
            {
               // LOGEP(ex.what());
                throw ex;
            }
            catch(...)
            {
               // LOGEP("Unknown error.");
                throw;
            }
        }
        
        long unsigned int CEconomyAPI::get_EconomicIndicatorDataList(
                unsigned long id, 
                se::data::IStoreQueryResult** ppOut,
                U32 instances
            ) 
        {
            try
            {
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                try
                {
                    q << "call list_EconomicData(" << id << ", 1, 0, " << instances << ")";
                    THROWIFFAILED(IStoreQueryResult::Create(get_Session(), q.store(), ppOut), "Failed to create stored query result.");
                    while (q.more_results())
                        q.store_next();
                }
                catch(...)
                {
                    while (q.more_results())
                        q.store_next();
                    throw;
                }
                return S_Success;
            }
            CATCHCLAUSE("get_EconomicIndicatorDataList");
        }

        
        ///////////////////////Private Method////////////////////////////////
        SE_CODE CEconomyAPI::getIndicatorCategory(UID id, IIndicatorCategory **ppOut)
        {
            try
            {
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                q << "select * from se_eco_indicatorcategories where category_id = ";
                q << id;
                UseQueryResult qr = q.use();
                if (!qr)
                    return E_NotFound;
                Row r = qr.fetch_row();
                if (!r)
                    return E_NotFound;
                return getIndicatorCategory(r, ppOut);
            }
            CATCHCLAUSE("getIndicatorCategory");
        }

        SE_CODE CEconomyAPI::getIndicator(UID id, IEconomyIndicator **ppOut)
        {
            try
            {
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query();
                q << "select * from se_eco_indicators order by Name";
                UseQueryResult ur = q.use();
                if (!ur)
                    return E_NotFound;
                Row r = ur.fetch_row();
                if (!r)
                    return E_NotFound;
                return getIndicator(r, ppOut);
            }
            CATCHCLAUSE("get_Indicator");
        }
        
        SE_CODE CEconomyAPI::getIndicatorCategory(Row& row, IIndicatorCategory **p)
        {
            return CObject<CIndicatorCategory>::Create(
                    get_Session(), 
                    row[0], 
                    (string)row[1], 
                    (string)row[2], 
                    (string)row[3]=="1"?true:false, p
                    );
        }
        
        SE_CODE CEconomyAPI::getIndicator(Row& row, IEconomyIndicator** pout)
        {
            SP<CEconomicIndicator> p;
            SE_CODE ret = CObject<CEconomicIndicator>::Create
                    (
                    get_Session(), 
                    (UID)row["ei_id"],
                    (string)row["Name"],
                    (string)row["Description"],
                    (string)row["PublishPeriod"],
                    (string)row["SourceLink"],
                    (string)row["ReleaseDates"],
                    (UID)row["Category"],
                    (string)row["product_display"]=="1"?true:false,
                    (string)row["copyright"],
                    (string)row["ChartType"],
                    (string)row["seasonal"]=="1"?true:false,
                    (string)row["TableName"], 
                    &p);
            p->set_Code((CSTR)row["code"]);
            p->set_DownloadLink((CSTR)row["DownloadLink"]);
            p->set_Determiner((se::training::DET)(I16)row["determiner"]);
            p.CopyTo(pout);
            return ret;
        }

        long unsigned int CEconomyAPI::get_EconomicNewsFeeds(IEconomicNewsFeeds** ppOut) 
        {
            try
            {
                
               // LOGIP("In api-> Reading news feeds from the database.");
                
                CSmartConnection conn(SEDBCONN);
                Query q = conn->query("call list_EcoRSSFeeds()");
                
                try
                {
                    // a.category_id, a.category_name, 
                    // b.rss_id, b.name, b.contributor, b.contributor_url, 
                    // c.rss_date, c.rss_title, c.rss_description, c.rss_link

                    SP<CEconomicNewsFeeds> plist;
                    RETURNIFFAILED(CObject<CEconomicNewsFeeds>::Create(get_Session(), &plist));
                    UID lastcatid = 0;
                    SP<CEconomicNewsFeed> pcat;
                    SP<CEconomicNewsFeeds> pitems;
                    for (const Row& r : q.store())
                    {
                        if (lastcatid != (UID)r["category_id"])
                        {
                            pcat.Clear();
                            RETURNIFFAILED(CObject<CEconomicNewsFeed>::Create(get_Session(), &pcat));
                            plist->Add(pcat);
                            pcat->set_Id(r["category_id"]);
                            pcat->set_Title(r["category_name"]);
                            // create items
                            RETURNIFFAILED(CObject<CEconomicNewsFeeds>::Create(get_Session(), &pitems));

                            pcat->set_Items(pitems);
                        }

                       // LOGI(_logger, "News feed for catid " << lastcatid);

                        SP<CEconomicNewsFeed> pfeed;
                        RETURNIFFAILED(CObject<CEconomicNewsFeed>::Create(get_Session(), &pfeed));

                       // LOGI(_logger, "Created an empty news feed item for catid " << lastcatid);

                        pfeed->set_Id(r["rss_id"]);
                        pfeed->set_Title(r["rss_title"]);
                        pfeed->set_Description(r["rss_description"]);
                        pfeed->set_Link(r["rss_link"]);
                        pfeed->set_Property("contributor", (CSTR)r["contributor"]);
                        pfeed->set_Property("contributorurl", (CSTR)r["contributor_url"]);
                        pfeed->set_Link(r["rss_link"]);
                        sql_datetime sqldt = r["rss_date"];
                        DATETIME dt;
                        CMyqlUtilities::PopulateDateTime(sqldt, dt);
                        pfeed->set_PublishedOn(dt);

                        pitems->Add(pfeed);
                       // LOGI(_logger, "Addomg feed item for catid " << lastcatid);

                        lastcatid = r["category_id"];
                    }

                    while (q.more_results())
                        q.store_next();
                
                    plist.CopyTo(ppOut);
                   // LOGIP("Completed reading economic news from the database.");
                }
                catch(...)
                {
                    while (q.more_results())
                        q.store_next();
                    throw;
                }
               return S_OK;
            }
            CATCHCLAUSE("get_EconomicNewsFeeds")
        }
        
        
        SE_CODE CEconomyAPI::ProcessEconomicNewsFeeds()
        {
           // LOGIP("Starting to process news feeds.");
            CEconomicNewsStorageProvider provider(get_Session());
            return provider.Run();
        }
        
        PERIOD CEconomyAPI::ConvertPeriodFromString(CSTR periodname)
        {
            if (!strcmp(periodname, "Monthly"))
                return PERIOD_Monthly;
            else if (!strcmp(periodname, "Weekly"))
                return PERIOD_Weekly;
            else if (!strcmp(periodname, "Daily"))
                return PERIOD_Daily;
            else if (!strcmp(periodname, "Quarterly"))
                return PERIOD_Quarterly;
            return PERIOD_None;
        }

        

        long unsigned int CEconomyAPI::get_EconomicIndicatorTrainingTable(
            unsigned long indicatorId, 
            I16 countryid, I16 stateid,
            DATAPERIOD from, DATAPERIOD to, 
            se::training::ITrainingTable** ppOutTable) 
        {

            se::training::ITrainingTable* ptable;
            string funcName = "se.economy.get_EconomicIndicatorTrainingTable";
            RTRY
            if (!FC.TrySPGetFValue(funcName, indicatorId, from.ToNumber(), to.ToNumber(), ptable))
            {
                Poco::ScopedLock<Poco::Mutex> g(_lock);
                if (!FC.TrySPGetFValue(funcName, indicatorId, from.ToNumber(), to.ToNumber(), ptable))
                {
                    CSmartConnection conn(SEDBCONN);
                    Query q = conn->query();
                    q << "select TableName, PublishPeriod, determiner from se_eco_indicators where ei_id = " << indicatorId;
                   // LOGIP(q.str());
                    StoreQueryResult sqr = q.store();
                    if (!sqr.size())
                        return E_NotFound;
                    se::training::DET det = (se::training::DET)(I32)sqr[0]["determiner"];

                    q = conn->query();
                    try
                    {
                        RETURNIFFAILED(se::training::CreateTrainingTable(get_Session(), PERIOD_Quarterly, PERIOD_Quarterly, &ptable));
                        std::ostringstream ostr;
                        ostr << "E_" << indicatorId;
                        ptable->set_Name(ostr.str().c_str());

                        q << "call list_EcoDataQtrlyAdjusted(" << indicatorId << ")";
                        DATAPERIOD dp;
                        OmniType lastval;
                        for (const Row& r : q.store())
                        {
                            // first we will skip without adding.
                            sql_decimal_null dval = r[2];
                            dp.Year = (I16)r[0];
                            dp.Quarter = (I16)r[1];
                            if (dval.is_null && !lastval.IsEmpty())
                            {
                                ptable->AddRecord(dp, lastval);
                                continue;
                            }
                            else
                            {
                                const OmniType& val = (R8)dval.data;
                                ptable->AddRecord(dp, val);
                                lastval = val;
                            }
                        }
                        ptable->set_Property("Determiner", (I32)det);

                        while (q.more_results())
                            q.store_next();

                    }
                    catch(...)
                    {
                        while (q.more_results())
                            q.store_next();

                        return E_DBQueryException;
                    }

                    FC.SPSetFValue(funcName, indicatorId, from.ToNumber(), to.ToNumber(), ptable, 3600);
                    *ppOutTable = ptable;
                    return S_OK;
                }
            }
            ptable->AddRef();
            *ppOutTable = ptable;
            return S_OK;
            
            
            RCATCH("get_EconomicIndicatorTrainingTable")
        }

        long unsigned int CEconomyAPI::get_WeightedLinearFitEquation(se::training::ITrainingTable* pTable, double* c0, double* c1, double* c00, double* c01, double* c11, double* chisq) 
        {
            if (!pTable)
            {
               // LOGEP("Invalid pTable. It is null");
                return E_InvalidArg;
            }
            
            if (!c0 || !c1)
            {
               // LOGEP("c0 and c1 cannot be null");
                return E_InvalidPointer;
            }
            
            using namespace se::training;
            
            try
            {
                // create vectors for x and v
                std::vector<R8> Xs, Ys, Ws;
                for (ITrainingRecord* pRec : *pTable)
                {
                    if (pRec->get_Value().IsEmpty())
                        continue;

                    DATAPERIOD dp = pRec->get_Period();
                    R8 xVal = PERIODMASK(dp);
                    R8 yval = pRec->get_Value();

                    Xs.push_back(xVal);
                    Ys.push_back(yval);
                    Ws.push_back(1/sqrt(dp.Month));
                }

                R8 tc0, tc1, tcov00, tcov01, tcov11, tchisq;
                gsl_fit_wlinear(Xs.data(), 1, Ws.data(), 1, Ys.data(), 1, Xs.size(), &tc0, &tc1, &tcov00, &tcov01, &tcov11, &tchisq);

                *c0 = tc0;
                *c1 = tc1;
                if (c00)
                    *c00 = tcov00;
                if (c01)
                    *c01 = tcov01;
                if (c11)
                    *c11 = tcov11;
                if (chisq)
                    *chisq = tchisq;

                return S_OK;
            }
            CATCHCLAUSE("get_WeightedLinearFitEquation")
        }

        long unsigned int CEconomyAPI::get_LinearFitEquation(se::training::ITrainingTable* pTable, double* c0, double* c1, double* c00, double* c01, double* c11, double* chisq) 
        {
            if (!pTable)
            {
               // LOGEP("Invalid pTable. It is null");
                return E_InvalidArg;
            }
            
            if (!c0 || !c1)
            {
               // LOGEP("c0 and c1 cannot be null");
                return E_InvalidPointer;
            }
            
            using namespace se::training;
            try
            {

                // create vectors for x and v
                std::vector<R8> Xs, Ys;
                for (ITrainingRecord* pRec : *pTable)
                {
                    if (pRec->get_Value().IsEmpty())
                        continue;

                    DATAPERIOD dp = pRec->get_Period();
                    R8 xVal = PERIODMASK(dp);
                    R8 yval = pRec->get_Value();

                    Xs.push_back(xVal);
                    Ys.push_back(yval);
                }

                R8 tc0, tc1, tcov00, tcov01, tcov11, tchisq;
                gsl_fit_linear(Xs.data(), 1, Ys.data(), 1, Xs.size(), &tc0, &tc1, &tcov00, &tcov01, &tcov11, &tchisq);

                *c0 = tc0;
                *c1 = tc1;
                if (c00)
                    *c00 = tcov00;
                if (c01)
                    *c01 = tcov01;
                if (c11)
                    *c11 = tcov11;
                if (chisq)
                    *chisq = tchisq;

                return S_OK;
            }
            CATCHCLAUSE("get_LinearFitEquation")

        }

        long unsigned int CEconomyAPI::SourceEconomicData(unsigned long eid) 
        {
            CEconomicIndicatorFeeder feeder;
            return feeder.Run(eid);
        }

        long unsigned int CEconomyAPI::SourceEconomicData() 
        {
            /*
            SP<IFredDataSource> pfds;
            auto rCode = IFredDataSource::Create(&pfds);
            SP<IIndicatorCategoryList> pcatList ;
            rCode = pfds->get_CategoryChildren(0, &pcatList);
            BINF << "Categories Count:" << pcatList->get_Count();
            
            for (IIndicatorCategory* pCat : *pcatList)
            {
                BINF << pCat->get_Name();
                
                // get indicators by category
                SP<IEconomyIndicatorList> pseries;
                rCode = pfds->get_IndicatorsByCategory(pCat->get_Id(), &pseries);
                if (ISOK(rCode))
                {
                    BINF << "Series Count: " << pseries->get_Count();
                    SP<IEconomyIndicator> pind;
                    SP<IEnumerator<IEconomyIndicator*>> penum;
                    if (ISOK(pseries->get_Enumerator(&penum)))
                    {
                        while (penum->MoveNext())
                        {
                            IEconomyIndicator* pind = penum->get_Current();
                            BINF << "Indicator: " << pind->get_Code() << "|" << pind->get_Name() << endl;
                            SP<se::training::ITrainingTable> ptbl;
                            CSTR freq = pind->get_Property("frequency_short");
                            if (ISOK(pfds->get_IndicatorObservations(pind->get_Code(), &ptbl, freq )))
                            {
                                for (se::training::ITrainingRecord* record : *ptbl)
                                {
                                    BINF << "Obsv: " << record->get_Period() << "|" << record->get_Value() << "|" << record->get_Change() << endl;
                                }
                            }
                        }
                    }
                }
            }
            
            return S_OK;
            */
            
            CEconomicIndicatorFeeder feeder;
            return feeder.Run();
            
        }
        
        long unsigned int CEconomyAPI::GetConfigurator(se::config::IConfigurator** ppOut) 
        {
            SP<IApp> papp;
            RETURNIFFAILED(SEOpenApp(&papp));
            return papp->get_Configurator(ppOut);
        }

    }
}
