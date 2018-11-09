
#include "internal.h"
#include "GICSInfo.h"

ErrorId CGICSInfo::Init(CSTR sector, CSTR industry)
{
    _sSector = sector;
    _sIndustry = industry;
    return S_Success;
}

CGICSInfo::CGICSInfo(ISession* ps) : CObjectRoot(ps)
{
    //ctor
}

CGICSInfo::~CGICSInfo()
{
    //dtor
}

CSTR CGICSInfo::get_Sector() 
{
    return _sSector.c_str();
}

CSTR CGICSInfo::get_Industry() 
{
    return _sIndustry.c_str();
}

long unsigned int CGICSInfo::get_Companies(IPublicCompanyInfoList** ppOut) 
{
    SP<ISecurityAPI> papi;
    RETURNIFFAILED(SecOpen(get_Session(),this->get_Session()->get_Configs()->get_DefaultMarketDataProvider(),  &papi));
    ostringstream ostr;
    return papi->get_PublicCompanies(ppOut, 
            [this, &ostr](IPublicCompanyInfo::QUERYFLDS fld, LGX& l, BOOL& b)
            {
                ostr.str("");
                switch (fld)
                {
                    case IPublicCompanyInfo::GicId:
                    {
                        b = true;
                        ostr << get_Id();
                        return ostr.str().c_str();
                    }
                }
                return "";
            }
        );
}


void CGICSInfo::Serialize(ISerializedData<IString>* pIn)
{
    if(!pIn)
        return;
    pIn->Add("Sector",_sSector.c_str());
    pIn->Add("Industry",_sIndustry.c_str());
            
}

SE_CODE SECEXP IGICInfo::Create(ISession* ps, CSTR sector, CSTR industry, IGICInfo** ppOut)
{
    return CObject<CGICSInfo>::Create(ps, sector, industry, ppOut);
}

