/* 
 * File:   CTimeStamp.h
 * Author: santony
 *
 * Created on August 7, 2013, 10:16 PM
 */

#ifndef CTIMESTAMP_H
#define	CTIMESTAMP_H

namespace se
{
    class CTimeStamp 
    {
    public:
        CTimeStamp();
        CTimeStamp(I32 year, I32 month, I32 day, I32 hour, I32 min, I32 sec, I32 ms);
        CTimeStamp(const CTimeStamp& orig);
        CTimeStamp(CTimeStamp&& orig);
        virtual ~CTimeStamp();

        I32 get_Year();
        I32 get_Month();
        I32 get_Day();
        I32 get_Hour();
        I32 get_Minute();
        I32 get_Second();
        I32 get_MillSecond();
        bool IsEmpty() ;
        
        CTimeStamp operator=(const CTimeStamp& orig);

    private:
        int _year;
        int _month;
        int _day;
        int _hour;
        int _min;
        int _sec;
        int _ms;
    };
}
#endif	/* CTIMESTAMP_H */

