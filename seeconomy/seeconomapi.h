/* 
 * File:   seeconomapi.h
 * Author: santony
 *
 * Created on January 22, 2014, 4:16 PM
 */

#ifndef SEECONOMAPI_H
#define	SEECONOMAPI_H

#define ECONOMYEXP __attribute__ ((visibility ("default")))

#include "../seglobaltypedefs.h"
#include "../seglobal/IObject.h"
#include "../seglobalenums.h"
#include "../seglobal/CTimeStamp.h"
#include "../seglobal/CDate.h"
#include "../semysql/CStoreQueryResult.h"
#include "../setraining/setrainingapi.h"

namespace se
{
    namespace economy
    {
        /*
         * Error codes are defined the following way in a 32-bit number (HRESULT)
         * 
         *  7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0   7 6 5 4 3 2 1 0
         *------------------------------------------------------------------------------------------
         *  E| |C|          |     Facility     | T |                    Code                        |
         * 
         * Facility code of this library is 12
         * 
         */
        
        //#define E_PaypalError                                   0x800C0001
        
        //#define S_NewUserAddress                                0x000C0001      // success, but a new user address record is created
        
        struct IIndicatorCategory;
        struct IEconomyIndicator;
        
        typedef std::tuple<DATAPERIOD, R8 > EIVAL;
        
        typedef ISPList<IIndicatorCategory> IIndicatorCategoryList;
        typedef ISPList<IEconomyIndicator> IEconomyIndicatorList;
        
        #define PERIODMASK(dp) (dp.Year << 16 | dp.Month << 8 | dp.Day)
        
        struct IIndicatorCategory : public IIdName, public ISDObject
        {
            virtual UID get_ParentId() = 0;
            virtual RP<IIndicatorCategory> get_Parent() = 0;
            virtual CSTR get_Description() = 0;
            virtual bool get_IsDisplayed() = 0;
            virtual SE_CODE get_Children(IIndicatorCategoryList**)=0;
            virtual SE_CODE get_Indicators(bool displayTypesOnly, IEconomyIndicatorList** ppOut) = 0;
            static SE_CODE ECONOMYEXP Create(ISession* ps, IIndicatorCategory**);
        };
        
        struct IEconomyIndicator : public IIdName, public ISDObject
        {
            enum QUERYFLDS
            {
                CategoryId,
                NameLike,
                Displayed
            } ;
            
            virtual CSTR get_Code() = 0;
            
            virtual CSTR get_Description() = 0;
            virtual CSTR get_Source() = 0;
            virtual CSTR get_PublishPeriod() = 0;
            virtual CSTR get_ReleaseDates() = 0;
            virtual SE_CODE get_Category(IIndicatorCategory**) = 0;
            virtual UID get_CategoryId() = 0;
            virtual EIVAL get_LatestValue() = 0;
            virtual bool get_IsDisplayed() = 0;
            virtual CSTR get_CopyRight() = 0;
            virtual CSTR get_TableName() = 0;
            virtual bool get_IsSeasonal() = 0;
            virtual CSTR get_DownloadLink() = 0;
            virtual CSTR get_Units() = 0;
            virtual se::training::DET get_Determiner() = 0;
            
            virtual void set_Description(CSTR) = 0;
            virtual void set_Source(CSTR) = 0;
            virtual void set_PublishPeriod(CSTR) = 0;
            virtual void set_ReleaseDates(CSTR) = 0;
            virtual void set_Category(UID catId) = 0;
            virtual void set_IsDisplayed(bool) = 0;
            virtual void set_CopyRight(CSTR) = 0;
            virtual void set_TableName(CSTR) = 0;
            virtual void set_IsSeasonal(bool) = 0;
            virtual void set_DownloadLink(CSTR) = 0;
            virtual void set_Determiner(se::training::DET) = 0;
            virtual void set_Units(CSTR) = 0;
            static SE_CODE ECONOMYEXP Create(ISession*, IEconomyIndicator** ppOut);
            
        };
        
        struct IEconomicNewsFeed;
        
        struct IEconomicNewsFeeds : public IROSPList<IEconomicNewsFeed>
        {
        };

        struct IEconomicNewsFeed : public IObject, public ISerialization
        {
            virtual CSTR get_Title() = 0;
            virtual CSTR get_Link() = 0;
            virtual CSTR get_Description() = 0;
            virtual CSTR get_Language() = 0;
            virtual SE_CODE get_Items(IEconomicNewsFeeds** ppOut) = 0;
            virtual const DATETIME& get_PublishedOn() = 0;
        };
        
        struct IFredDataTag : public IIdName, public ISDObject
        {
        };
        typedef ISPList<IFredDataTag> IFredDataTagList;
        
        struct IFredDataSource : public IObject
        {
            // Methods related to categories
            // Get a category. Set categoryId to zero for the root category.
            virtual SE_CODE get_Category(UID categoryId, IIndicatorCategory**) = 0;
            virtual SE_CODE get_CategoryChildren(UID categoryId, IIndicatorCategoryList**) = 0;
            virtual SE_CODE get_RelatedCategories(UID categoryId, IIndicatorCategoryList**) = 0;
            virtual SE_CODE get_IndicatorsByCategory(UID categoryId, IEconomyIndicatorList**) = 0;
            
            // Methods related to series
            virtual SE_CODE get_Indicator(CSTR code, IEconomyIndicator**) = 0;
            // @frequency : m = monthly, d = daily, w = weekly, q = quarterly, a = annual, sa = semiannual, bw = biweekly
            // @aggr = aggregation method (avg, sum or eop (eod of the period))
            virtual SE_CODE get_IndicatorObservations(CSTR code, se::training::ITrainingTable**, CSTR frequency = "m", CSTR aggr = "avg", const CDate& start = CDate::Empty(), const CDate& end = CDate::Empty()) =0;
            virtual SE_CODE get_IndicatorCategories(CSTR code, IIndicatorCategoryList**) = 0;
            virtual SE_CODE find_Indicators(CSTR searchText, IIndicatorCategoryList**)=0;
            
            static SE_CODE ECONOMYEXP Create(ISession*, IFredDataSource**);
            
       };
        
        static NamedObjectNamingType IEconomyApiName = 0xC;
        
        struct IEconomyApi : public INamedObject, public IErrorDescription
        {
            
            virtual SE_CODE get_EconomicNewsFeeds(IEconomicNewsFeeds** ppOut) = 0;
            
            virtual SE_CODE ProcessEconomicNewsFeeds() = 0;

            virtual SE_CODE get_IndicatorCategory(UID id, IIndicatorCategory **ppOut) = 0;
            
            virtual SE_CODE get_IndicatorCategories(IIndicatorCategoryList **ppOut) = 0;
            
            virtual SE_CODE get_EconomicIndicator(UID id, IEconomyIndicator **ppOut) = 0;
            
            // updates when get_Id returns a + non-zero value, else insert. 
            // get_Id will return new id.
            virtual SE_CODE set_EconomicIndicator(IEconomyIndicator *pInOut) = 0;
            
            virtual SE_CODE get_EconomicIndicators
            (
                IEconomyIndicatorList **ppOut,
                QUERYFUNC(IEconomyIndicator) criteria = nullptr
            ) = 0;

            virtual EIVAL get_EconomicIndicatorLatestValue(UID id) = 0; // check return value. if DATAPERIOD::Empty returns true, then return value is not valid.
            
            virtual SE_CODE get_EconomicIndicatorDataList(
                UID eid, 
                se::data::IStoreQueryResult** ppOut,
                U32 instances = 10
                ) = 0;
            
            // pass from and to with zero period values for full range.
            virtual SE_CODE get_EconomicIndicatorTrainingTable(
                UID indicatorId, 
                I16 countryid, I16 stateid,
                DATAPERIOD from, DATAPERIOD to,
                se::training::ITrainingTable** ppOutTable) = 0;
            
            // get a weighted ordinary least squares equation for a training set data. 
            // Note: when dataset is created, 
            // X is chosen as period in a mask for using PERIODMASK macro (DATAPERIOD as parameter), 
            // while the sqrt of reciprocal of month is used as weight,
            // and value field in the record is used as Y value. 
            // the final equation is is constructed as y = c0 + (x*c1).
            // fields c00, c01 and c11 are covariance matrix values constructed as [c00, c01; c01 c11].
            // chisq is chi square value. A lower value means signification probability.
            virtual SE_CODE get_WeightedLinearFitEquation(
                se::training::ITrainingTable* pTable, 
                R8 *c0, 
                R8 *c1, 
                R8 *c00,
                R8 *c01,
                R8 *c11,
                R8 *chisq
                ) = 0;
                
            // get a ordinary least squares equation for a training set data. 
            // Note: when dataset is created, 
            // X is chosen as period in a mask for using PERIODMASK macro (DATAPERIOD as parameter)
            // and value field in the record is used as Y value. 
            // the final equation is is constructed as y = c0 + (x*c1).
            // fields c00, c01 and c11 are covariance matrix values constructed as [c00, c01; c01 c11].
            // chisq is chi square value. A lower value means signification probability.
            virtual SE_CODE get_LinearFitEquation(
                se::training::ITrainingTable* pTable, 
                R8 *c0, 
                R8 *c1, 
                R8 *c00,
                R8 *c01,
                R8 *c11,
                R8 *chisq
                ) = 0;
            
            
            virtual SE_CODE SourceEconomicData() = 0;
            virtual SE_CODE SourceEconomicData(UID eid) = 0;
            
        };
        
        SE_CODE ECONOMYEXP EconOpen(ISession*, IEconomyApi** ppOut);
        
    }
}


#endif	/* SEECONOMAPI_H */

