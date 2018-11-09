/* 
 * File:   IFunctions.h
 * Author: santony
 *
 * Created on October 1, 2012, 9:38 PM
 */

#ifndef IFUNCTIONS_H
#define	IFUNCTIONS_H

#include "../secommon.h"
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "../seglobal/CDate.h"
#include <jsoncpp/json/json.h>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>


using namespace se;
using namespace boost;
using namespace boost::posix_time;
using namespace Json;

#define MINUTERANGE     1

using namespace std;

struct IFunction
{
    virtual ErrorId Execute() = 0;
    virtual void Interrupt() = 0;
};

typedef enum
{
    FREQ_Daily,
    FREQ_Continuous,
    FREQ_Weekly,
    FREQ_Monthly,
    FREQ_Once,
    FREQ_Static     // static instance - an always running instance not decided by timers.
} FREQ;

struct Interval
{
    FREQ Frequency;
    string Factor;
};

struct CTimedFunction
{
private:
    FREQ _intervalFreq;
    Value _config;
    time_t _clk;
    
protected:
    bool _bIsBusy;
    
public:
    
    CTimedFunction();
    
    void SetInterval(const Value& config);
        
    virtual ~CTimedFunction();
    
    bool HasExpired();
    
    bool IsBusy();
    
    void SetBusy();

    void UnsetBusy();
    
    FREQ GetFreq();
};

struct BoolCompletion
{
    bool& bReference;
    
    BoolCompletion(bool& ref) : bReference(ref)
    {
    }
    ~BoolCompletion()
    {
        bReference = false;
    }
};


#endif	/* IFUNCTIONS_H */

