/* 
 * File:   CQuarter.cpp
 * Author: santony
 * 
 * Created on November 7, 2012, 2:04 PM
 */

#include "../seglobaltypedefs.h"
#include "CQuarter.h"

#include <boost/date_time/gregorian/gregorian.hpp>

namespace se
{
#define FISCALSTARTMONTH    1

    CQuarter::CQuarter()
    {
    }
    
    CQuarter::CQuarter(const CDate& date)
    {
        _date = date;
        SetQuarterInfo();
    }

    CQuarter::CQuarter(const CQuarter& orig) 
    {
        _date = orig._date;
        SetQuarterInfo();
    }

    CQuarter::~CQuarter() 
    {
        
    }

    void CQuarter::SetDate(const CDate& dt) 
    {
        _date = dt;
        SetQuarterInfo();
    }

    
    void CQuarter::SetQuarterInfo()
    {
        /* below code assumes jan 1 as fiscal start month. 
         for other fiscal start months, we need an alternative calculation.*/
        auto month = _date.get_Month();

        // = ( (M-1) - MOD((M -1 ), 3) )/3 + 1
        
        auto m0 = month-1;
        _quarter = ( m0 - (m0 % 3) ) / 3 + 1;
        
        using namespace boost::gregorian;
        auto mod3 = m0 % 3;
        _sdate.set_Day(1);
        _sdate.set_Month(month - mod3);
        _sdate.set_Year(_date.get_Year());
        
        
        I16 endday = gregorian_calendar::end_of_month_day(_date.get_Year(), _sdate.get_Month() + 2 );
        _edate.set_Day(endday);
        _edate.set_Month(_sdate.get_Month() + 2);
        _edate.set_Year(_date.get_Year());
        
    }
    
    // TODOS: number of days and weeks in the quarter
    
    U16 CQuarter::get_Quarter() const 
    {
        return _quarter;
    }
    
    CDate & CQuarter::get_StartDate() 
    {
        return _sdate;
    }
    
    CDate& CQuarter::get_EndDate() 
    {
        return _edate;
    }
}