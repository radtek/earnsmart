/* 
 * File:   seglobaltypedefs.h
 * Author: santony
 *
 * Created on August 6, 2012, 9:03 AM
 */

#ifndef SEGLOBALTYPEDEFS_H
#define	SEGLOBALTYPEDEFS_H

#include <iostream>
#include  <limits>

namespace se
{

    #define BOOL                    bool    
    #define CWSTR                   const wchar_t*
    #define WSTR                    wchar_t*
    #define WCHR                    wchar_t
    #define CHR                     char
    #define STR                     char*
    #define CSTR                    const char*
    #define U8                      unsigned char
    #define I8                      char
    #define U16                     unsigned short
    #define I16                     short
    #define U32                     unsigned int
    #define I32                     signed int
    #define U64                     unsigned long
    #define I64                     signed long
    #define R8                      double
    #define R4                      float
    #define UID                     U64
    //#ifdef _LP64
    #define PSIZE                   __SIZE_TYPE__
    #define SIZE                    __SIZE_TYPE__
    #define POINTER                 void*
    #define CPOINTER                const void*
    typedef  I16                     EXCHANGEID;
    typedef  I16                     COUNTRYID;
    typedef  I16                     CURRENCYID;

    typedef CSTR                    CIK;
    typedef CSTR                    SYM;
    typedef UID                     ICIK;

    typedef void*                    LPVOID;

    #define ErrorId                 __SIZE_TYPE__
    #define SE_CODE                 ErrorId

    #define ER8             std::numeric_limits<R8>::max()
    #define MINR8           std::numeric_limits<R8>::min()
    #define MAXR8           std::numeric_limits<R8>::max()
    #define ISER8(d) (d == ER8 )


    typedef enum enDataTypes
    {
        DT_BOOL = 1,
        DT_CWSTR = 2,
        DT_WSTR = 3,
        DT_WCHR = 4,
        DT_CHR = 5,
        DT_STR = 6,
        DT_CSTR = 7,
        DT_U32 = 8,
        DT_I32 = 9,
        DT_U64 = 10,
        DT_I64 = 11,
        DT_R8 = 12,
        DT_R4 = 13,
        DT_DATETIME = 14,
        DT_I16 = 15,
        DT_U16 = 16,
        DT_I8 = 17,
        DT_U8 = 18,
        DT_POINTER = 19,
        DT_OBJECT = 20,
        DT_NONE = 0,
        DT_ERROR = 128
    } DATATYPES;

   
    typedef struct tagYearQuarterMonth
    {
        int Month;
        int Quarter;    
        int Year;
        int Day;
        int Week;
        
        bool operator==(const tagYearQuarterMonth& r) const;
        bool operator!=(const tagYearQuarterMonth& r) const;
        bool operator>(const tagYearQuarterMonth& r) const;
        bool operator<(const tagYearQuarterMonth& r) const;
        bool operator>=(const tagYearQuarterMonth& r) const;
        bool operator<=(const tagYearQuarterMonth& r) const;
        friend std::ostream& operator<<(std::ostream& os, const tagYearQuarterMonth& dp);
        friend std::istream& operator>>(std::istream& is, tagYearQuarterMonth& dp);
        friend void operator>>(std::string& str, tagYearQuarterMonth& dp);
        
        tagYearQuarterMonth();
        
        bool IsEmpty();
        
        U64 ToNumber() const;
        
        tagYearQuarterMonth(CSTR str);
        tagYearQuarterMonth& operator=(CSTR str);
        
    } MONTHYEAR, YEARQUARTER, DATAPERIOD;

    typedef enum tagPeriods
    {
        PERIOD_Monthly = 0,
        PERIOD_SemiAnnually = 1,
        PERIOD_Annually = 2,
        PERIOD_Quarterly = 3,
        PERIOD_Daily = 4,
        PERIOD_Hourly = 5,
        PERIOD_Weekly = 6,
        PERIOD_Continuous = 0xFE,
        PERIOD_None = 0xFF
    } PERIOD;
    
    extern YEARQUARTER ALLPERIODS;
    
    typedef enum class tagFiscalPeriods
    {
        Q1 = 1,
        Q2 = 2,
        Q3 = 3,
        Q4 = 4,
        FY = 4,
        HY = 6
    } FISCALPERIODS, FPRDS;
    
    // econmic adjustment masks used by some methods.
    typedef enum tagEconomicAdjMasks
    {
        // make no adjustments.
        EAM_None = 0x0,
        // make periodic inflationary adjustments.
        EAM_Inflation = 0x1, 
        EAM_CPIInflation = EAM_Inflation,
        EAM_PPIInflation = 0x2,
    } ECONOMICADJMASKS, EAM;
    
    
    typedef enum tagLogics
    {
        LGX_None,
        LGX_And,
        LGX_Or,
        LGX_Xor,
        LGX_Not,
    } LGX;
    
    
    typedef struct _typeCheck
    {
        static BOOL IsNumber(CSTR s);
    } TYPECHECK;
    
    // The market data providers. The default provider
    // is used by ISecurityAPI when it is created (see SecOpen method in this file).
    // Occassionaly some methods may as for a specific  market data provider if
    // default market data provider is not able support providing data used in the method.
    typedef enum class enMarketDataProviders
    {
        NONE = 1, 
        EodData = 2,  // Eoddata provider.

    } MARKETDATAPROVIDERS, MDP;
    
}



#endif	/* SEGLOBALTYPEDEFS_H */

