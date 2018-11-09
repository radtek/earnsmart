/* 
 * File:   CFinancialItem.h
 * Author: santony
 *
 * Created on January 27, 2014, 7:48 PM
 */

#ifndef CFINANCIALITEM_H
#define	CFINANCIALITEM_H

#include <string>
#include "../seglobal/IObject.h"
#include <vector>
#include "../seglobal/exceptions.h"
#include "sexbrlapi.h"
#include "../seglobal/CROList.h"
#include <unordered_map>

using namespace std;
using namespace se;
using namespace se::xbrl;

typedef std::array<std::array<OmniType, 4>, 3> arr;
typedef map<int, arr> NODEVALUES;

class CFinancialsConsolidated;
class CXbrlRecord;

template<typename TValueType = double>
struct FinItemNode
{
    friend class CFinancialsConsolidated;
    friend class CXbrlRecord;
    
//    enum HALFYEAR
//    {
//        FirstHalf,
//        SecondHalf
//    };
    
    enum Duration
    {
        FirstQuarter                = 1, 
        FirstHalf                   = 2,
        FirstThreeQuarters          = 3,
        FullYear                    = 4
    };

public:
    UID XbrlId;
    string XbrlName;
    I16 FloorYear;
    I16 CeilYear;
    I16 LowQuarter;
    I16 HighQuarter;
    XPT PeriodType;
    bool IsProgression;     // set to true if the numbers for each quarter is a sum of previous quarter and real value of current quarter. In this case the last quarter value will be equal to full year value.
    IFinancialsConsolidated* pConsolidated;

private:
    typedef map<UID, FinItemNode<TValueType> > SEGMENTVALUES;
    NODEVALUES NodeValue;
    SEGMENTVALUES SegValue;
    
public:    
    FinItemNode() : XbrlId(0), FloorYear(2999), CeilYear(0), LowQuarter(4), HighQuarter(1), PeriodType(XPT_None), IsProgression(false)
    {
        memset(&NodeValue, sizeof(NODEVALUES), 0);
    }
    
    FinItemNode(UID xbrlId) : XbrlId(xbrlId), FloorYear(2999), CeilYear(0), LowQuarter(4), HighQuarter(1), PeriodType(XPT_None), IsProgression(false)
    {
        memset(&NodeValue, sizeof(NODEVALUES), 0);
    }
    
    FinItemNode(const FinItemNode& o) 
    {
        XbrlId = o.XbrlId;
        XbrlName = o.XbrlName;
        NodeValue = o.NodeValue;
        FloorYear = o.FloorYear;
        CeilYear = o.CeilYear;
        PeriodType = o.PeriodType;
        IsProgression = o.IsProgression;
        pConsolidated = o.pConsolidated;
        LowQuarter = o.LowQuarter;
        HighQuarter = o.HighQuarter;
    }

    FinItemNode(FinItemNode&& o) 
    {
        XbrlId = o.XbrlId;
        XbrlName = std::move(o.XbrlName);
        NodeValue = std::move(o.NodeValue);
        FloorYear = o.FloorYear;
        CeilYear = o.CeilYear;
        PeriodType = o.PeriodType;
        IsProgression = o.IsProgression;
        pConsolidated = o.pConsolidated;
        o.pConsolidated = nullptr;
        LowQuarter = o.LowQuarter;
        HighQuarter = o.HighQuarter;
    }
    
    FinItemNode& operator=(const FinItemNode& o) 
    {
        XbrlId = o.XbrlId;
        XbrlName = o.XbrlName;
        NodeValue = o.NodeValue;
        FloorYear = o.FloorYear;
        CeilYear = o.CeilYear;
        PeriodType = o.PeriodType;
        IsProgression = o.IsProgression;
        pConsolidated = o.pConsolidated;
        LowQuarter = o.LowQuarter;
        HighQuarter = o.HighQuarter;
        return *this;
    }

    FinItemNode& operator=(FinItemNode&& o) 
    {
        XbrlId = o.XbrlId;
        XbrlName = std::move(o.XbrlName);
        NodeValue = std::move(o.NodeValue);
        FloorYear = o.FloorYear;
        CeilYear = o.CeilYear;
        PeriodType = o.PeriodType;
        IsProgression = o.IsProgression;
        pConsolidated = o.pConsolidated;
        o.pConsolidated = nullptr;
        LowQuarter = o.LowQuarter;
        HighQuarter = o.HighQuarter;
        return *this;
    }
    
    // Save quarterly data.
    // params: 
    //  year between 2000 and 2999
    //  quarter between 1 and 4
    //  value    - double value.
    void SetValue(int year, int quarter, const OmniType& value)
    {
        if (year >= 2000 && year <= 2999 && quarter >=1 && quarter <= 4)
        {
            CeilYear = std::max(CeilYear, (I16)year);
            FloorYear = std::min(FloorYear, (I16)year);
            LowQuarter = std::min(LowQuarter, (I16)quarter);
            HighQuarter = std::max(HighQuarter, (I16)quarter);
            NodeValue[year % YearModulus][0][quarter-1] = value;
            return;
        }
        throw seexception(E_InvalidArg, "Invalid argument.");
    }

    // half yearly values.
    void SetValue(int year, Duration q, const OmniType& value)
    {
        if (year >= 2000 && year <= 2999)
        {
            CeilYear = std::max(CeilYear, (I16)year);
            FloorYear = std::min(FloorYear, (I16)year);
            if (q == Duration::FirstQuarter)
            {
                LowQuarter = std::min(LowQuarter, (I16)1);
                HighQuarter = std::max(HighQuarter, (I16)1);
            }
            else if (q == Duration::FirstHalf)
            {
                LowQuarter = std::min(LowQuarter, (I16)2);
                HighQuarter = std::max(HighQuarter, (I16)2);
            }
            else if (q == Duration::FirstThreeQuarters)
            {
                LowQuarter = std::min(LowQuarter, (I16)3);
                HighQuarter = std::max(HighQuarter, (I16)3);
            }
            else
            {
                LowQuarter = std::min(LowQuarter, (I16)4);
                HighQuarter = std::max(HighQuarter, (I16)4);
            }
            NodeValue[year % YearModulus][2][q-1] = value;
            return;
        }
        throw seexception(E_InvalidArg, "Invalid argument.");
    }
    
    // Save annual data.
    // params: 
    //  year between 2000 and 2999
    //  value    - double value.
    void SetValue(int year, const OmniType& value)
    {
        if (year >= 2000 && year <= 2999)
        {
            CeilYear = std::max(CeilYear, (I16)year);
            FloorYear = std::min(FloorYear, (I16)year);
            int index = year % YearModulus;
            NodeValue[index][1][0] = value;
            return;
        }
        throw seexception(E_InvalidArg, "Invalid argument.");
    }

    // Get quarterly value.
    const TValueType& GetValue(int year, int quarter)
    {
        if (year >= 2000 && year <= 2999 && quarter >=1 && quarter <= 4)
        {
            return NodeValue[year % YearModulus][0][quarter-1];
        }
        throw seexception(E_InvalidArg, "Invalid argument.");
    }

    const TValueType&  GetValue(int year, Duration q)
    {
        if (year >= 2000 && year <= 2999)
        {
            return NodeValue[year % YearModulus][2][q-1];
        }
        throw seexception(E_InvalidArg, "Invalid argument.");
    }

    // Get Annual value.
    const TValueType&  GetValue(int year)
    {
        if (year >= 2000 && year <= 2999)
        {
            return NodeValue[year % YearModulus][1][0];
        }
        throw seexception(E_InvalidArg, "Invalid argument.");
    }
    
    SE_CODE GetSegmentXbrlIds(IROList<UID> *ppOut)
    {
        return E_NotImpl;
    }
    
/*    
    I64 GetHashCode() const
    {
        I64 hash = 17;
        hash = hash * 31 + XbrlId;
        hash = hash * 31 + SegmentId;
        hash = hash * 31 + DimensionId;
        return hash;
    }
*/    
};

//struct FinItemNodeComparer
//{
//    bool operator()(const FinItemNode& left, const FinItemNode& right)
//    {
//        return left.GetHashCode() < right.GetHashCode();
//    }
//};



/////////////////////////////////CXbrlRecord///////////////////////////////////

class CXbrlRecordList;
class CXbrlRecord;

class CXbrlField : public CObjectRoot, public IXbrlField
{
public:
    
    CXbrlField(ISession*);
    virtual ~CXbrlField(){}

    virtual const OmniType&  get_FY() const;

    virtual const OmniType&  get_HY1()  const;

    virtual const OmniType&  get_F3Q() const;

    virtual const OmniType&  get_Q1() const;

    virtual const OmniType&  get_Q2() const;

    virtual const OmniType&  get_Q3() const;

    virtual const OmniType&  get_Q4() const;
    
    virtual const OmniType& get_Q(short quarter)  const;
    
    SE_CODE Init(I16 year, CXbrlRecord*);
   

    virtual void Serialize(ISerializedData<IString>* pIn);

    
private:
    FinItemNode<OmniType>* _pnode;
    I16 _year;
    CXbrlRecord* pr;
};

typedef std::unordered_map<I16, SP<IXbrlField>> YEARLYVALUES;

class CXbrlAbstractRecord : public CObjectRoot, public IXbrlAbstractRecord
{
public:
    
    CXbrlAbstractRecord(ISession*);
    
    virtual ~CXbrlAbstractRecord();
    
#ifdef __DEBUG__
    virtual void _AddRef();
    virtual void _DecRef();
#endif
    
    
    virtual const char* get_Name();

    virtual void set_Name(const char* name);
    
    virtual bool get_IsAbstract();

    virtual const char* get_XbrlLabel();

    virtual long unsigned int get_Children(IXbrlRecordList**);
    
    virtual RP<IXbrlAbstractRecord> get_Parent();
    
    virtual void set_Parent(IXbrlAbstractRecord*);
    
    void set_Children(IXbrlRecordList* pkids);
    
    long unsigned int Init(UID id, CSTR name, CSTR label);


    virtual void Serialize(ISerializedData<IString>* pIn);


private:
    SP<IXbrlRecordList> _pkids;
    string _label;
    string _name;
    IXbrlAbstractRecord* _pparent;
};

class CXbrlRecord : public CObjectRoot, public IXbrlRecord
{
    friend class CXbrlField;
    friend class CXbrlRecordList;
private:
    FinItemNode<OmniType> _node;
    string _label;
    YEARLYVALUES _yv;
    SP<IXbrlRecordList> _pkids;
    IXbrlAbstractRecord* _pparent;
    
public :
    
    CXbrlRecord(ISession*);
    virtual ~CXbrlRecord() ;

#ifdef __DEBUG__
    virtual void _AddRef();
    virtual void _DecRef();
#endif
    
    SE_CODE Init(const FinItemNode<OmniType>& node, CSTR label);

    virtual const char* get_Name();

    virtual void set_Name(const char* name);

    virtual const char* get_XbrlLabel();

    virtual bool get_IsAbstract();
    
    virtual RP<IXbrlAbstractRecord> get_Parent();

    virtual void set_Parent(IXbrlAbstractRecord*);

    virtual short get_LowerYear();

    virtual short get_HighestYear();
    
    virtual I16 get_LatestQuarter();

    virtual long unsigned int get_Financials(short year, IXbrlField** ppOut);

    virtual XPT get_PeriodType();
    

    virtual bool get_IsProgression();


    virtual long unsigned int FillEmptyFields();

    virtual SE_CODE get_Children(IXbrlRecordList**);
    

    //virtual long unsigned int get_ConsolidatedView(IFinancialsConsolidated**);
   

    SE_CODE set_Children(IXbrlRecordList* pIn);
    
    virtual void Serialize(ISerializedData<IString>* pIn);
    
};




///////////////////////////////CXbrlRecordList//////////////////////////

class CXbrlRecordList : public CROSPList<IXbrlAbstractRecord, IXbrlRecordList>
{
public:
    
    CXbrlRecordList(ISession*);
    virtual ~CXbrlRecordList();
    
    SE_CODE Init(IFinancialsConsolidated* p);
    SE_CODE Init(IFinancialsConsolidated* p, IXbrlAbstractRecord* pParent);
    
    virtual short get_LowerYear();
    virtual short get_HighestYear();
    virtual long unsigned int FillEmptyFields();
    RP<IXbrlAbstractRecord> get_Parent();
    void set_Parent(IXbrlAbstractRecord* parent);
    virtual void Add(IXbrlAbstractRecord* item);
    
private:
    //SP<IFinancialsConsolidated> _p;
    I16 _ceilYear, _floorYear;
    IXbrlAbstractRecord* _pparent;
    friend class CXbrlAbstractRecord;
    friend class CXbrlRecord;
};



#endif	/* CFINANCIALITEM_H */

