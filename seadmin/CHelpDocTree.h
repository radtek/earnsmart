/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CHelpDocTree.h
 * Author: santony
 *
 * Created on April 27, 2016, 8:16 AM
 */

#ifndef CHELPDOCTREE_H
#define CHELPDOCTREE_H

#include "../external/mariadbpp/mysql++.h"
#include "../semysql/CSEConnections.h"

using namespace mysqlpp;

namespace se
{
    namespace admin
    {
        class CHelpDocTree : public CObjectRoot, public se::admin::IHelpDocTree
        {
        private:
            
        private:
            SE_CODE saveDocItem(Json::Value& item);
            SE_CODE getDocItem(UID docId, Json::Value& value, int sublevels);
            void bind(const Row& row, Json::Value& docitem);
            void rgetDocItem(CSmartConnection& conn, int levels, int& level, UID docId, Json::Value& doc);
            void ensureRoot(CSmartConnection&);
            bool hasParent(CSmartConnection&, UID parentId);
            SE_CODE deleteNode(CSmartConnection&, UID docId, bool allowdeleteChildren);
            
            
        protected:
#ifdef __DEBUG__
            void OnAdd(unsigned short rc) override;
            void OnDec(unsigned short rc) override;
#endif

            
        public:
            CHelpDocTree(ISession* psession);
            CHelpDocTree(const CHelpDocTree& orig) = delete;
            CHelpDocTree(CHelpDocTree&& orig) = delete;
            CHelpDocTree& operator=(const CHelpDocTree& orig) = delete;
            CHelpDocTree& operator=(CHelpDocTree&& orig) = delete;
            virtual ~CHelpDocTree();
            
            
            SE_CODE get_RootDoc(Json::Value& value, int sublevels = 10) override;
            SE_CODE get_Doc(UID docId, Json::Value& value, int sublevels = 10) override;
            SE_CODE set_Doc(Json::Value& value) override;
            long unsigned int del_Doc(unsigned long docId, bool agreedToDelChildren = false) override;

        };
    }

}

#endif /* CHELPDOCTREE_H */

