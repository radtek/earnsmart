/* 
 * File:   CConfigItem.h
 * Author: santony
 *
 * Created on December 22, 2013, 12:49 PM
 */

#ifndef CCONFIGITEM_H
#define	CCONFIGITEM_H

#include <map>

using namespace se;
using namespace se::config;
using namespace std;

class CConfigItem : public CObjectRoot, public IConfigItem
{
    friend class CConfigurator;
public:
    CConfigItem(ISession*);
    CConfigItem(const CConfigItem& orig);
    virtual ~CConfigItem();
    
    virtual DATATYPES get_Type() ;
    virtual CSTR get_Name() ;
    virtual CSTR get_Value();
    virtual IConfigItem* get_Parent();
    
    void set_Name(const char* name) override;
    void set_Value(const OmniType& value) override;

    
private:
    DATATYPES _type;
    string _name;
    string _value;
    SP<IConfigItem> _pItem;

};

class CConfigItemList : public CObjectRoot, public IConfigItemList
{
public:
    CConfigItemList(ISession*);
    CConfigItemList(const CConfigItemList& orig);
    virtual ~CConfigItemList();
    
    virtual IConfigItem* get_Item(UID configId) ;
    virtual IConfigItem* get_Item(CSTR configName) ;
    
    // internal
    void Add(IConfigItem* p);
    
private:
    typedef vector<SP<IConfigItem>> ConfigList;
    typedef map<UID, SIZE> ConfigIdMap;
    typedef map<string, SIZE> ConfigNameMap;
    
    ConfigList _list;
    ConfigIdMap _idMap;
    ConfigNameMap _nameMap;

};


#endif	/* CCONFIGITEM_H */

