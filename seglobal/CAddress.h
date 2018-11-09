/* 
 * File:   CAddress.h
 * Author: santony
 *
 * Created on September 14, 2012, 6:37 AM
 */

#ifndef CADDRESS_H
#define	CADDRESS_H

#include <string>

#include "IObject.h"

using namespace std;

namespace se
{

    class CAddress : public CObjectRoot, public IAddress
    {
    public:
        CAddress(ISession* ps);
        CAddress(const CAddress& orig);
        virtual ~CAddress();

        virtual CSTR get_Name() ;
        virtual void set_Name(CSTR) ;
        
        virtual CSTR get_Street() ;
        virtual CSTR get_Street2() ;
        virtual CSTR get_City() ;
        virtual CSTR get_State() ;
        virtual CSTR get_Country() ;
        virtual CSTR get_PostalCode();

        virtual void set_Street(CSTR value) ;
        virtual void set_Street2(CSTR value) ;
        virtual void set_City(CSTR value) ;
        virtual void set_State(CSTR value) ;
        virtual void set_Country(CSTR value) ;
        virtual void set_PostalCode(CSTR value) ;

        virtual void Serialize(ISerializedData<IString>* pIn) ;
        virtual ErrorId  Deserialize(ISerializedData<IString>* pIn);
        
        virtual bool Equals(IObject* other);


    private:

        string _sname ,
                _sStreet, 
                _sStreet2, 
                _sCity, 
                _sState, 
                _sCountry, 
                _sPostCode;

    };

}

#endif	/* CADDRESS_H */

