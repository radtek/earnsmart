#ifndef SECURITY_H
#define SECURITY_H

#include <string>

#include "sesecuritiesapi.h"

using namespace se;
using namespace se::securities;
using namespace std;

class CSecurity : public CObjectRoot, public ISecurity
{
    public:
        CSecurity(ISession*);
        virtual ~CSecurity();
        
        virtual ErrorId Init(ISecurityDescription* secDesc);

        virtual CSTR get_Symbol() ;
        virtual ErrorId get_SecurityDescription(ISecurityDescription** ppOut) ;

        virtual const char* get_Name();

        virtual const char* get_LongName();

        void set_Name(CSTR name);
        
        void set_LongName(CSTR longName);

        virtual ErrorId get_Company(IPublicCompanyInfo** ppOut) ;
        virtual void set_Company(IPublicCompanyInfo*);
        
        virtual void Serialize(ISerializedData<IString>* pIn);
        
        virtual ErrorId get_AvgHistoricalPriceAndVolume(CDate const& from, CDate const& to,  ISecurityPrice** ppOut);
        
        virtual R8 get_Marketcap();
        
        virtual R8 get_IndustryMarketcapShare();
        
        void set_Marketcap(R8 v);
        
        void set_IndustryMarketcapShare(R8 v);

        virtual long unsigned int get_News(ISecurityNewsList**);
    

    protected:
    private:
        string _name, _lname;
        SP<IPublicCompanyInfo> _pco;
        SP<ISecurityDescription> _pSecDesc;
        //log4cxx::LoggerPtr _logger;
        R8 _marketcap, _mktcapsicshare;
                
};

#endif // SECURITY_H
