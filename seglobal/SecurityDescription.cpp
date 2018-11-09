
#include "SecurityDescription.h"

CSecurityDescription::CSecurityDescription(ISession* ps):CObjectRoot(ps)
{
    //ctor
}

CSecurityDescription::~CSecurityDescription()
{
    //dtor
}

unsigned long CSecurityDescription::get_Id() 
{
    if (!_get_Id())
    {
        if (_esid && !_excode.empty())
        {
            std::hash<string> hasher;
            UID _id = hasher(_excode);
            _id = _id << 32 | _esid;
            _set_Id(_id) ;
        }
    }
    return _get_Id();
}


unsigned long CSecurityDescription::get_ESid() 
{
    return _esid;
}


CSTR CSecurityDescription::get_Symbol() 
{
    return _sSymbol.c_str();
}

CSTR CSecurityDescription::get_ExchCode() 
{
    return _excode.c_str();
}

CSTR CSecurityDescription::get_Currency()
{
    return _currency.c_str();
}

CSTR CSecurityDescription::get_Country()
{
    return _country.c_str();
}

SECTYPES CSecurityDescription::get_Type()
{
    return _type;
}

void CSecurityDescription::set_ESid(unsigned long esid) 
{
    _esid = esid;
}


void CSecurityDescription::set_Symbol(CSTR symbol) 
{
    _sSymbol = symbol;
}

void CSecurityDescription::set_ExchCode(CSTR  exCode ) 
{
    _excode = exCode;
}


void CSecurityDescription::set_Currency(CSTR currency ) 
{
    _currency = currency;
}

void CSecurityDescription::set_Country(CSTR country )
{
    _country = country;
}

void CSecurityDescription::set_Type(SECTYPES typeId)
{
    _type = typeId;
}

void CSecurityDescription::Serialize(ISerializedData<IString>* pIn)
{
    if(!pIn)
        return;
    pIn->Add("Id", get_Id());
    pIn->Add("ESid", _esid);
    pIn->Add("Symbol",_sSymbol.c_str());
    pIn->Add("ExchCode",_excode.c_str());
    pIn->Add("Currency",_currency.c_str());
    pIn->Add("Country",_country.c_str());
    pIn->Add("Type", (I32)_type);

}

SE_CODE GBLEXP ISecurityDescription::Create(ISession* ps, ISecurityDescription** ppOut)
{
    return CObject<CSecurityDescription>::Create(ps, ppOut);
}


///////////////////////ISecurityDescriptionList///////////////////////////
CSecurityDescriptionList::CSecurityDescriptionList(ISession* ps):CSPList<ISecurityDescription, ISecurityDescriptionList>(ps)
{
    
}

CSecurityDescriptionList::~CSecurityDescriptionList()
{
    
}

void CSecurityDescriptionList::Serialize(ISerializedData<IString>* pIn) {

}




SE_CODE GBLEXP ISecurityDescriptionList::Create(ISession* ps, ISecurityDescriptionList** ppOut)
{
    return CObject<CSecurityDescriptionList>::Create(ps, ppOut);
}
