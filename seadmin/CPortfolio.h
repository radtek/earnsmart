/* 
 * File:   CPortfolio.h
 * Author: santony
 *
 * Created on June 28, 2014, 1:52 PM
 */

#ifndef CPORTFOLIO_H
#define	CPORTFOLIO_H

#include "seadminapi.h"
#include "../seglobal/CList.h"

namespace se
{
    namespace admin
    {
        
        class CPortfolioItems : public CSPList<IPortfolioItem, IPortfolioItemList>
        {
        public:
            CPortfolioItems(ISession* ps) : CSPList<IPortfolioItem, IPortfolioItemList>(ps)
            {
                
            }
            
            CPortfolioItems(const CPortfolioItems& orig): CSPList<IPortfolioItem, IPortfolioItemList>(orig)
            {
                
            }            
        };

        class CPortfolio : public CObjectRoot, public IPortfolio
        {
        public:
            CPortfolio(ISession*);
            CPortfolio(const CPortfolio& pf)=delete;
            virtual ~CPortfolio();

            virtual const OmniType& get_AMean() const;

            virtual const OmniType& get_Alpha() const;

            virtual const OmniType& get_Beta() const;

            virtual const DateTime& get_CreatedOn() const;

            virtual const OmniType& get_GMean() const;

            virtual long unsigned int get_Items(IPortfolioItemList** ppOut);


            virtual const DateTime& get_ModifiedOn() const;

            virtual const char* get_Name();

            virtual const char* get_Notes() const;

            virtual const OmniType& get_ReturnRisk() const;

            virtual const OmniType& get_RiskFreeReturn() const;

            virtual const OmniType& get_Volatility() const;
            

            virtual void set_AMean(const OmniType& val);

            virtual void set_Alpha(const OmniType& val);

            virtual void set_Beta(const OmniType& val);

            virtual void set_GMean(const OmniType& val);

            virtual void set_Name(const char* name);

            virtual void set_Notes(const char* v);

            virtual void set_ReturnRisk(const OmniType& val);

            virtual void set_RiskFreeReturn(const OmniType& val);

            virtual void set_Volatility(const OmniType& val);
            
            virtual long unsigned int Save();
            

            virtual void Serialize(ISerializedData<IString>* pIn);


            
        private:
            OmniType _amean, _gmean, _alpha, _beta, _returnrisk, _volatility, _riskfreereturn;
            string _name, _notes;
            DateTime _createdon, _modifiedon;
            SP<CPortfolioItems> _items;
            
        };
        
    }
}

#endif	/* CPORTFOLIO_H */

