/* 
 * File:   CCountry.cpp
 * Author: santony
 * 
 * Created on January 1, 2014, 3:44 PM
 */

#include "internal.h"
#include "CCountry.h"

namespace se
{
    namespace admin
    {

        CCountry::CCountry(ISession* ps) : CObjectRoot(ps)
        {
        }

        CCountry::~CCountry() 
        {
            
        }

        long unsigned int CCountry::Init(
                I16 id, const string& name, const string& threecharcode, const string& twocharcode, bool isppavailable) 
        {
            set_Id(id);
            _name = name;
            _threecode = threecharcode;
            _twocode = twocharcode;
            _bisppavailable = isppavailable;
            
            return S_Success;

        }
        

        const char* CCountry::get_Name() 
        {
            return _name.c_str();
        }

        const char* CCountry::get_ThreeCharCode() {
            return _threecode.c_str();
        }

        const char* CCountry::get_TwoCharCode() {
            return _twocode.c_str();
        }

        bool CCountry::get_IsPaypalAvailable() {
            return _bisppavailable;
        }

        void CCountry::set_Name(CSTR name) {
            _name=name;
        }

        void CCountry::set_ThreeCharCode(const string& code) 
        {
            _threecode = code;

        }

        void CCountry::set_TwoCharCode(const string& code) {
            _twocode = code;

        }

        void CCountry::set_IsPaypalAvailable(bool b) {
            _bisppavailable = b;

        }

        void CCountry::Serialize(ISerializedData<IString>* pIn) 
        {
            pIn->Add("Id", get_Id());
            pIn->Add("Name", _name.c_str());
            pIn->Add("ThreeCharCode", _threecode.c_str());
            pIn->Add("TwoCharCode", _twocode.c_str());
            pIn->Add("IsPaypalAvailable", _bisppavailable);
        }
 
        
        
    }
}