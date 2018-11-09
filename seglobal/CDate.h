/* 
 * File:   CDate.h
 * Author: santony
 *
 * Created on November 7, 2012, 2:13 PM
 */

#ifndef CDATE_H
#define	CDATE_H

#include "../seglobaltypedefs.h"
#include <ostream>

namespace se
{
    struct IString;
    
    typedef enum WeekDay
    {
        WD_Sun = 0,
        WD_Mon = 1,
        WD_Tue = 2,
        WD_Wed = 3,
        WD_Thu = 4,
        WD_Fri = 5,
        WD_Sat = 6
    } WD;

    class CDate 
    {
    public:
        CDate();
        CDate(I16 year, I16 month, I16 day);
        CDate(const CDate& orig);
        CDate(CDate&& orig);
        CDate(U64 num);
        
        ~CDate();
        
        CDate& operator=(const CDate& other);
        CDate& operator=(CDate&& other);
        bool operator==(const CDate& other) const;
        bool operator!=(const CDate& other) const;
        bool operator >(const CDate& other) const;
        bool operator <(const CDate& other) const;
        bool operator >=(const CDate& other) const;
        bool operator <=(const CDate& other) const;
    
        CDate& operator-=(const int duration);
        CDate& operator+=(const int duration);
        DATAPERIOD operator-(const CDate&);
        DATAPERIOD get_Duration(const CDate&);
        
        CDate AddSubQuarters(int quarter);      // use + or -ve values
        CDate AddSubMonths(int months);        // use + or -ve values
        CDate AddSubYears(int years);          // use + or - values
        CDate AddSubDays(int days);
    
        const int get_Quarter() const;
        
        const U16 get_Day() const;
        const U16 get_Month() const;
        const U16 get_Year() const;
        
        const WeekDay get_WeekDay() const;
        const U16 get_LastDayOfMonth() const;
        const U16 get_MidDayOfMonth() const;
        
        void set_Day(U16 day);
        void set_Month(U16 month);
        void set_Year(U16 year);
        
        static CDate Parse(CSTR datestr);
        static CDate Parse(CSTR dateasstring, CSTR format);
        static CDate Now();
        
        static CDate Empty();
        
        bool IsEmpty() const;
        bool IsInvalid() const;
        bool IsToday() const;
        
        // returns an iso date (YYYY-MM-DD)
        SE_CODE ToString(se::IString** ppOut) const; 
        SE_CODE AsDayString(se::IString** ppOut) const;
        U64 ToNumber();
        DATAPERIOD ToPeriodForm() const;
        static CDate From(const DATAPERIOD& dp);
        // output stream operator overload
        friend std::ostream& operator << (std::ostream& os, const CDate& date);
        friend void CheckDateValidity(CDate& date);
        
    private:

        U16 _day, _month, _year;
        WeekDay _weekday;
    };
    
    
    std::ostream & operator << (std::ostream& os, const CDate& date);

}

#endif	/* CDATE_H */

