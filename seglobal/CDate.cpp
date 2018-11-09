/* 
 * File:   CDate.cpp
 * Author: santony
 * 
 * Created on November 7, 2012, 2:13 PM
 */

#include "../seglobaltypedefs.h"
#include "CDate.h"
#include <boost/date_time.hpp>
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "Object.h"
#include "IObject.h"
#include "seglobalinternal.h"
#include "CQuarter.h"
#include "CString.h"
#include <ctime>
#include <chrono>
#include <boost/locale.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/date_clock_device.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <time.h>
#include <regex>

using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;
using namespace std::chrono;
using namespace boost::locale;
using namespace boost::locale::period;


namespace se
{
    CDate::CDate() : _day(0), _month(0), _year(0), _weekday(WD_Sun)
    {
    }
    
    CDate::CDate(I16 year, I16 month, I16 day): _day(day), _month(month), _year(year)
    {
        CheckDateValidity(*this);
    }

    CDate::CDate(const CDate& orig) 
    {
        _day = orig._day;
        _month = orig._month;
        _year = orig._year;
        _weekday = orig._weekday;
    }
    
    CDate::CDate(CDate&& orig)
    {
        _day = orig._day;
        _month = orig._month;
        _year = orig._year;
        _weekday = orig._weekday;
    }

    CDate::~CDate() 
    {
    }
    
    CDate& CDate::operator =(const CDate& other)
    {
        if (this != &other)
        {
            _day = other._day;
            _month = other._month;
            _year = other._year;
            _weekday = other._weekday;
        }
        return *this;
    }
    
    CDate& CDate::operator =(CDate&& other)
    {
        if (this != &other)
        {
            _day = other._day;
            _month = other._month;
            _year = other._year;
            _weekday = other._weekday;
        }
        return *this;
    }
    
    bool CDate::operator ==(const CDate& other) const
    {
        return (_day == other._day &&
                _month == other._month &&
                _year == other._year);
    }

    bool CDate::operator !=(const CDate& other) const
    {
        return !this->operator ==(other);
    }
    
    bool CDate::operator >(const CDate& other) const
    {
        if (this->_year > other._year)
            return true;
        if (this->_year == other._year)
        {
            if (this->_month > other._month)
                return true;
            if (this->_month == other._month)
                if (this->_day > other._day)
                    return true;
        }
        return false;
    }

    bool CDate::operator <(const CDate& other) const
    {
        if (this->_year < other._year)
            return true;
        if (this->_year == other._year)
        {
            if (this->_month < other._month)
                return true;
            if (this->_month == other._month)
                if (this->_day < other._day)
                    return true;
        }
        return false;
    }
    
    bool CDate::operator >=(const CDate& other) const
    {
        if (this->_year > other._year)
            return true;
        if (this->_year == other._year)
        {
            if (this->_month > other._month)
                return true;
            if (this->_month == other._month)
                if (this->_day >= other._day)
                    return true;
        }
        return false;
    }

    bool CDate::operator <=(const CDate& other) const
    {
        if (this->_year < other._year)
            return true;
        if (this->_year == other._year)
        {
            if (this->_month < other._month)
                return true;
            if (this->_month == other._month)
                if (this->_day <= other._day)
                    return true;
        }
        return false;
    }

    const U16 CDate::get_Day() const
    {
        return _day;
    }
    const U16 CDate::get_Month() const
    {
        return _month;
    }
    const U16 CDate::get_Year() const
    {
        return _year;
    }

    void CDate::set_Day(U16 day)
    {
        _day = day;
        CheckDateValidity(*this);
    }
    
    void CDate::set_Month(U16 month)
    {
        _month = month;
        CheckDateValidity(*this);
    }
    
    void CDate::set_Year(U16 year)
    {
        _year = year;
        CheckDateValidity(*this);
    }

    const int CDate::get_Quarter()  const
    {
        CQuarter q(*this);
        return q.get_Quarter();
    }

    const WeekDay CDate::get_WeekDay() const 
    {
        return _weekday;
    }
    
    const unsigned short CDate::get_LastDayOfMonth() const 
    {
        boost::gregorian::date d(_year, _month, _day);
        return (U16)d.end_of_month().day();
    }

    const unsigned short CDate::get_MidDayOfMonth() const 
    {
        U16 lastday = get_LastDayOfMonth();
        return (U16)lastday / 2;
    }
    

    void CheckDateValidity(CDate& date)
    {
        if (date._day > 0 && date._year > 0 && date._month > 0)
        {
                // what is the last day of month for the year ?
            boost::gregorian::date d(date._year, date._month, 1);
            date._day = std::min(date._day,  (U16)d.end_of_month().day() );
            boost::gregorian::date d1(date._year, date._month, date._day);
            date._weekday = (WD)(int)d1.day_of_week();
        }
    }
    
    CDate& CDate::operator-=(const int duration)
    {
        boost::gregorian::date dt(_year, _month, _day);
        boost::gregorian::date_duration dur(duration);
        dt -= dur;
        _year = dt.year();
        _month = dt.month();
        _day = dt.day();
        _weekday = (WD)(int)dt.day_of_week();
        return *this;
    }
    
    CDate& CDate::operator+=(const int duration)
    {
        boost::gregorian::date dt(_year, _month, _day);
        boost::gregorian::date_duration dur(duration);
        dt += dur;
        _year = dt.year();
        _month = dt.month();
        _day = dt.day();
        _weekday = (WD)(int)dt.day_of_week();
        return *this;
    }
    
    DATAPERIOD CDate::operator -(const CDate& dt) 
    {
        return get_Duration(dt);
    }


    DATAPERIOD CDate::get_Duration(const CDate& d)
    {
        boost::gregorian::date dtlo(_year, _month, _day);
        boost::gregorian::date dthi(d._year, d._month, d._day);
        if (dtlo > dthi)
        {
            boost::gregorian::date dttmp = dtlo;
            dtlo = dthi;
            dthi = dttmp;
        }
        date_duration dur = dthi - dtlo;
        DATAPERIOD dt;
        dt.Day = dur.days();
        dt.Year = dthi.year() - dtlo.year();
        dt.Month = dt.Year * 12 + (dthi.month() - dtlo.month());
        dt.Quarter = dt.Month / 3 ;

        // FOR FUTURE REFERENCE
        /*
        for (day_iterator it = dtlo; it != dthi; ++it)
        {
            if (it->day_of_week() != boost::date_time::Saturday && 
                    it->day_of_week() != boost::date_time::Sunday)
        }
        */
        
        dt.Week = dt.Day / 7;
        
        return dt;
    }

    CDate CDate::AddSubDays(int days) 
    {
        boost::gregorian::date dt(_year, _month, _day);
        dt += boost::gregorian::days(days);
        CDate dtt(dt.year(), dt.month(), dt.day());
        return dtt;
    }

    CDate CDate::AddSubQuarters(int quarter) 
    {
        return AddSubMonths(quarter * 3);
    }
    

    CDate CDate::AddSubMonths(int months) 
    {
        boost::gregorian::date dt(_year, _month, _day);
        dt += boost::gregorian::months(months);
        CDate dtt(dt.year(), dt.month(), dt.day());
        return dtt;
    }

    CDate CDate::AddSubYears(int years) 
    {
        boost::gregorian::date dt(_year, _month, _day);
        dt += boost::gregorian::years(years);
        CDate dtt(dt.year(), dt.month(), dt.day());
        return dtt;
    }

    
    
    /*
     * const std::locale formats[] = {
std::locale(std::locale::classic(),new bt::time_input_facet("%Y-%m-%d %H:%M:%S")),
std::locale(std::locale::classic(),new bt::time_input_facet("%Y/%m/%d %H:%M:%S")),
std::locale(std::locale::classic(),new bt::time_input_facet("%d.%m.%Y %H:%M:%S")),
std::locale(std::locale::classic(),new bt::time_input_facet("%Y-%m-%d"))};
const size_t formats_n = sizeof(formats)/sizeof(formats[0]);
     */
    
    CDate CDate::Parse(CSTR datestr)
    {
        date d(from_simple_string(datestr));        
        CDate dt(d.year(), d.month(), d.day());
        return std::move(dt);
    }
    
    CDate CDate::Parse(CSTR dateasstring, CSTR format)
    {
        string sdate = dateasstring;
        const std::locale fct(std::locale::classic(), new boost::posix_time::time_input_facet(format));           
        std::istringstream is(dateasstring);
        is.imbue(fct);
        boost::posix_time::ptime date;
        is >> date;
        CDate dt(date.date().year(), date.date().month(), date.date().day()) ;
        return dt;
    } 

    bool CDate::IsToday() const
    {
        return *this == Now();
    }

    
    CDate CDate::Now()
    {
        const date& d = day_clock::local_day();
        CDate dt(d.year(), d.month(), d.day());
        return std::move(dt);
    }
    
    CDate CDate::Empty()
    {
        return std::move(CDate());
    }
    
    bool CDate::IsEmpty() const
    {
        return _day == 0 &&
                _month == 0 &&
                _year == 0;
    }
    
    bool CDate::IsInvalid() const
    {
        if (_day < 1 || _day > 31 || _month < 1 || _month > 12 || _year < 1900)
            return true;
        return false;
    }
    
    CDate::CDate(unsigned long num)
    {
        _day = num & 0xF;
        _month = (num >> 4) & 0xF;
        _year = (num >> 8) & 0xF;
    }
    
    U64 CDate::ToNumber()
    {
        return (U64)(_year << 8 | _month << 4 | _day);
    }

    DATAPERIOD CDate::ToPeriodForm() const 
    {
        DATAPERIOD dp;
        dp.Year = _year;
        dp.Month = _month;
        dp.Quarter = get_Quarter();
        dp.Day = _day;
        return dp;
    }

    CDate CDate::From(const DATAPERIOD& dp) 
    {
        return CDate(dp.Year, dp.Month, dp.Day);
    }

    
    SE_CODE CDate::ToString(IString** ppOut) const
    {
        stringstream strm;
        strm << _year << "-" 
                << setw(2) 
                << setfill('0')
                << _month << "-" 
                << setw(2) 
                << setfill('0')
                << _day;
        return IString::Create(strm.str().c_str(), ppOut);
    }

    SE_CODE  CDate::AsDayString(se::IString** ppOut) const
    {
        struct tm* mktm;
        time_t r;
        time(&r);
        mktm = localtime(&r);
        mktm->tm_mday = _day;
        mktm->tm_mon = _month -1;
        mktm->tm_year = _year - 1900;
        const int size = 80;
        char buffer[size];
        size_t sz = strftime(buffer, size, "%A", mktm);
        if (!sz)
            return E_MemoryUnavailable;
        return IString::Create(buffer, ppOut);
    }

    
    /********************datetime structure in seglobaltypedefs*************/
    
    DateTime::DateTime() :
        Year(0), Month(0), Day(0), Hour(0), Minute(0), Second(0), Millisecond(0)
    {
        
    }

    DateTime::DateTime(const time_t& tm)
    {
        // local time
        struct tm* t = localtime(&tm);
        this->Day = t->tm_mday;
        this->Month = t->tm_mon + 1;
        this->Year = 1900 + t->tm_year;
        this->Hour = t->tm_hour;
        this->Minute = t->tm_min;
        this->Second = t->tm_sec;
        this->Millisecond = 0;
        IString::Create(t->tm_zone, &this->Zone);
    }
    
    DateTime::DateTime(const DateTime& cp) 
    {
        Year = cp.Year;
        Month = cp.Month;
        Day = cp.Day;
        Hour = cp.Hour;
        Minute = cp.Minute;
        Second = cp.Second;
        Millisecond = cp.Millisecond;
        Zone.Assign(cp.Zone);
    }

    DateTime::DateTime(DateTime&& mv)
    {
        Year = mv.Year;
        Month = mv.Month;
        Day = mv.Day;
        Hour = mv.Hour;
        Minute = mv.Minute;
        Second = mv.Second;
        Millisecond = mv.Millisecond;
        Zone.Assign(mv.Zone);
        
    }
    
    DateTime& DateTime::operator =(const DateTime& cp)
    {
        Year = cp.Year;
        Month = cp.Month;
        Day = cp.Day;
        Hour = cp.Hour;
        Minute = cp.Minute;
        Second = cp.Second;
        Millisecond = cp.Millisecond;
        Zone.Assign(cp.Zone);
        return *this;
    }
    
    DateTime& DateTime::operator =(DateTime&& mv)
    {
        Year = mv.Year;
        Month = mv.Month;
        Day = mv.Day;
        Hour = mv.Hour;
        Minute = mv.Minute;
        Second = mv.Second;
        Millisecond = mv.Millisecond;
        Zone.Assign(mv.Zone);
        return *this;
    }
    
    bool DateTime::IsEmpty()
    {
        return Year == 0 &&
                Month == 0 && 
                Day == 0 &&
                Hour == 0 &&
                Minute == 0 &&
                Second == 0 &&
                Millisecond == 0;
    }
    
 
    DateTime& DateTime::operator =(const time_t& tm)
    {
        // local time
        struct tm* t = localtime(&tm);
        this->Day = t->tm_mday;
        this->Month = t->tm_mon + 1;
        this->Year = 1900 + t->tm_year;
        this->Hour = t->tm_hour;
        this->Minute = t->tm_min;
        this->Second = t->tm_sec;
        this->Millisecond = 0;
        IString::Create(t->tm_zone, &this->Zone);
        return *this;
    }
    
    ErrorId DateTime::ToString(IString** ppOut)
    {
        stringstream s;
        s << setw(4) << Year << "-"
                << setw(2) << setfill('0') << Month << "-"
                << setw(2) << setfill('0') << Day << " "
                << setw(2) << setfill('0') << Hour << ":"
                << setw(2) << setfill('0') << Minute << ":"
                << setw(2) << setfill('0') << Second << ".000";
        return IString::Create(s.str().c_str(), ppOut);
    }
    

    DateTime DateTime::Now()
    {
        DateTime dt;
        time_t ttt = time(0);
        tm* tt = localtime(&ttt);
        dt.Year = 1900 + tt->tm_year;
        dt.Month = tt->tm_mon + 1;
        dt.Day = tt->tm_mday;
        dt.Hour = tt->tm_hour;
        dt.Minute = tt->tm_min;
        dt.Second = tt->tm_sec;
        return dt;
    }

    DateTime DateTime::Empty() 
    {
        return DateTime();
    }

    
    std::ostream& operator<<(std::ostream& os, const DateTime& dp)
    {
        os  << setw(4) << dp.Year << "-"
            << setw(2) << setfill('0') << dp.Month << "-"
            << setw(2) << setfill('0') << dp.Day << " "
            << setw(2) << setfill('0') << dp.Hour << ":"
            << setw(2) << setfill('0') << dp.Minute << ":"
            << setw(2) << setfill('0') << dp.Second;
        return os;
    }
    
    DateTime DateTime::Parse(CSTR dateasstring, CSTR format)
    {
        if (dateasstring && strlen(dateasstring))
        {
            string sdate = dateasstring;
            const std::locale fct(std::locale::classic(), new boost::posix_time::time_input_facet(format));           
            std::istringstream is(dateasstring);
            is.imbue(fct);
            boost::posix_time::ptime date;
            is >> date;
            DateTime dt ;
            dt.Day = date.date().day();
            dt.Month = date.date().month();
            dt.Year = date.date().year();
            dt.Hour = date.time_of_day().hours();
            if (sdate.find("PM")!=string::npos)
                dt.Hour = dt.Hour + 12;
            dt.Minute = date.time_of_day().minutes();
            dt.Second = date.time_of_day().seconds();
            dt.Millisecond = date.time_of_day().fractional_seconds();
            IString::Create(date.zone_abbrev(true).c_str(), &dt.Zone);
            return dt;
        }
        else 
            return DateTime::Empty();
    }
    
    
    // tagYearQuarterMonth;
    bool tagYearQuarterMonth::operator==(const tagYearQuarterMonth& r) const
    {
        return this->Day == r.Day &&
                this->Month == r.Month &&
                this->Year == r.Year &&
                this->Quarter == r.Quarter;
    }
    
    bool tagYearQuarterMonth::operator!=(const tagYearQuarterMonth& r) const
    {
        return !operator ==(r);
    }    
    

    bool tagYearQuarterMonth::operator>(const tagYearQuarterMonth& r) const
    {
        if (Quarter && Year)
        {
            U64 left = ((U64)Year << 32) | Quarter;
            U64 right = ((U64)r.Year << 32) | r.Quarter;
            bool ret = left > right;
            return ret;
            
        }
        else if (Month && Day && Year)
        {
            U64 left = ((U64)Year << 32) | Month << 16 | Day;
            U64 right = ((U64)r.Year << 32) | r.Month << 16 | r.Day;
            bool ret = left > right;
            return ret;
            
        }
        else if (Month && Year)
        {
            U64 left = ((U64)Year << 32) | Month;
            U64 right = ((U64)r.Year << 32) | r.Month;
            bool ret = left > right;
            return ret;
            
        }
        else
            return Year > r.Year;
        return false;
    }
    
    bool tagYearQuarterMonth::operator<(const tagYearQuarterMonth& r) const    
    {
        if (Quarter && Year)
        {
            U64 left = ((U64)Year << 32) | Quarter;
            U64 right = ((U64)r.Year << 32) | r.Quarter;
            bool ret = left < right;
            return ret;
            
        }
        else if (Month && Day && Year)
        {
            U64 left = ((U64)Year << 32) | Month << 16 | Day;
            U64 right = ((U64)r.Year << 32) | r.Month << 16 | r.Day;
            bool ret = left < right;
            return ret;
        }
        else if (Month && Year)
        {
            U64 left = ((U64)Year << 32) | Month;
            U64 right = ((U64)r.Year << 32) | r.Month;
            bool ret = left < right;
            return ret;
        }
        else
            return Year < r.Year;
        return false;
    }

    bool tagYearQuarterMonth::operator>=(const tagYearQuarterMonth& r) const
    {
        if (Quarter && Year)
        {
            U64 left = ((U64)Year << 32) | Quarter;
            U64 right = ((U64)r.Year << 32) | r.Quarter;
            bool ret = left >= right;
            return ret;
            
        }
        else if (Month && Day && Year)
        {
            U64 left = ((U64)Year << 32) | Month << 16 | Day;
            U64 right = ((U64)r.Year << 32) | r.Month << 16 | r.Day;
            bool ret = left >= right;
            return ret;
            
        }
        else if (Month && Year)
        {
            U64 left = ((U64)Year << 32) | Month;
            U64 right = ((U64)r.Year << 32) | r.Month;
            bool ret = left >= right;
            return ret;
            
        }
        else
            return Year >= r.Year;
        return false;
    }

    bool tagYearQuarterMonth::operator<=(const tagYearQuarterMonth& r) const
    {
        if (Quarter && Year)
        {
            U64 left = ((U64)Year << 32) | Quarter;
            U64 right = ((U64)r.Year << 32) | r.Quarter;
            bool ret = left <= right;
            return ret;
            
        }
        else if (Month && Day && Year)
        {
            U64 left = ((U64)Year << 32) | Month << 16 | Day;
            U64 right = ((U64)r.Year << 32) | r.Month << 16 | r.Day;
            bool ret = left <= right;
            return ret;
            
        }
        else if (Month && Year)
        {
            U64 left = ((U64)Year << 32) | Month;
            U64 right = ((U64)r.Year << 32) | r.Month;
            bool ret = left <= right;
            return ret;
            
        }
        else
            return Year <= r.Year;
        return false;
    }
    
    void CreateDateFromStrUsingRegex(tagYearQuarterMonth* date, const char* s)
    {
        const string patt = "(\\d{4})-(\\d{2})-(\\d{2})";
        std::regex rg(patt, std::regex_constants::ECMAScript | std::regex_constants::icase);
        std::smatch m;
        std::string str = s; 
        if (std::regex_search(str, m, rg) && m.size() > 1)
        {
            date->Year = strtol(m.str(1).c_str(), nullptr, 10);
            date->Month = strtol(m.str(2).c_str(), nullptr, 10);
            date->Day = strtol(m.str(3).c_str(), nullptr, 10);
        }
    }

    tagYearQuarterMonth& tagYearQuarterMonth::operator =(const char* str)
    {
        if (str)
            CreateDateFromStrUsingRegex(this, str);
        return *this;
    }


    tagYearQuarterMonth::tagYearQuarterMonth(const char* str) 
    {
        if (str)
            CreateDateFromStrUsingRegex(this, str);
    }

    
    std::ostream& operator<<(std::ostream& os, const tagYearQuarterMonth& dp)
    {
        if (dp.Day && dp.Month)
            os << dp.Year << "/" << dp.Month << "/" << dp.Day;
        else if (dp.Month)
            os << dp.Year << "/" << dp.Month;
        else if (dp.Quarter)
            os << dp.Year << "/" << dp.Quarter;
        return os;
    }
    
    std::istream& operator>>(std::istream& is, tagYearQuarterMonth& dp)
    {
        std::string str ;
        is >> str;
        if (!str.empty())
            CreateDateFromStrUsingRegex(&dp, str.c_str());
        return is;
    }
    
    std::ostream & operator << (std::ostream& os, const CDate& date)
    {
        return os << date.get_Year() << "-" << std::setw(2) << std::setfill('0') << date.get_Month() << "-"  << std::setw(2) << std::setfill('0') << date.get_Day() ;
    }
   
    
    tagYearQuarterMonth::tagYearQuarterMonth() : 
        Day(0), Month(0), Year(0), Week(0), Quarter(0)
    {
    }
    
    bool tagYearQuarterMonth::IsEmpty()
    {
        return Month == 0 && 
                Day == 0 & 
                Year == 0 && 
                Quarter == 0 &&
                Week == 0;
    }

    unsigned long tagYearQuarterMonth::ToNumber() const 
    {
        return ((U64)Year << 48 | (U64)Quarter << 32 | Month << 16 | Day);
    }

}