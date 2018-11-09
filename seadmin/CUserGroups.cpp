/* 
 * File:   CUserGroups.cpp
 * Author: santony
 * 
 * Created on April 8, 2013, 8:45 PM
 */

#include "CUserGroups.h"

using namespace se;
using namespace se::admin;

CUserGroups::CUserGroups() {
}

CUserGroups::CUserGroups(const CUserGroups& orig) {
}

CUserGroups::~CUserGroups() {
}

USERGROUPS CUserGroups::GetSerializedvalue(ISerializedData<IString>* pIn, CSTR propName)
{
    return (USERGROUPS)pIn->getInt32(propName);
}

void CUserGroups::SetSerializedValue(ISerializedData<IString>* pIn, CSTR PropName, const USERGROUPS& value)
{
    pIn->Add(PropName, (I32)value);
}
