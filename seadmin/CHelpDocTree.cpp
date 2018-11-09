/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHelpDocTree.cpp
 * Author: santony
 * 
 * Created on April 27, 2016, 8:16 AM
 */

#include "internal.h"
#include "CHelpDocTree.h"
#include "../semain/semain.h"

#define SEDBADM         CSEConnections::GetAdmin()

using namespace se::admin;
using namespace se::data;

CHelpDocTree::CHelpDocTree(ISession* psession) : CObjectRoot(psession)
{
    
}

CHelpDocTree::~CHelpDocTree()
{
    
}

#ifdef __DEBUG__

void CHelpDocTree::OnAdd(unsigned short rc)
{
    BINF << rc;
}

void CHelpDocTree::OnDec(unsigned short rc)
{
    BINF << rc;
}

#endif

void CHelpDocTree::bind(const Row& row, Json::Value& val)
{
    UID docId = (UID)row["docid"];
    val["docid"] = (Json::Int64) docId;
    val["doctitle"] = (string)row["doctitle"];
    val["docdesc"] = (string)row["docdesc"];
    val["parentdocid"] = (string)row["parentdocid"];
}

void CHelpDocTree::rgetDocItem(CSmartConnection& conn, int levels, int& level, unsigned long docId, Json::Value& doc)
{
    level++;
    if (level == levels)
        return ;
    auto q = conn->query();
    q << "select * from finhelpdoc where docid = " << docId;
    auto sqr = q.store();
    if (sqr.size())
    {
        const Row& row = sqr[0];
        bind(row, doc);
        q = conn->query();
        q << "select docid from finhelpdoc where parentdocid = " << docId;
        sqr = q.store();
        for (const Row& row : sqr)
        {
            Json::Value subdoc;
            rgetDocItem(conn, levels, level, (UID)row["docid"], subdoc);
            doc["subjects"].append(subdoc);
        }
    }
}

void CHelpDocTree::ensureRoot(CSmartConnection& conn)
{
    auto query = conn->query();
    query << "insert ignore into finhelpdoc (docid, doctitle, docdesc, parentdocid) ";
    query << "values ";
    query << "(1, 'Finance && Investements', 'Finance and Investment Information Help', 0)";
    query.exec();
}


SE_CODE CHelpDocTree::getDocItem(unsigned long docId, Json::Value& value, int sublevels)
{
    CSmartConnection conn(SEDBADM);
    this->ensureRoot(conn);
    int level = 0;
    rgetDocItem(conn, sublevels, level, docId, value);
    return S_OK;
}

bool CHelpDocTree::hasParent(CSmartConnection& conn, unsigned long parentId) 
{
    auto query = conn->query();
    query << "select docid from finhelpdoc where docid = " << parentId << " limit 1";
    auto use = query.use();
    return use && use.fetch_row();
}


long unsigned int CHelpDocTree::saveDocItem(Json::Value& item)
{
    CSmartConnection conn(SEDBADM);
    this->ensureRoot(conn);
    
    // three things are done in this function.
    // 1. A new document. In this case docid = 0. Parent id needs to be verified
    // 2. An existing document, same docid and parentid. Existing record is updated
    // 3. An existing document, parentid is different. Parentid is verified and document's parentid is set to new value.

    UID parentId = (UID)item.get("parentdocid", (Json::Int64)0L).asInt64();
    bool hasParent = this->hasParent(conn, parentId);
    if (hasParent)
    {
        UID docId = item.get("docid",  (Json::Int64)0L).asInt64();
        auto query = conn->query();
        if (docId == 0) // new document.
        {
            query << "insert into finhelpdoc (doctitle, docdesc, parentdocid) ";
            query << "values (" << quote << item["doctitle"].asString() << ", " << quote << item["docdesc"].asString() << ", " << parentId << ")";
            query.exec();
            item["docid"] = (Json::Int64)query.insert_id();
        }
        else
        {
            query << "update finhelpdoc set";
            query << " doctitle = " << quote << item["doctitle"].asString();
            query << ", docdesc = " << quote << item["docdesc"].asString() ;
            query << ", parentdocid = " << parentId;
            query << " where docid = " << docId;
            query.exec();
        }
        return S_OK;
    }
    return E_ParentNotFound;
}

long unsigned int CHelpDocTree::deleteNode(CSmartConnection& conn, unsigned long docId, bool allowdeleteChildren) 
{
    Query q = conn->query();
    q << "select docid from finhelpdoc where parentdocid = " << docId;
    auto store = q.store();
    if (!allowdeleteChildren && store.size())
    {
        return E_HelpDocHasChildren;
    }
    for (const Row& row : q.store())
    {
        UID did = (UID)row["docid"];
        auto r = deleteNode(conn, did, allowdeleteChildren);
        if (FAILED(r))
        {
            return r;
        }
    }
    return S_OK;
}



SE_CODE CHelpDocTree::get_RootDoc(Json::Value& value, int sublevels)
{
    return getDocItem(1, value, sublevels);
}

SE_CODE CHelpDocTree::get_Doc(unsigned long docId, Json::Value& value, int sublevels)
{
    return getDocItem(docId, value, sublevels);
}

long unsigned int CHelpDocTree::set_Doc(Json::Value& value)
{
    ISessionInterface* pSession = static_cast<ISessionInterface*>(get_Session());
    if (!pSession->get_IsAuthenticated() || !pSession->get_User()->get_IsInRole(UserRole::Internal))
    {
        return E_SecurityAuthorization;
    }
    return this->saveDocItem(value);
}

long unsigned int CHelpDocTree::del_Doc(unsigned long docId, bool agreedToDelChildren) 
{
    ISessionInterface* pSession = static_cast<ISessionInterface*>(get_Session());
    if (!pSession->get_IsAuthenticated() || !pSession->get_User()->get_IsInRole(UserRole::Internal))
    {
        return E_SecurityAuthorization;
    }
    
    try
    {
        CSmartConnection conn(SEDBADM);
        try
        {
            conn.BeginTransaction();
            auto r = deleteNode(conn, docId, agreedToDelChildren);
            if (r == S_OK)
                conn.CommitTransaction();
            else
                conn.RollbackTransaction();
            return r;
        }
        catch(const std::exception& ex)
        {
            conn.RollbackTransaction();
            throw ex;
        }
    }
    CATCHCLAUSE("CHelpDocTree::del_Doc");
}

