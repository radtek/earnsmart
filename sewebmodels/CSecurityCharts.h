/* 
 * File:   CSecurityCharts.h
 * Author: santony
 *
 * Created on October 20, 2015, 10:27 PM
 */

#ifndef CSECURITYCHARTS_H
#define	CSECURITYCHARTS_H

#include "CMaster.h"

class CSecurityCharts : public CMaster
{
public:
    CSecurityCharts();
    CSecurityCharts(const CSecurityCharts& orig) = delete;
    virtual ~CSecurityCharts();
    

    virtual void set_Title(const string& title);
    

private:

};

#endif	/* CSECURITYCHARTS_H */

