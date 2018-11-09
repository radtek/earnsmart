/* 
 * File:   CSecuritySelectionStatistics.h
 * Author: santony
 *
 * Created on July 2, 2014, 4:14 PM
 */

#ifndef CSECURITYSELECTIONSTATISTICS_H
#define	CSECURITYSELECTIONSTATISTICS_H

#if !defined(EXPAND_MY_SSQLS_STATICS)
#   define MYSQLPP_SSQLS_NO_STATICS
#endif

#include "../seglobal/CROList.h"
#include "sesecuritiesapi.h"

#include <mysql++.h>
#include <ssqls.h>
#include <unordered_set>
#include <random>

using namespace mysqlpp;

//sql_create_9(
//    se_sec_product_keystats, 
//        3, 
//        4, 
//        sql_bigint, product_id,
//        sql_smallint, year,
//        sql_smallint, quarter,
//        sql_double, expected_return_value,
//        sql_decimal, expected_return_rate,
//        sql_decimal, return_gmean,
//        sql_decimal, return_amean,
//        sql_decimal, variance,
//        sql_decimal, stddev);


namespace se
{
    namespace securities
    {
        
        class CSecuritySelectionStatistics : public CObjectRoot, public ISecuritySelectionStatistics
        {
        public:
            CSecuritySelectionStatistics(ISession*);

            virtual ~CSecuritySelectionStatistics();
            
            virtual long unsigned int Init(const Row& r);

            virtual R8 get_AMeanOfReturnRate() const;

            virtual R8 get_Alpha() const;

            virtual R8 get_Beta() const;

            virtual R8 get_ExpectedReturnRate() const;

            virtual R8 get_ExpectedReturnValue() const;

            virtual R8 get_GMeanOfReturnRate() const;

            virtual R8 get_ReturnRisk() const;

            virtual long unsigned int get_SecurityDescription(ISecurityDescription** ppOut);

            virtual R8 get_Volatility() const;
            

            virtual void set_AMeanOfReturnRate(R8 val);

            virtual void set_Alpha(R8 val);

            virtual void set_Beta(R8 val);

            virtual void set_ExpectedReturnRate(R8 val);

            virtual void set_ExpectedReturnValue(R8 val);

            virtual void set_GMeanOfReturnRate(R8 val);

            virtual void set_ReturnRisk(R8 val);

            virtual void set_Volatility(R8 val);

            virtual void Serialize(ISerializedData<IString>* pIn);

            static SE_CODE PopulateInstance(ISession*, const Row& row, ISecuritySelectionStatistics** ppOut);

        private:
            SP<ISecurityDescription> _pdesc;
            mysqlpp::Row _r;
        public:
            //se_sec_product_keystats _data;

        };

        typedef std::unordered_set<SIZE> SizeList;
        
        class CSecuritySelectionStatisticsList : public CROSPList<ISecuritySelectionStatistics, ISecuritySelectionStatisticsList>
        {
        public:
            CSecuritySelectionStatisticsList(ISession*);
            virtual ~CSecuritySelectionStatisticsList();

            long unsigned int Init(SIZE selectionSize, const StoreQueryResult& sqr);

            virtual long unsigned int Reset();

            virtual long unsigned int RenewList();
            

        private:
            StoreQueryResult _fullList;
            SizeList _selectedItems;
            SIZE _selSize;
            std::random_device seeder;

        };        
    }
}

#endif	/* CSECURITYSELECTIONSTATISTICS_H */

