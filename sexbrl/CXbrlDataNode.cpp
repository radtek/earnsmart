/* 
 * File:   CXbrlDataNode.cpp
 * Author: aashok
 * 
 * Created on July 9, 2013, 8:33 PM
 */

#include "sexbrlinternal.h"
#include "CXbrlDataNode.h"
#include "../seglobal/exceptions.h"

using namespace se;


CXbrlDataNode::CXbrlDataNode(ISession* ps) : CObjectRoot(ps) 
{
    
}


CXbrlDataNode::~CXbrlDataNode() 
{
}

ErrorId CXbrlDataNode::Init()
{
    _numeric =0;
    _monetary = 0.0;
    _total = 0.0;
    _periodType = XPT_None;
    _weight = 0;
    
    auto retCode = CObject< CROSPList<IXbrlDataNode> >::Create(get_Session(), &_children);
    
    return retCode;
}

void CXbrlDataNode::Serialize(ISerializedData<IString>* pIn)
{
    if (!pIn)
    {
       // LOGWP("pIn is null, can't serialize.");
        return;
    }
    
/*
 *     R8 _monetary;
    string _text;
    I32 _numeric;
    string _name;
    XDT _type;  
    string _label;
    R8 _total;
    
    SP< CROSPList <IXbrlDataNode> > _children;

 */  
   // LOGIP("Starting to serialize xbrl data node.");
    pIn->Add("id", get_Id());
    pIn->Add("name", _name.c_str());
    pIn->Add("label", _label.c_str());
   // LOGIP("name::"<<_name);
    R8 total =0;
    switch (_type)
    {
        case XDT_Monetary:
            pIn->Add("type", "Monetary");
            pIn->Add("value", _monetary);
            total = _monetary;
            break;
        case XDT_Numeric:
            pIn->Add("type", "Numeric");
            pIn->Add("value", _numeric);
            total = (R8)_numeric;
            break;
        case XDT_Text:
            pIn->Add("type", "Text");
            pIn->Add("value", _text.c_str());
            break;
        default:
            pIn->Add("type", "Not Set");
            pIn->Add("value", "");
            break;
    }
    if(_children->get_Count() == 0)
    {
        pIn->Add("total", total);
        total = 0;
    }
    else if(_children->get_Count() != 0 && total !=0)
    {
        pIn->Add("total", total);
    }
    else
    {
       pIn->Add("total", _total);
    }
    
    if (_children)
    {
        string str = "Children Of " + _name;
        pIn->Add(str.c_str(), _children);
    }

    switch (_periodType)
    {
        case XPT_None:
            pIn->Add("periodtype", "None");
            break;
        case XPT_Duration:
            pIn->Add("periodtype", "Duration");
            break;
        case XPT_Instant:
            pIn->Add("periodtype", "Instant");
            break;
    }
  
   // LOGIP("Completed serializing xbrl data node.");
}

XDT CXbrlDataNode::get_Type() 
{
    return _type;
}

CSTR CXbrlDataNode::get_Name()
{
    return _name.c_str();
}

CSTR CXbrlDataNode::get_Label()
{
    return _label.c_str();
}

XPT CXbrlDataNode::get_PeriodType()
{
    return _periodType;
}

ErrorId CXbrlDataNode::get_Children(IROSPList<IXbrlDataNode>** ppOut)
{
    _children.CopyTo(ppOut);
    return S_Success;
}

void CXbrlDataNode::AppendChildren(IXbrlDataNode* node)
{
    if (_xbrlIds.find(node->get_Id())==_xbrlIds.end())
    {
        _children->Add(node);
        _xbrlIds.insert(node->get_Id());
        
        if (_weight >= 0 )
        {
            if(_type == XDT_Monetary)
                _total += (R8)node->get_Monetary();
            else if(_type == XDT_Monetary)
                _total += (R8)node->get_Numeric();
        }
        else
        {
            if(_type == XDT_Monetary)
                _total -= (R8)node->get_Monetary();
            else if(_type == XDT_Monetary)
                _total -= (R8)node->get_Numeric();
        }
    }
}

void CXbrlDataNode::set_Weight(I32 weight)
{
    _weight = weight;
}


R8 CXbrlDataNode::get_Total()
{
    return _total;
}

CXbrlDataNode* CXbrlDataNode::Add(CXbrlDataNode* pIn)
{
    if (pIn->get_Type() != this->get_Type())
        throw seexception("Type mismatch.");
    
    if (
        (this->get_Type() != XDT_Numeric) ||
        (this->get_Type() != XDT_Monetary)
    )
        throw seexception("Only monetary or numeric is allowed in arithmetic operations.");
    
    SP<CXbrlDataNode> pNode;
    CObject<CXbrlDataNode>::Create(get_Session(), &pNode);
    switch (pNode->get_Type())
    {
        case XDT_Numeric:
            pNode->_numeric = this->_numeric + pIn->_numeric;
            break;
        case XDT_Monetary:
            pNode->_monetary = this->_monetary + pIn->_monetary;
            break;
    }
    return pNode;
}

CXbrlDataNode* CXbrlDataNode::Substract(CXbrlDataNode* pIn)
{
    if (pIn->get_Type() != this->get_Type())
        throw seexception("Type mismatch.");
    
    if (
        (this->get_Type() != XDT_Numeric) ||
        (this->get_Type() != XDT_Monetary)
    )
        throw seexception("Only monetary or numeric is allowed in arithmetic operations.");
    
    SP<CXbrlDataNode> pNode;
    CObject<CXbrlDataNode>::Create(get_Session(), &pNode);
    switch (pNode->get_Type())
    {
        case XDT_Numeric:
            pNode->_numeric = this->_numeric - pIn->_numeric;
            break;
        case XDT_Monetary:
            pNode->_monetary = this->_monetary - pIn->_monetary;
            break;
    }
    return pNode;
}

CXbrlDataNode* CXbrlDataNode::Multiply(CXbrlDataNode* pIn)
{
    if (pIn->get_Type() != this->get_Type())
        throw seexception("Type mismatch.");
    
    if (
        (this->get_Type() != XDT_Numeric) ||
        (this->get_Type() != XDT_Monetary)
    )
        throw seexception("Only monetary or numeric is allowed in arithmetic operations.");
    
    SP<CXbrlDataNode> pNode;
    CObject<CXbrlDataNode>::Create(get_Session(), &pNode);
    switch (pNode->get_Type())
    {
        case XDT_Numeric:
            pNode->_numeric = this->_numeric * pIn->_numeric;
            break;
        case XDT_Monetary:
            pNode->_monetary = this->_monetary * pIn->_monetary;
            break;
    }
    return pNode;
}

CXbrlDataNode* CXbrlDataNode::Divide(CXbrlDataNode* pIn)
{
    if (pIn->get_Type() != this->get_Type())
        throw seexception("Type mismatch.");
    
    if (
        (this->get_Type() != XDT_Numeric) ||
        (this->get_Type() != XDT_Monetary)
    )
        throw seexception("Only monetary or numeric is allowed in arithmatic operations.");
    
    if(this->get_Type() != XDT_Numeric && pIn->_numeric == 0)
        throw seexception("Numeric value - Divide by Zero Not Possible");
    
    if(this->get_Type() != XDT_Monetary && pIn->_monetary == 0)
        throw seexception("Monetary value - Divide by Zero Not Possible");
    
    SP<CXbrlDataNode> pNode;
    CObject<CXbrlDataNode>::Create(get_Session(), &pNode);
    switch (pNode->get_Type())
    {
        case XDT_Numeric:
            pNode->_numeric = this->_numeric / pIn->_numeric;
            break;
        case XDT_Monetary:
            pNode->_monetary = this->_monetary / pIn->_monetary;
            break;
    }
    return pNode;
}


R8 CXbrlDataNode::get_Monetary()
{
    return _monetary;
}

I64 CXbrlDataNode::get_Numeric()
{
    return _numeric;
    
}

CSTR CXbrlDataNode::get_Text()
{
    return _text.c_str();
    
}

void CXbrlDataNode::set_Type(XDT type)
{
    _type = type;
}

void CXbrlDataNode::set_Monetary(R8 Monetary)
{
    _monetary = Monetary;
    _type = XDT_Monetary;
}

void CXbrlDataNode::set_Numeric(I64 Numeric)
{
    _numeric = Numeric;
    _type = XDT_Numeric;
}

void CXbrlDataNode::set_Text(CSTR Text)
{
    _text = Text;
    _type = XDT_Text;
}

void CXbrlDataNode::set_Name(CSTR Name) 
{
    _name = Name;
}

void CXbrlDataNode::set_PeriodType(XPT periodType)
{
    _periodType = periodType;
}

void CXbrlDataNode::set_Total(double Total)
{
    _total = Total;
}

void CXbrlDataNode::set_label(CSTR Label)
{
    _label = Label;
}

IROSPList<IXbrlDataNode>* CXbrlDataNode::get_ChildItems()
{
    return _children._ptr;
}
