/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CAst.cpp
 * Author: santony
 * 
 * Created on May 22, 2016, 5:29 PM
 */

#include "IObject.h"
#include <algorithm>
#include "CAst.h"
#include <sstream>

namespace se
{
    

    AstCondition::AstCondition(const OmniType& key, AST_OPERATOR assocOp, const OmniType& val): 
        Key(key), 
        AssocOp(assocOp), 
        Value(val)
    {

    }

    AstCondition::AstCondition(const AstCondition& right) : 
        Key(right.Key), 
        AssocOp(right.AssocOp), 
        Value(right.Value)
    {
    }

    AstCondition::AstCondition(AstCondition&& mov) : 
            Key(mov.Key), 
            AssocOp(mov.AssocOp), 
            Value(std::move(mov.Value))
    {

    }

    const AstCondition& AstCondition::operator = (const AstCondition& right)
    {
        this->Key = right.Key;
        this->AssocOp = right.AssocOp;
        this->Value = right.Value;
        return *this;
    }

    AstCondition& AstCondition::operator= (AstCondition&& mov)
    {
        this->Key = mov.Key;
        this->AssocOp = mov.AssocOp;
        this->Value = std::move(mov.Value);
        return *this;
    }

    AstLogicLink::AstLogicLink(AST_CONDITION cond, AST_OPERATOR op) : condition(cond), oPerator(op)
    {

    }

    AstLogicLink::AstLogicLink(const AstLogicLink& right) : condition(right.condition), oPerator(right.oPerator)
    {
    }

    const AstLogicLink& AstLogicLink::operator=(const AstLogicLink& other)
    {
        this->condition = other.condition;
        this->oPerator = other.oPerator;
        return *this;
    }

    AstLogicLink& AstLogicLink::operator=(AstLogicLink&& other)
    {
        this->condition = std::move(other.condition);
        this->oPerator = other.oPerator;
        return *this;
    }


    CSqlAst::CSqlAst(const AST_CONDITION& condition) 
    {
        Push(condition, AST_OPERATOR::None);
    }

    CSqlAst::CSqlAst(const CSqlAst& orig) : clause(orig.clause) 
    {
    }

    CSqlAst::CSqlAst(CSqlAst&& orig) : clause(std::move(orig.clause))
    {
    }

    CSqlAst::~CSqlAst() 
    {
    }

    void CSqlAst::Push(const AST_CONDITION& condition, const AST_OPERATOR Op)
    {
        AST_LOGICLINK link(condition, Op);
        clause.push_back(link);
    }

    const CSqlAst& CSqlAst::operator=(const CSqlAst& other) 
    {
        this->clause = other.clause;
        return *this;
    }

    CSqlAst& CSqlAst::operator =(CSqlAst&& other)
    {
        this->clause = std::move(other.clause);
        return *this;
    }


    const CSqlAst& CSqlAst::operator&(const AST_CONDITION& condition) 
    {
        Push(condition, AST_OPERATOR::And);
        return *this;
    }

    const CSqlAst& CSqlAst::operator|(const AST_CONDITION& condition) 
    {
        Push(condition, AST_OPERATOR::Or);
        return *this;
    }

    const ConditionClause& CSqlAst::Clause() const 
    {
        return this->clause;
    }

    std::ostream& operator<<(std::ostream& os, CSqlAst const & ast)
    {
        for(const AST_LOGICLINK& link : ast.Clause())
        {
            if (link.oPerator != AST_OPERATOR::None)
                os << OperatorAsSqlString(link.oPerator) << " ";
            
            stringstream val;
            switch(link.condition.Value.dt)
            {
                case DATATYPES::DT_CSTR:
                    val << "'" << link.condition.Value << "'";
                    break;
                default:
                    val << link.condition.Value;
                    break;
            }
            os  << " " << link.condition.Key  << " " << OperatorAsSqlString(link.condition.AssocOp)  << " " << val.str();
        }
        return os;
    }    

    string OperatorAsSqlString(AST_OPERATOR op)
    {
        switch (op)
        {
            case AST_OPERATOR::And:
                return " AND ";
            case AST_OPERATOR::Equal:
                return " = ";
            case AST_OPERATOR::Like:
                return " LIKE ";
            case AST_OPERATOR::NoEqual:
                return " <> ";
            case AST_OPERATOR::Or:
                return " OR ";
            default:
                return " ";
        }
    }

    string OperatorAsString(AST_OPERATOR op)
    {
        switch (op)
        {
            case AST_OPERATOR::And:
                return " & ";
            case AST_OPERATOR::Equal:
                return " == ";
            case AST_OPERATOR::Like:
                return " LIKE ";
            case AST_OPERATOR::NoEqual:
                return " != ";
            case AST_OPERATOR::Or:
                return " | ";
            default:
                return " ";
        }
    }    

}