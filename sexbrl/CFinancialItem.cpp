/* 
 * File:   CFinancialItem.cpp
 * Author: santony
 * 
 * Created on January 27, 2014, 7:48 PM
 */

#include "sexbrlinternal.h"
#include "CFinancialsConsolidated.h"
#include "CFinancialItem.h"
#include "../seglobal/exceptions.h"

#ifdef __DEBUG__
//LOGgerPtr _logger  = Logger::getLogger("se.xbrl.financialitems");
#endif

/////////////////////////////////CXbrlRecord///////////////////////////////////

CXbrlAbstractRecord::CXbrlAbstractRecord(ISession* ps) : CObjectRoot(ps)
{

}


CXbrlAbstractRecord::~CXbrlAbstractRecord() 
{
    _pkids.Clear();
}

#ifdef __DEBUG__

void CXbrlAbstractRecord::_AddRef() 
{
    CObjectRoot::_AddRef();
   // LOGI(_logger, "CXbrlAbstractRecord: Adding Reference to " << _rc);
}

void CXbrlAbstractRecord::_DecRef() 
{
   // LOGI(_logger, "CXbrlAbstractRecord: Decreasing Reference to " << _rc-1);
    CObjectRoot::_DecRef();
}

#endif

long unsigned int CXbrlAbstractRecord::Init(unsigned long id, const char* name, const char* label) 
{
    set_Id(id);
    _name = name;
    _label = label;
    return S_OK;
}


const char* CXbrlAbstractRecord::get_Name() 
{
    return _name.c_str();
}

void CXbrlAbstractRecord::set_Name(const char* name) 
{
    _name = name;
}

const char* CXbrlAbstractRecord::get_XbrlLabel() 
{
    return _label.c_str();
}

bool CXbrlAbstractRecord::get_IsAbstract() 
{
    return true;
}

RP<IXbrlAbstractRecord> CXbrlAbstractRecord::get_Parent() 
{
    return _pparent;
}

void CXbrlAbstractRecord::set_Parent(IXbrlAbstractRecord* pin) 
{
    _pparent = pin;
}


long unsigned int CXbrlAbstractRecord::get_Children(IXbrlRecordList** ppOut) 
{
    if (!_pkids)
        return E_NotFound;
    _pkids.CopyTo(ppOut);
    return S_OK;
}

void CXbrlAbstractRecord::set_Children(IXbrlRecordList* pkids) 
{
    _pkids = pkids;
}


void CXbrlAbstractRecord::Serialize(ISerializedData<IString>* pIn) 
{
    pIn->Add("AccountId", get_Id());
    pIn->Add("Name", _name.c_str());
    pIn->Add("Label", _label.c_str());
    pIn->Add("IsAbstract", true);
    pIn->Add("Sub accounts", _pkids);
}


////////////////////////////////////CXbrlRecord///////////////////////////////////

CXbrlRecord::CXbrlRecord(ISession* ps) : CObjectRoot(ps)
{

}


CXbrlRecord::~CXbrlRecord() 
{
    _pkids.Clear();
}

#ifdef __DEBUG__

void CXbrlRecord::_AddRef() 
{
    CObjectRoot::_AddRef();
   // LOGI(_logger, "CXbrlRecord: Adding Reference to " << _rc);
}

void CXbrlRecord::_DecRef() 
{
   // LOGI(_logger, "CXbrlRecord: Decreasing Reference to " << _rc-1);
    CObjectRoot::_DecRef();
}

#endif


long unsigned int CXbrlRecord::Init(const FinItemNode<OmniType>& node, CSTR label) 
{
    _node = node;
    _label = label;
    set_Id(node.XbrlId);
    return S_OK;
}

const char* CXbrlRecord::get_Name() 
{
    return _node.XbrlName.c_str();
}

void CXbrlRecord::set_Name(const char* name) 
{
    throw seexception(E_NotSupported);
}


const char* CXbrlRecord::get_XbrlLabel() 
{
    return _label.c_str();
}

bool CXbrlRecord::get_IsAbstract() 
{
    return false;
}

RP<IXbrlAbstractRecord> CXbrlRecord::get_Parent() 
{
    return _pparent;
}

void CXbrlRecord::set_Parent(IXbrlAbstractRecord* pin) 
{
    _pparent = pin;
}

short CXbrlRecord::get_LowerYear() 
{
    return _node.FloorYear;
}

short CXbrlRecord::get_HighestYear() 
{
    return _node.CeilYear;
}

short CXbrlRecord::get_LatestQuarter() 
{
    return _node.HighQuarter;
}


long unsigned int CXbrlRecord::get_Financials(short year, IXbrlField** ppOut) 
{
    //////LoggerPtr _logger = Logger::getLogger("se.xbrlrecord.getfinancials");
    YEARLYVALUES::const_iterator it = _yv.find(year);
    if (it != _yv.end())
    {
       // LOGI(_logger, "Found financials for year " << year);
        *ppOut = it->second._ptr;
        (*ppOut)->AddRef();
        return S_OK;
    }
    
    SP<IXbrlField> pf;
    RETURNIFFAILED(CObject<CXbrlField>::Create(get_Session(), year, this, &pf));
    _yv.insert(make_pair(year, pf));
   // LOGI(_logger, "Created a new financials for year " << year);
    pf.CopyTo(ppOut);
    return S_OK;
}

XPT CXbrlRecord::get_PeriodType() 
{
    return _node.PeriodType;
}

bool CXbrlRecord::get_IsProgression() 
{
    return _node.IsProgression;
}


long unsigned int CXbrlRecord::FillEmptyFields() 
{
    if (_node.PeriodType != XPT_Duration)
        return S_False;
    // loop first to see if a quarter data is available.
    for (int year = _node.FloorYear; year <= _node.CeilYear; ++year)
    {
        int quarters = 0;
        double qsum = 0.0;
        for (int i=1; i <= 4; ++i)
        {
            double val = _node.GetValue(year, i); 
            if (val)
                ++quarters;
            qsum += val;
        }
        if (quarters < 4)
        {
            double fy = _node.GetValue(year);
            if (fy)
            {
                if (fy == (double)_node.GetValue(year, 4))
                {
                    qsum -= fy;
                    _node.SetValue(year, 4, 0.00);
                    quarters--;
                }
                double diff = fy - qsum;
                double split = diff / (4-quarters);
                for (int i=1; i <= 4; ++i)
                {
                    if (!_node.GetValue(year, i).IsEmpty())
                    {
                        _node.SetValue(year, i, split);
                    }
                }
            }
        }
        else
        {
            if (!_node.GetValue(year).IsEmpty())
            {
                _node.SetValue(year, qsum);
            }
        }
    }
}

SE_CODE CXbrlRecord::get_Children(IXbrlRecordList** ppOut) 
{
    if (!_pkids)
        return E_NotFound;
    _pkids.CopyTo(ppOut);
    return S_OK;
}

SE_CODE CXbrlRecord::set_Children(IXbrlRecordList* pIn) 
{
    _pkids = pIn;
    return S_OK;
}

//long unsigned int CXbrlRecord::get_ConsolidatedView(IFinancialsConsolidated** ppOut) 
//{
//    _node.
//    if (_node.pConsolidated)
//    {
//        _node.pConsolidated->AddRef();
//        *ppOut = _node.pConsolidated;
//        return S_OK;
//    }
//    return E_NotFound;
//}


void CXbrlRecord::Serialize(ISerializedData<IString>* pIn) 
{
    pIn->Add("AccountId", get_Id());
    pIn->Add("Name", _node.XbrlName.c_str());
    pIn->Add("Label", _label.c_str());
    pIn->Add("HighestYear", _node.CeilYear);
    pIn->Add("LowestYear", _node.FloorYear);
    switch (_node.PeriodType)
    {
    case XPT_None:
        pIn->Add("PeriodType", "None");
        break;
    case XPT_Duration:
        pIn->Add("PeriodType", "Duration");
        break; 
    case XPT_Instant:
        pIn->Add("PeriodType", "Instant");
        break;
    case XPT_PeriodicInstance:
        pIn->Add("PeriodType", "PeriodicInstance");
        break;
    }
    pIn->Add("IsProgression", _node.IsProgression);
    pIn->Add("IsAbstract", false);
    pIn->Add("Sub accounts", _pkids);
    for (int i=_node.FloorYear; i <= _node.CeilYear; ++i)
    {
        SP<IXbrlField> pf;
        if (SUCCEEDED(get_Financials(i, &pf)))
        {
            stringstream str;
            str << i;
            pIn->Add(str.str().c_str(), pf);
        }
    }
}

/////////////////////////CXbrlRecordList/////////////////////

CXbrlRecordList::CXbrlRecordList(ISession* ps) : CROSPList<IXbrlAbstractRecord, IXbrlRecordList>(ps) 
{

}

CXbrlRecordList::~CXbrlRecordList()
{
   //_p.Clear();
    _vector.clear();
}

long unsigned int CXbrlRecordList::Init(IFinancialsConsolidated* p) 
{
    _floorYear = 2999;
    _ceilYear = 0;
    //_p = p;
    _pparent = nullptr;
    return S_OK;
}

long unsigned int CXbrlRecordList::Init(IFinancialsConsolidated* p, IXbrlAbstractRecord* pParent) 
{
    _floorYear = 2999;
    _ceilYear = 0;
    //_p = p;
    _pparent = pParent;
    return S_OK;
}

RP<IXbrlAbstractRecord> CXbrlRecordList::get_Parent() 
{
    return _pparent;
}

void CXbrlRecordList::set_Parent(IXbrlAbstractRecord* parent) 
{
    _pparent = parent;
}

short CXbrlRecordList::get_HighestYear() {
    return _ceilYear;
}

short CXbrlRecordList::get_LowerYear() {
    return _floorYear;
}

void CXbrlRecordList::Add(IXbrlAbstractRecord* item) 
{
    if (!item->get_IsAbstract())
    {
        IXbrlRecord* pr = (IXbrlRecord*)item;
        _floorYear = std::min(_floorYear, pr->get_LowerYear());
        _ceilYear = std::max(_ceilYear, pr->get_HighestYear());
    }
    item->set_Parent(_pparent);
    CROSPList::Add(item);
}

long unsigned int CXbrlRecordList::FillEmptyFields() 
{
    for (IXbrlAbstractRecord* p : _vector)
    {
        if (!p->get_IsAbstract())
            static_cast<IXbrlRecord*>(p)->FillEmptyFields();
    }
}

////////////////////////////CXbrlField/////////////////////////////////

CXbrlField::CXbrlField(ISession* ps) : CObjectRoot(ps)
{
}

long unsigned int CXbrlField::Init(I16 year, CXbrlRecord* prec) 
{
    pr = prec;
    _year = year;
    _pnode = &pr->_node;
    return S_OK;
}


 const OmniType& CXbrlField::get_FY()  const
{
    return _pnode->GetValue(_year);
}

const OmniType& CXbrlField::get_HY1()  const
{
    return _pnode->GetValue(_year, FinItemNode<OmniType>::FirstHalf);
}

const OmniType& CXbrlField::get_F3Q()  const
{
    return _pnode->GetValue(_year, FinItemNode<OmniType>::FirstThreeQuarters);
}

const OmniType& CXbrlField::get_Q1()  const
{
    return _pnode->GetValue(_year, 1);
}

const OmniType& CXbrlField::get_Q2()  const
{
    return _pnode->GetValue(_year, 2);

}

const OmniType& CXbrlField::get_Q3()  const
{
    return _pnode->GetValue(_year, 3);

}

const OmniType& CXbrlField::get_Q4()  const
{
    return _pnode->GetValue(_year, 4);

}

const OmniType& CXbrlField::get_Q(short quarter) const
{
    switch (quarter)
    {
    case 1:
        return get_Q1();
    case 2:
        return get_Q2();
    case 3:
        return get_Q3();
    case 4:
        return get_Q4();
    default:
        throw seexception("Invalid quarter.");
    }
}


void CXbrlField::Serialize(ISerializedData<IString>* pIn)
{
    pIn->Add("Year", _year);
    std::stringstream str;
    str << _pnode->GetValue(_year);
    pIn->Add("FY", str.str().c_str());

    str.str("");
    str << _pnode->GetValue(_year, FinItemNode<OmniType>::FirstHalf);
    pIn->Add("HY1", str.str().c_str());

    str.str("");
    str << _pnode->GetValue(_year, FinItemNode<OmniType>::FirstThreeQuarters);
    pIn->Add("F3Q", str.str().c_str());

    str.str("");
    str << _pnode->GetValue(_year, 1);
    pIn->Add("Q1", str.str().c_str());

    str.str("");
    str << _pnode->GetValue(_year, 2);
    pIn->Add("Q2", str.str().c_str());

    str.str("");
    str << _pnode->GetValue(_year, 3);
    pIn->Add("Q3", str.str().c_str());

    str.str("");
    str << _pnode->GetValue(_year, 4);
    pIn->Add("Q4", str.str().c_str());
   
}

