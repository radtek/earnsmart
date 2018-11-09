/* 
 * File:   CTimeStamp.cpp
 * Author: santony
 * 
 * Created on August 7, 2013, 10:16 PM
 */

#include "../seglobaltypedefs.h"
#include "CTimeStamp.h"

namespace se
{
    
    CTimeStamp::CTimeStamp():
        _year(0), _month(0), _day(0), _hour(0),
            _min(0), _sec(0), _ms(0)
    {
    
    }

    CTimeStamp::CTimeStamp(I32 year, I32 month, I32 day, I32 hour, I32 min, I32 sec, I32 ms) :
        _year(year), _month(month), _day(day), _hour(hour),
            _min(min), _sec(sec), _ms(ms)
    {
    }

    CTimeStamp::CTimeStamp(const CTimeStamp& orig) 
    {
        _year = orig._year;
        _month = orig._month;
        _day =orig._day;
        _hour = orig._hour;
        _min = orig._min;
        _sec = orig._sec;
        _ms = orig._ms;
    }
    
    CTimeStamp::CTimeStamp(CTimeStamp&& orig)
    {
        _year = orig._year;
        _month = orig._month;
        _day =orig._day;
        _hour = orig._hour;
        _min = orig._min;
        _sec = orig._sec;
        _ms = orig._ms;
    }
    
    CTimeStamp CTimeStamp::operator=(const CTimeStamp& orig)
    {
        _year = orig._year;
        _month = orig._month;
        _day =orig._day;
        _hour = orig._hour;
        _min = orig._min;
        _sec = orig._sec;
        _ms = orig._ms;
    }

    CTimeStamp::~CTimeStamp() 
    {
    }
    
    I32 CTimeStamp::get_Year()
    {
        return _year;
    }
    
    I32 CTimeStamp::get_Month()
    {
        return _month;
    }
    
    I32 CTimeStamp::get_Day()
    {
        return _day;
    }
    
    I32 CTimeStamp::get_Hour()
    {
        return _hour;
    }
    
    I32 CTimeStamp::get_Minute()
    {
        return _min;
    }
    
    I32 CTimeStamp::get_Second()
    {
        return _sec;
    }
    
    I32 CTimeStamp::get_MillSecond()
    {
        return _ms;
    }
    
    bool CTimeStamp::IsEmpty()
    {
        return _year == 0 && 
                _month == 0 &&
                _day == 0 &&
                _hour == 0 &&
                _min == 0 && 
                _sec == 0 && 
                _ms == 0;
    }

}