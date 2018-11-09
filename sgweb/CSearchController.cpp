/* 
 * File:   CSearchController.cpp
 * Author: santony
 * 
 * Created on August 25, 2013, 7:58 AM
 */

#include <cppcms/http_response.h>
#include <cppcms/http_request.h>
#include "CSearchController.h"
#include "../seglobal/CJsonSerializedData.h"
#include "models.h"

namespace sg
{
    namespace web
    {
        CSearchController::CSearchController(cppcms::service& srv) : CBaseApplication(srv) 
        {
            // initialize secuirty api
            SecOpen(&_pSecApi);
            
            // this regex allows words and digits and spaces, but also allows a space alone which
            // needs a fix later.
            // contains search urls 
            // first one is company search.
            // url mapping for searching a company (http://domainname/search/co/[searchtext])
            dispatcher().assign("/co", &CSearchController::GetMatchingCompanies, this);
            mapper().assign("co", "/co/");
            
        }

        CSearchController::~CSearchController() 
        {
        }

        void CSearchController::GetMatchingCompanies()
        {
            //response().set_redirect_header("/home");
            // redirect to home of someone is trying to access this page
            // directly.
            // go back to the page called.
            string const& path = request().path_info();
            
            if (request().request_method() != "POST")
            {
                response().set_redirect_header(path);
                return;
            }
            
            const string& searchquery = request().post("cosearch_text");
            if (searchquery.empty())
            {
                response().set_redirect_header(path);
                return;
            }

            RecentFinds f(context());
            
            SP<IROSPList<ISecurityDescription> > pList;
            ErrorId retVal = _pSecApi->SearchSecurities(searchquery.c_str(), &pList);
            if (FAILED(retVal))
            {
                response().set_redirect_header(path);
                return;
            }
            
            if (pList->get_Count() == 1)
            {
                // add this to users recently searched products
                SP<ISecurityDescription> pDesc;
                pList->get_Item(0, &pDesc);
                
                f += pDesc->get_Id();
                
                // redirect t product page.
                stringstream url ;
                url << "/fundamentals/p/summary/" << pDesc->get_Id();
                response().set_redirect_header(url.str());
                
            }
            else
            {
                // load a template that displays a list of companies
                // that matched search query.
                Search search(context());
                search.SetList(pList);
                Master master(context(), search);
                master.Title = "Smartearn | Search Results";
                InitModel(master);
                render("MasterView", master);
            }
        }
    }
}