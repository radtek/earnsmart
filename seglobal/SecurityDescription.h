#ifndef SECURITYDESCRIPTION_H
#define SECURITYDESCRIPTION_H

#include <string>

#include "../seglobal/CList.h"
#include "../seglobal/IObject.h"

using namespace se;
using namespace se::securities;
using namespace std;

class CSecurityDescription : 
    public CObjectRoot, 
    public se::securities::ISecurityDescription
{
    public:
        CSecurityDescription(ISession*);
        virtual ~CSecurityDescription();
        

        virtual unsigned long get_Id();

        
        virtual UID get_ESid();
        virtual CSTR get_Symbol() ;
        virtual CSTR get_ExchCode() ;
        virtual CSTR get_Currency() ;
        virtual CSTR get_Country();
        virtual SECTYPES get_Type();        // security type.
        

        virtual void set_ESid(unsigned long esid);

        virtual void set_Symbol(CSTR symbol) ;
        virtual void set_ExchCode(CSTR exchCode) ;
        virtual void set_Currency(CSTR currency) ;
        virtual void set_Country(CSTR country);
        virtual void set_Type(SECTYPES typeId);
        
        virtual void Serialize(ISerializedData<IString>* pIn);
        
    protected:
        
    private:
        SECTYPES _type = SECTYPES::Equity;
        UID _esid = 0;
        string _sSymbol;
        string _excode;
        string _currency;
        string _country ;
};

class CSecurityDescriptionList : public CSPList<ISecurityDescription, ISecurityDescriptionList>
{
public:
    CSecurityDescriptionList(ISession*);
    virtual ~CSecurityDescriptionList();
    void Serialize(ISerializedData<IString>* pIn) override;

};

#endif // SECURITYDESCRIPTION_H
