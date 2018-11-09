/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CAst.h
 * Author: santony
 *
 * Created on May 22, 2016, 5:29 PM
 */

#ifndef CAST_H
#define CAST_H

#include <string>
#include <list>

using namespace std;

namespace se
{
    typedef enum class AstOperator
    {
        None,
        And,
        Or,
        Equal,
        NoEqual,
        Like
    } AST_OPERATOR;

    
    // For now a simple AST which chains conditions to the right.
    typedef struct AstCondition
    {
        OmniType Key;
        OmniType Value;
        AST_OPERATOR AssocOp;
        
        AstCondition(const OmniType& key, AST_OPERATOR assocOp, const OmniType& val);
        
        AstCondition(const AstCondition& right);
        
        AstCondition(AstCondition&& mov);
        
        const AstCondition& operator = (const AstCondition& right);
        
        AstCondition& operator= (AstCondition&& mov);

    } AST_CONDITION;
    
    
    typedef struct AstLogicLink
    {
        AST_CONDITION condition;
        AST_OPERATOR oPerator;
        
        AstLogicLink(AST_CONDITION cond, AST_OPERATOR op);
        
        AstLogicLink(const AstLogicLink& right);
        
        const AstLogicLink& operator=(const AstLogicLink& other);
        
        AstLogicLink& operator=(AstLogicLink&& other);
        
    } AST_LOGICLINK;

    typedef list<AST_LOGICLINK> ConditionClause;

    string OperatorAsString(AST_OPERATOR op);
    
    string OperatorAsSqlString(AST_OPERATOR op);
    
    // A simple sql abstract syntax tree
    class CSqlAst 
    {
    private:
        ConditionClause clause;
        
    private:
        void Push(const AST_CONDITION& condition, const AST_OPERATOR Op);
        
    public:
        CSqlAst(const AST_CONDITION&);
        CSqlAst(const CSqlAst& orig);
        CSqlAst(CSqlAst&& orig);
        virtual ~CSqlAst();

        
        const CSqlAst& operator=(const CSqlAst& other);
        CSqlAst& operator=(CSqlAst&& other);
        
        const CSqlAst& operator&(const AST_CONDITION& condition);
        const CSqlAst& operator|(const AST_CONDITION& condition);

        friend std::ostream& operator<<(std::ostream& os, CSqlAst const & ast);

        const ConditionClause& Clause() const;
        
    };

    std::ostream& operator<<(std::ostream& os, CSqlAst const& ast);

}
#endif /* CAST_H */

