/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CEmailTemplate.h
 * Author: santony
 *
 * Created on March 26, 2016, 5:25 PM
 */

#ifndef CEMAILTEMPLATE_H
#define CEMAILTEMPLATE_H

#include <map>

using namespace std;

namespace se
{
    namespace net
    {
        
        typedef map<string, string> TemplateVaribles;
        
        /*
         * Class that allows to process templated email documents
         * with asp style template variables.
         * 
         */
        class CEmailTemplate 
        {
        private:
            const TemplateVaribles& _vars;
            string _filePath;
            SP<ISession> _pSession;
            
        public:
            CEmailTemplate(ISession* pSession, const string& templateFile, const TemplateVaribles& variables);
            CEmailTemplate(const CEmailTemplate& orig) = delete;
            virtual ~CEmailTemplate();
            
            // Apply variables on the email template file stream and set the variable values.
            // returns the path to the newly created file with changes.
            string Apply();

            void Close();
        };
       
    }
}


#endif /* CEMAILTEMPLATE_H */

