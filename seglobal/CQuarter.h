/* 
 * File:   CQuarter.h
 * Author: santony
 *
 * Created on November 7, 2012, 2:04 PM
 */

#ifndef CQUARTER_H
#define	CQUARTER_H

#include "CDate.h"

/*
 * Accept current date and returns its quarterly position in 
 * a year (such as Quarter 1, 2, 3 or 4, first and last days of the
 * quarter. Also returns other information of the quarter.
 */
namespace se
{

    class CQuarter 
    {
    public:
        CQuarter();
        CQuarter(const CDate& date);
        CQuarter(const CQuarter& orig);
        ~CQuarter();

        U16 get_Quarter() const ;
        CDate& get_StartDate() ;
        CDate & get_EndDate() ;

        void SetDate(const CDate& dt);

    private:
        void SetQuarterInfo();

    private:
        CDate _date;
        CDate _sdate;
        CDate _edate;
        U16 _quarter;
    };

}

#endif	/* CQUARTER_H */

