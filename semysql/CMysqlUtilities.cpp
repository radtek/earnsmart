/* 
 * File:   CMyqlUtilities.cpp
 * Author: santony
 * 
 * Created on September 4, 2013, 6:34 AM
 */

#include "../seglobaltypedefs.h"
#include "CMyqlUtilities.h"

namespace se
{
    namespace data
    {
        void CMyqlUtilities::PopulateDateTime(sql_datetime const& indt, se::DATETIME & outdt )
        {
            outdt.Year = indt.year();
            outdt.Month = indt.month();
            outdt.Day = indt.day();
            outdt.Hour = indt.hour();
            outdt.Minute = indt.minute();
            outdt.Second = indt.second();
        }

        void CMyqlUtilities::PopulateDateTime(se::DATETIME const& indt, sql_datetime & outdt)
        {
            outdt.year(indt.Year);
            outdt.month(indt.Month);
            outdt.day(indt.Day);
            outdt.hour(indt.Hour);
            outdt.minute(indt.Minute);
            outdt.second(indt.Second);
        }
        void CMyqlUtilities::PopulateDateTime(sql_datetime const& indt, se::CTimeStamp & outdt )
        {
            outdt = CTimeStamp
                    (
                        (I32)indt.year(), (I32)indt.month(), (I32)indt.day(),
                        (I32)indt.hour(), (I32)indt.minute(), (I32)indt.second(), 0
                    );
        }

        void CMyqlUtilities::PopulateDateTime(se::CTimeStamp & indt, sql_datetime & outdt)
        {
            outdt.year((I16)indt.get_Year());
            outdt.month((I16)indt.get_Month());
            outdt.day((I16)indt.get_Day());
            outdt.hour((I16)indt.get_Hour());
            outdt.minute((I16)indt.get_Minute());
            outdt.second((I16)indt.get_Second());
        }
        
        void CMyqlUtilities::PopulateDateTime(se::CDate& indt, sql_date& outdt)
        {
            outdt.year(indt.get_Year());
            outdt.month(indt.get_Month());
            outdt.day(indt.get_Month());
        }
        void CMyqlUtilities::PopulateDateTime(sql_date& indt, se::CDate& outdt)
        {
     
            outdt.set_Year(indt.year());
            outdt.set_Month(indt.month());
            outdt.set_Day(indt.day());
        }
    }
}