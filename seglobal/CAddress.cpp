/* 
 * File:   CAddress.cpp
 * Author: santony
 * 
 * Created on September 14, 2012, 6:37 AM
 */

#include "IObject.h"
#include "CObjectRoot.h"
#include "CAddress.h"
#include "Object.h"

using namespace se;

CAddress::CAddress(ISession* ps):CObjectRoot(ps) 
{
}

CAddress::CAddress(const CAddress& orig) : 
    CObjectRoot(orig),
        _sname(orig._sname),
    _sStreet(orig._sStreet),
        _sCity(orig._sCity),
        _sState(orig._sState),
        _sCountry(orig._sCountry),
        _sPostCode(orig._sPostCode)
{
    
}

CAddress::~CAddress() {
}

CSTR CAddress::get_Name() 
{
    return _sname.c_str();
}

void CAddress::set_Name(CSTR value) 
{
    _sname = value;
}

CSTR CAddress::get_Street() 
{
    return _sStreet.c_str();
}
CSTR CAddress::get_Street2() 
{
    return _sStreet2.c_str();
}

CSTR CAddress::get_City() 
{
    return _sCity.c_str();
}

CSTR CAddress::get_State() 
{
    return _sState.c_str();
}

CSTR CAddress::get_Country() 
{
    return _sCountry.c_str();
}

CSTR CAddress::get_PostalCode()
{
    return _sPostCode.c_str();
}

void CAddress::set_Street(CSTR value) 
{
    _sStreet = value;
}
void CAddress::set_Street2(CSTR value) 
{
    _sStreet2 = value;
}

void CAddress::set_City(CSTR value) 
{
    _sCity = value;
}

void CAddress::set_State(CSTR value) 
{
    _sState = value;
}

void CAddress::set_Country(CSTR value) 
{
    _sCountry = value;
}

void CAddress::set_PostalCode(CSTR value) 
{
    _sPostCode = value;
}

bool CAddress::Equals(IObject* other) 
{
    IAddress* paddr = dynamic_cast<IAddress*>(other);
    if (paddr == nullptr)
        return false;
    CAddress* pp = static_cast<CAddress*>(paddr);
    if (_sStreet == pp->_sStreet &&
            _sStreet2 == pp->_sStreet2 &&
            _sCity == pp->_sCity &&
            _sState == pp->_sState &&
            _sCountry == pp->_sCountry &&
            _sPostCode == pp->_sPostCode)
        return true;
    return false;
}


void CAddress::Serialize(ISerializedData<IString>* pIn)
{
    pIn->set_TypeName("IAddress");
    pIn->Add("Name", _sname.c_str());
    pIn->Add("Street", _sStreet.c_str());
    pIn->Add("City", _sCity.c_str());
    pIn->Add("State", _sState.c_str());
    pIn->Add("PostalCode", _sPostCode.c_str());
    pIn->Add("Country", _sCountry.c_str());
}

ErrorId CAddress::Deserialize(ISerializedData<IString>* pIn)
{
    _sname = pIn->getString("Name");
    _sStreet = pIn->getString("Street");
    _sCity = pIn->getString("City");
    _sState = pIn->getString("State");
    _sPostCode = pIn->getString("PostalCode");
    _sCountry = pIn->getString("Country");
}

ErrorId IAddress::Create(ISession* ps, IAddress** ppOut)
{
    return CreateAddress(ps, ppOut);
}

extern "C" 
{
    ErrorId CreateAddress(ISession* ps, IAddress** ppOut)
    {
        return CObject<CAddress>::Create(ps, ppOut);
    }
}