/* 
 * File:   CAttribute.cpp
 * Author: santony
 * 
 * Created on December 4, 2013, 8:04 PM
 */

#include "internal.h"
#include "CAttribute.h"

namespace se
{
    namespace intel
    {

        CAttribute::CAttribute(string name) 
        {
            set_Name(name);
        }

        CAttribute::CAttribute(const CAttribute& orig) 
        {
            _attrDataType = orig._attrDataType;
            _attrName = orig._attrName;
        }

        CAttribute::~CAttribute() 
        {
            
        }
        
        string CAttribute::get_Name()
        {
            return _attrName;
        }
        
        ADT CAttribute::get_DataType() const
        {
            return _attrDataType;
        }
        
        void CAttribute::set_DateType(ADT datatype)
        {
            _attrDataType = datatype;
        }
        
        void CAttribute::set_Name(const string& name)
        {
            _attrName = name;
        }
        
        string CAttribute::get_Comments()
        {
            return _comments;
        }

        void CAttribute::set_Comments(string& text)
        {
            _comments = text;
        }
        
        
        // CAttributeDiscrete
        
        CAttributeDiscrete::CAttributeDiscrete(string name) : CAttributeT(name),  _numItems(0), _bIsOrdered(false)
        {
            set_DateType(ADT_Discrete);
        }
        
        CAttributeDiscrete::CAttributeDiscrete(string name, unsigned int maxDiscreteItems) 
                : CAttributeT(name),  _numItems(maxDiscreteItems), _bIsOrdered(false)
        {
            set_DateType(ADT_Discrete);
            _value = std::move(DiscreteAttributes(maxDiscreteItems));
        }
        
        CAttributeDiscrete::CAttributeDiscrete(const CAttributeDiscrete& orig) 
                : CAttributeT(static_cast<CAttributeT<DiscreteAttributes>>(orig))
        {
            _numItems = orig._numItems;
            _bIsOrdered = orig._bIsOrdered;
        }
        
        void CAttributeDiscrete::AddItem(const string& item)
        {
            _value.push_back(item);
            _numItems = _value.size();
        }
        
        bool CAttributeDiscrete::get_IsOrdered()
        {
            return _bIsOrdered;
        }

        void CAttributeDiscrete::set_IsOrdered(bool val)
        {
            _bIsOrdered = val;
        }
        
        U32 CAttributeDiscrete::get_MaxDiscreteItems()
        {
            return _numItems;
        }
        
        // CAttributeDate
        CAttributeDate::CAttributeDate(string name) : CAttributeT(name)
        {
            set_DateType(ADT_Date);
        }
        
        CAttributeDate::CAttributeDate(const CAttributeDate& orig)
                : CAttributeT(static_cast<CAttributeT<CDate>>(orig))
        {
            set_DateType(ADT_Date);
        }
        
        // CAttributeDateTime
        CAttributeDateTime::CAttributeDateTime(string name): CAttributeT(name)
        {
            set_DateType(ADT_Timestamp);
        }
        
        CAttributeDateTime::CAttributeDateTime(const CAttributeDateTime& orig)
                : CAttributeT(static_cast<CAttributeT<DATETIME>>(orig))
        {
            set_DateType(ADT_Timestamp);
        }
        
        // CAttributeNumeric
        CAttributeNumeric::CAttributeNumeric(string name) : CAttributeT(name)
        {
            set_DateType(ADT_Continous);
        }
        
        CAttributeNumeric::CAttributeNumeric(const CAttributeNumeric& orig)
                : CAttributeT(static_cast<CAttributeT<R8>>(orig))
        {
            set_DateType(ADT_Continous);
        }
        
        // CAttributeFormula
        CAttributeFormula::CAttributeFormula(string name) : CAttributeT(name)
        {
            set_DateType(ADT_Continous);
        }
        
        CAttributeFormula::CAttributeFormula(const CAttributeFormula& orig)
                : CAttributeT(static_cast<CAttributeT<string>>(orig))
        {
            set_DateType(ADT_Continous);
        }
    }
}