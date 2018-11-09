/* 
 * File:   CSecurityExchange.cpp
 * Author: santony
 * 
 * Created on March 28, 2014, 10:23 PM
 */

#include "CSecurityExchange.h"
#include "Object.h"

namespace se
{
    namespace securities
    {
        CSecurityExchange::CSecurityExchange(ISession* ps):CIdName(ps)
        {
            
        }

   

        CSecurityExchange::~CSecurityExchange() 
        {
        }

        const char* CSecurityExchange::get_Code() {
            return _code.c_str();
        }

        const char* CSecurityExchange::get_Country() {
            return _country.c_str();
        }

        
        const char* CSecurityExchange::get_TimeZone() {
            return _timezone.c_str();
        }

        void CSecurityExchange::set_Code(const char* code) {
            _code = code;
        }

        void CSecurityExchange::set_Country(const char* country)
        {
            _country = country;
        }

        void CSecurityExchange::set_TimeZone(const char* zone) 
        {
            _timezone = zone;
        }
        
        SE_CODE GBLEXP IExchange::Create(ISession* ps, IExchange** ppOut)
        {
            return CObject<CSecurityExchange>::Create(ps, ppOut);
        }
        
    }
}

