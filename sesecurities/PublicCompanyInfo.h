#ifndef PUBLICCOMPANYINFO_H
#define PUBLICCOMPANYINFO_H

#include <string>

#include "sesecuritiesapi.h"

using namespace std;
using namespace se;
using namespace se::securities;

class CPublicCompanyInfo : public CObjectRoot, public IPublicCompanyInfo
{
public:
    CPublicCompanyInfo(ISession*);

    virtual ~CPublicCompanyInfo();

    // override get id
    virtual UID get_Id();

    virtual CSTR get_Name() 
    { 
        EnsureInitData();
        return _sName.c_str();
    }

    void set_Name(CSTR value)
    {
        _sName = value;
    }

    virtual CSTR get_Summary() 
    {
        EnsureInitData();
        return _sSummary.c_str();
    }

    void set_Summary(CSTR value)
    {
        _sSummary = value;
    }

    virtual ErrorId get_GICInfo(IGICInfo** ppOut) ;

    void set_GICInfo(IGICInfo* pIn)
    {
        _pGics = pIn;
    }

    virtual CSTR get_Ticker() 
    { 
        return _pSecDesc->get_Symbol();
    }

    virtual ErrorId get_Address(IAddress** ppOut) 
    {
        EnsureInitData();
        _pAddress.CopyTo(ppOut);
        return S_Success;
    }

    void set_Address(IAddress* pIn)
    {
        _pAddress = pIn;
    }

    virtual ErrorId get_Performance(IString** ppOut) ;

    virtual CSTR get_SICInfo();

    virtual void set_SICInfo(CSTR);

    virtual void set_CIK(CIK);

    virtual CIK get_CIK();

    virtual CSTR get_UrlNews();
    
    void set_UrlNews(CSTR v);

    virtual CDate& get_FinancialYearEnd() ;
        
    //virtual ErrorId get_PerformanceIndicators(IIndicatorList** ppOut);

    virtual void Serialize(ISerializedData<IString>* pIn);

    virtual ErrorId Init();

    ErrorId Init(ISecurityDescription* securityDesc);
       
private:
    ErrorId EnsureInitData() ;
    

    
    
protected:

private:
    SP<ISecurityDescription> _pSecDesc;
    string _sName, _sSummary, _sSICInfo, _cik, _urlnews;
    SP<IAddress> _pAddress;
    SP<IGICInfo> _pGics;
    //log4cxx::LoggerPtr _logger;
    bool _bInitData ;
    CDate _finyearend;
        
};

#endif // PUBLICCOMPANYINFO_H
