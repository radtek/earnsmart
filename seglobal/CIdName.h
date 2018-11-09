/* 
 * File:   CIdName.h
 * Author: santony
 *
 * Created on January 1, 2014, 3:17 PM
 */

#ifndef CIDNAME_H
#define	CIDNAME_H

#include "CObjectRoot.h"
#include "IObject.h"
#include <string>

using namespace std;

namespace se
{

    template<typename T = IIdName>
    class CIdName : public CObjectRoot, public T
    {
    public:
        CIdName(ISession* ps):CObjectRoot(ps){}
        CIdName(const CIdName& orig) = delete;
        virtual ~CIdName() {};
        

        virtual long unsigned int Init(){return S_OK;}

        
        virtual ErrorId Init(CSTR name)
        {
            _name = name;
            return S_OK;
        }

        virtual ErrorId Init(UID id, CSTR name)
        {
            _set_Id(id);
            _name = name;
            return S_OK;
        }
        
        virtual CSTR get_Name() 
        {
            return _name.c_str();
        }

        virtual void set_Name(CSTR name)
        {
            _name = name;
        }

        virtual void Serialize(ISerializedData<IString>* pIn)
        {
            pIn->Add("Id", _get_Id());
            pIn->Add("Name", get_Name());
        }

    private:
        string _name;

    };

}
#endif	/* CIDNAME_H */

