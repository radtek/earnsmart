/* 
 * File:   content.h
 * Author: santony
 *
 * Created on August 12, 2013, 8:28 PM
 */

#ifndef CONTENT_H
#define	CONTENT_H

#include <cppcms/view.h>
#include <string>
#include <list>

using namespace cppcms;
using namespace std;

namespace se
{
    namespace model
    {
        
        class master : public base_content
        {
        public:
            session_interface* session;
            string title;
            
            master(session_interface* session)
            {
                this->session = session;
            }
        };
        
        
        class news : public master
        {
        public:
            list<string> news_list;
            
            news(session_interface* session) : master(session)
            {
            }
        };
        
        class ttest
        {
        public:
            string teststr;
        };
        
        class page : public master
        {
        public:
            string 
                page_title, 
                page_content;
            ttest test;

            page(session_interface* session) : master(session)
            {
                
            }
            
        };
    }
}


#endif	/* CONTENT_H */

