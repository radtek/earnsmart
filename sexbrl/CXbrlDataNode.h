/* 
 * File:   CXbrlDataNode.h
 * Author: aashok
 *
 * Created on July 9, 2013, 8:33 PM
 */

#ifndef CXBRLDATANODE_H
#define	CXBRLDATANODE_H

#include "sexbrlapi.h"

#include <string>
#include "../seglobal/CROList.h"
////#include <log4cxx/logger.h>
#include <set>

using namespace se;
using namespace se::xbrl;
using namespace std;
//using namespace log4cxx;

class CXbrlDataNode : public CObjectRoot, public IXbrlDataNode
{
public:
    CXbrlDataNode(ISession*);
    CXbrlDataNode(const CXbrlDataNode& orig)=delete;
    virtual ~CXbrlDataNode();
    
    virtual XDT get_Type() ;
    virtual R8 get_Monetary();
    virtual I64 get_Numeric();
    virtual CSTR get_Text();
    virtual CSTR get_Name() ;
    virtual CSTR get_Label();
    virtual XPT get_PeriodType();
    virtual ErrorId get_Children(IROSPList<IXbrlDataNode>** ppOut) ;
    virtual R8 get_Total() ;
    virtual void Serialize(ISerializedData<IString>* pIn);
    virtual IROSPList<IXbrlDataNode>* get_ChildItems(); // same as get_Children, but returning a pointer or null.
    
    
    void set_Type(XDT type);
    void set_Monetary(R8 Monetary);
    void set_Numeric(I64 Numeric);
    void set_Text(CSTR Text);
    void set_Name(CSTR Name) ;
    void set_PeriodType(XPT periodType);
    void AppendChildren(IXbrlDataNode* node);
    void set_Total(R8 Total);
    void set_label(CSTR Label);
    
    
    void set_Weight(I32 weight);
    
    
    CXbrlDataNode* Add(CXbrlDataNode*);
    CXbrlDataNode* Substract(CXbrlDataNode*);
    CXbrlDataNode* Multiply(CXbrlDataNode*);
    CXbrlDataNode* Divide(CXbrlDataNode*);
    
    virtual ErrorId Init();
    
private:
  
    R8 _monetary;
    string _text;
    I64 _numeric;
    string _name;
    XDT _type;  
    string _label;
    XPT _periodType;
    R8 _total;
    SP< CROSPList <IXbrlDataNode> > _children;
    typedef set<I64> XBRLIDS;
    XBRLIDS _xbrlIds;
    ////LoggerPtr _logger;
    I32 _weight;

};

#endif	/* CXBRLDATANODE_H */

