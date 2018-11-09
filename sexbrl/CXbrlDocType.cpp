/* 
 * File:   CXbrlDocType.cpp
 * Author: aashok
 * 
 * Created on September 15, 2013, 12:03 PM
 */
#include "sexbrlinternal.h"
#include "CXbrlDocType.h"
#include "../seglobal/exceptions.h"

CXbrlDocType::CXbrlDocType(ISession* ps) : CObjectRoot(ps) 
{
}

CXbrlDocType::~CXbrlDocType() 
{
}

CSTR CXbrlDocType::get_Name()
{
    return _sName.c_str();
}
void CXbrlDocType::set_Name(CSTR value)
{
    _sName = value;
}

void CXbrlDocType::Serialize(ISerializedData<IString>* pIn)
{
    pIn->Add("Id", get_Id());
    pIn->Add("Name", _sName.c_str());
    pIn->Add("Desc", _sDesc.c_str());

    switch (_period)
    {
    case PERIOD_Annually:
        pIn->Add("Period","Annually");
        break;
    case PERIOD_Monthly:
        pIn->Add("Period", "Monthly");
        break;
    case PERIOD_Quarterly:
        pIn->Add("Period","Quarterly");
        break;
    case PERIOD_SemiAnnually:
        pIn->Add("Period", "Semi-Annually");
        break;
    }
}

CSTR CXbrlDocType::get_Desc()
{
    return _sDesc.c_str();
}

void CXbrlDocType::set_Desc(CSTR value)
{
    _sDesc = value;
}

PERIOD CXbrlDocType::get_PeriodType() 
{
    return _period;
}


void CXbrlDocType::set_PeriodType(PERIOD value) 
{
    _period = value;
}

