/* 
 * File:   CXbrlDocType.h
 * Author: aashok
 *
 * Created on September 15, 2013, 12:03 PM
 */

#ifndef CXBRLDOCTYPE_H
#define	CXBRLDOCTYPE_H
#include <string>

#include "../seglobaltypedefs.h"
#include "sexbrlapi.h"
#include "../seglobal/CROList.h"
using namespace se;
using namespace se::xbrl;

class CXbrlDocType : public CObjectRoot, public IXbrlDocType
{
public:
    CXbrlDocType(ISession*);
    CXbrlDocType(const CXbrlDocType& orig)=delete;
    virtual ~CXbrlDocType();

    virtual CSTR get_Name();
    virtual void set_Name(CSTR value);
    virtual CSTR get_Desc();
    virtual void set_Desc(CSTR value);
    virtual PERIOD get_PeriodType();
    virtual void set_PeriodType(PERIOD value);

    virtual void Serialize(ISerializedData<IString>* pIn);
       
private:
    string _sName;
    string _sDesc;
    PERIOD _period;
};

#endif	/* CXBRLDOCTYPE_H */

