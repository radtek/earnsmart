/* 
 * File:   CCountryState.cpp
 * Author: santony
 * 
 * Created on January 2, 2014, 2:39 PM
 */

#include "internal.h"
#include "CCountryState.h"
#include "CCountry.h"
#include <sstream>

using namespace std;

namespace se
{
    namespace admin
    {

        CCountryState::CCountryState(ISession* ps) : CObjectRoot(ps), _cid(0)
        {
        }


        CCountryState::~CCountryState() {
        }

        ErrorId CCountryState::Init(unsigned long id, const string& name, const string& code, short countryId)
        {
            set_Id(id);
            _name = name;
            _code = code;
            _cid = countryId;
            return S_Success;
        }
        
        CSTR CCountryState::get_Code()
        {
            return _code.c_str();
        }
        
        CSTR CCountryState::get_Name()
        {
            return _name.c_str();
        }
        
        ErrorId CCountryState::get_Country(ICountry** ppOut)
        {
            if (!_cid)
                return E_NotFound;
                        
            SP<IAdminAPI> papi;
            RETURNIFFAILED(AdminOpen(get_Session(), &papi))
            return papi->get_Country(ppOut, [this](ICountry::QUERYFLDS fld, LGX& lgx, BOOL& include)
            {
                if (fld == ICountry::QUERYFLDS::Id)
                {
                    include = true;
                    stringstream str;
                    str << _cid;
                    return str.str().c_str();
                }
            });
            
        }
        
        void CCountryState::set_Name(const char* name)
        {
            _name = name;
        }
        
        void CCountryState::set_Code(const string& v)
        {
            _code = v;
        }
        
        void CCountryState::set_CountryId(short v)
        {
            _cid = v;
        }
        
        void CCountryState::Serialize(ISerializedData<IString>* pIn)
        {
            pIn->Add("Id", get_Id());
            pIn->Add("Name", _name.c_str());
            pIn->Add("Code", _code.c_str());
            SP<ICountry> pc;
            if (_cid)
            {
                get_Country(&pc);
                if (pc)
                    pIn->Add("Country", pc);
            }
        }
    }
}