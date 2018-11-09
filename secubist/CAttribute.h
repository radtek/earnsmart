/* 
 * File:   CAttribute.h
 * Author: santony
 *
 * Created on December 4, 2013, 8:04 PM
 */

#ifndef CATTRIBUTE_H
#define	CATTRIBUTE_H

#include <string>
#include <vector>

#include "../seglobaltypedefs.h"
#include "../seglobal/IObject.h"

using namespace std;

namespace se
{
    namespace math
    {

        typedef enum enAttributeDataType
        {
            ADT_Continous,             // numeric (integer or float)
            ADT_Discrete,               // classifier such as a text Wet, Dry, Unknown
            ADT_Label,                   //  This attribute contains an identifying label for each case, such as an account number or an order code. The value of the attribute is ignored when models are constructed, but is used when referring to individual cases.
            ADT_Ignore,                  // The values of the attribute should be ignored.
            ADT_Timestamp,               // The attribute's values are times in the form YYYY/MM/DD HH:MM:SS or YYYY-MM-DD HH:MM:SS
            ADT_Date,                    // Date attribute of the form YYYY/MM/DD or YYYY-MM-DD
            ADT_Time,                   // time attribute of form HH:MM:SS 
            ADT_Formula,                // Formula attribute. A formula attribute depends on another attribute. Eg. Attribute "A" defined previously will become another attribute LogA = log(A)
            //ADT_CaseWeight              // a special attribute with numeric value only
        } ADT, ATTRIBUTEDATATYPE;
        

        class CAttribute 
        {
        protected:
            CAttribute(string name = "");
        public:
            CAttribute(const CAttribute& orig);
            virtual ~CAttribute();
            
            string get_Name();
            ADT get_DataType() const;
            string get_Comments();
            
            void set_Name(string const& name);
            void set_DateType(ADT datatype);
            void set_Comments(string& text);
            
        private:
            ADT _attrDataType;
            string _attrName;
            string _comments;
        };
        
        typedef vector<string> DiscreteAttributes;
        
        template<typename T>
        class CAttributeT : public CAttribute
        {
        public:
            CAttributeT(string name = "") : CAttribute(name)
            {
                
            }
            
            CAttributeT(const CAttributeT& orig) : CAttribute(static_cast<CAttribute>(orig))
            {
                _value = orig._value;
            }
                        
            virtual ~CAttributeT()
            {
                
            }
            
            T get_Value() const
            {
                return _value;
            }
            
            void set_Value(T const& value)
            {
                _value = value;
            }
        protected:
            T _value;
        };
        
        class CAttributeDiscrete : public CAttributeT<DiscreteAttributes>
        {
        public:
            CAttributeDiscrete(string name = "");
            CAttributeDiscrete(string name, U32 maxDiscreteItems);
            CAttributeDiscrete(const CAttributeDiscrete& orig);
            U32 get_MaxDiscreteItems();
            void AddItem(const string& item);
            bool get_IsOrdered();
            void set_IsOrdered(bool val);
            
        private:
            U32 _numItems;
            bool _bIsOrdered;
        };
        
        class CAttributeDate : public CAttributeT<CDate>
        {
        public:
            CAttributeDate(string name = "");
            CAttributeDate(const CAttributeDate& orig);
            
        };
        
        
        
        class CAttributeDateTime : public CAttributeT<se::DATETIME>
        {
        public:
            CAttributeDateTime(string name = "");
            CAttributeDateTime(const CAttributeDateTime& orig);
            
        };
        
        class CAttributeNumeric : public CAttributeT<R8>
        {
        public:
            CAttributeNumeric(string name = "");
            CAttributeNumeric(const CAttributeNumeric& orig);
            
        };

        class CAttributeFormula : public CAttributeT<string>
        {
        public:
            CAttributeFormula(string name = "");
            CAttributeFormula(const CAttributeFormula& orig);
            
        };
        
    }
}

#endif	/* CATTRIBUTE_H */

