/* 
 * File:   CConfigItem.cpp
 * Author: santony
 * 
 * Created on December 22, 2013, 12:49 PM
 */

#include "semaininternal.h"
#include "CConfigItem.h"


CConfigItem::CConfigItem(ISession* ps) : CObjectRoot(ps)
{
}

CConfigItem::CConfigItem(const CConfigItem& orig) : CObjectRoot(orig)
{
}

CConfigItem::~CConfigItem() 
{
}

DATATYPES CConfigItem::get_Type()
{
    return _type;
}

CSTR CConfigItem::get_Name()
{
    return _name.c_str();
}

CSTR CConfigItem::get_Value()
{
    return _value.c_str();
}

IConfigItem* CConfigItem::get_Parent()
{
    if (!_pItem) return nullptr;
    _pItem->AddRef();
    return _pItem._ptr;
}

void CConfigItem::set_Name(const char* name) {

    _name = name;
}

void CConfigItem::set_Value(const OmniType& value) 
{
    SP<IString> pstr;
    if (ISOK(value.toString(&pstr)))
    {
        _value = pstr->get_Buffer();
        _type = value.dt;
    }
}

SE_CODE IConfigItem::Create(ISession* ps, IConfigItem** ppOut)
{
    return CObject<CConfigItem>::Create(ps, ppOut);
}


//CConfigItemList

CConfigItemList::CConfigItemList(ISession* ps) : CObjectRoot(ps)
{
}

CConfigItemList::CConfigItemList(const CConfigItemList& orig) : CObjectRoot(orig)
{
}

CConfigItemList::~CConfigItemList() 
{
}

IConfigItem* CConfigItemList::get_Item(UID configId) 
{
    ConfigIdMap::const_iterator it;
    if ( (it = _idMap.find(configId)) == _idMap.end())
        return nullptr;
    IConfigItem* pItem = _list.at(it->second)._ptr;
//    pItem->AddRef();
    return pItem;
}

IConfigItem* CConfigItemList::get_Item(CSTR configName)
{
    ConfigNameMap::const_iterator it;
    if ( (it = _nameMap.find(configName)) == _nameMap.end())
        return nullptr;
    IConfigItem* pItem = _list.at(it->second)._ptr;
    //pItem->AddRef();
    return pItem;
}

void CConfigItemList::Add(IConfigItem* p)
{
    _idMap.insert(make_pair(p->get_Id(), _list.size()));
    _nameMap.insert(make_pair(p->get_Name(), _list.size()));
    _list.push_back(p);
}


