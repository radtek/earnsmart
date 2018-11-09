/* 
 * File:   CAdminApp.cpp
 * Author: santony
 * 
 * Created on August 18, 2013, 12:34 PM
 */

#include "CAdminApp.h"
#include "admmodels.h"
#include <jsoncpp/json/json.h>
#include "sgwebinternal.h"
#include "CSPSerializable.h"
#include "CSmartearnApp.h"
#include <boost/tokenizer.hpp>
#include <cgicc/HTMLClasses.h>
#include "../seglobal/exceptions.h"
#include "../senet/senetapi.h"
#include "../setraining/setrainingapi.h"
#include "../secubist/CCubist.h"
#include "../seglobal/CQuarter.h"
#include "../seeconomy/seeconomapi.h"

using namespace cppcms;
using namespace sg::web::adm;
using namespace Json;
using namespace se::data;
using namespace boost;
using namespace se::training;
using namespace se::economy;

#define SECONN  CSEConnections::GetMain()

namespace sg
{
    namespace web
    {
        

        CAdminApp::CAdminApp(cppcms::service& srv) : CBaseApplication(srv)
        {
            //_logger = Logger::getLogger("sg.web.adminapp");
            
            //CBaseApplication::_logger = Logger::getLogger("sg.web.adminapp");
            CBaseApplication::Init(srv);

            dispatcher().assign("/companies/mapaccounts/common", &CAdminApp::CommonAccounts, this);
            mapper().assign("co_ma_common");

            dispatcher().assign("/companies/mapaccounts/common/add", &CAdminApp::CommonAccountsAdd, this);
            mapper().assign("co_ma_common_add");
            
            dispatcher().assign("/companies/mapaccounts/mapcommon", &CAdminApp::MapCommonAccounts, this);
            mapper().assign("co_ma_mapcommon");

            dispatcher().assign("/companies/mapaccounts/mapcommon/add", &CAdminApp::MapCommonAccountsAdd, this);
            mapper().assign("co_ma_mapcommon_add");

            dispatcher().assign("/companies/mapaccounts/mapcommon/remove", &CAdminApp::MapCommonAccountsRemove, this);
            mapper().assign("co_ma_mapcommon_rem");

            dispatcher().assign("/companies/mapaccounts/mapcommon/sort", &CAdminApp::MapCommonAccountsSort, this);
            mapper().assign("co_ma_mapcommon_sort");
            
            dispatcher().assign("/companies/mapaccounts/mapspecific", &CAdminApp::MapSpecificAccounts, this);
            mapper().assign("co_ma_mapspecific");

            dispatcher().assign("/companies/mapaccounts/mapspecific/add", &CAdminApp::MapSpecificAccountsAdd, this);
            mapper().assign("co_ma_mapspecific_add");
            
            dispatcher().assign("/companies/mapaccounts/mapspecific/remove", &CAdminApp::MapSpecificAccountsRemove, this);
            mapper().assign("co_ma_mapspecific_rem");
            
            dispatcher().assign("/companies/mapaccounts/mapspecific/sort", &CAdminApp::MapSpecificAccountsSort, this);
            mapper().assign("co_ma_mapspecific_sort");

            dispatcher().assign("/companies/prediction/(general|tie|test)", &CAdminApp::PredictionSetup, this, 1);
            mapper().assign("co_pred_setup", "/companies/prediction/{1}");

            dispatcher().assign("/companies/prediction/setup", &CAdminApp::SetupPrediction, this);
            mapper().assign("co_setup_pred");
            
            dispatcher().assign("/companies/prediction/delete", &CAdminApp::DeletePrediction, this);
            mapper().assign("co_delete_pred");
            
            dispatcher().assign("/companies/prediction/listall", &CAdminApp::ListAllPredictionSetup, this);
            mapper().assign("co_pred_listall");

            dispatcher().assign("/companies/prediction/listproduct", &CAdminApp::ListProductPredictionSetup, this);
            mapper().assign("co_pred_listproduct");

            dispatcher().assign("/companies/prediction/listgics", &CAdminApp::ListGicsPredictionSetup, this);
            mapper().assign("co_pred_listgics");

            dispatcher().assign("/companies/prediction/saveproduct", &CAdminApp::SaveProductPredictionSetup, this);
            mapper().assign("co_pred_saveproduct");

            dispatcher().assign("/companies/prediction/savegics", &CAdminApp::SaveGicsPredictionSetup, this);
            mapper().assign("co_pred_savegics");

            dispatcher().assign("/companies/prediction/saveall", &CAdminApp::SaveAllPredictionSetup, this);
            mapper().assign("co_pred_saveall");

            dispatcher().assign("/companies/prediction/testmodel", &CAdminApp::StepThruModelTesting, this);
            mapper().assign("co_pred_testmodel");

            dispatcher().assign("/economy/manage/add", &CAdminApp::EconomyManageAdd, this);
            mapper().assign("econ_mg_add");

            dispatcher().assign("/economy/manage/edit", &CAdminApp::EconomyManageEdit, this);
            mapper().assign("econ_mg_edit");
            
            dispatcher().assign("/economy/manage/getpredictedvalue", &CAdminApp::EconomyGetPredictedValue, this);
            mapper().assign("econ_mg_getpredval");
            
            dispatcher().assign("/indicators/economy/manage", &CAdminApp::ManageEconomicIndicators, this);
            mapper().assign("ind_eco_manage");
            
            dispatcher().assign("/companies", &CAdminApp::Default, this);
            mapper().assign("companies");

            dispatcher().assign("", &CAdminApp::Default, this);
            mapper().assign("");
        }


        CAdminApp::~CAdminApp() 
        {
        }

        void CAdminApp::init()
        {
            CBaseApplication::init();
        }

        void CAdminApp::GetJson(string p1, string p2, string p3) 
        {
            try
            {
                response().set_content_header("application/json");
                int ctxt = strtol(p1.c_str(), nullptr, 10);
                switch (ctxt)
                {
                case 10010:
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("ListGeneralAccounts", data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    ListGeneralAccounts();
                    break;
                }
                case 10013:
                {
                    MakeSerializable<string> data;
                    if (cache().fetch_data("ListUsgaapAccounts", data))
                    {
                        response().out() << data.get();
                        break;
                    }
                    ListUsgaapAccounts();
                    break;
                }
                case 10011:
                {
                    ListCommonlyMappedAccounts(p3);
                    break;
                }
                case 10012:
                {
                    ListCompanyAccounts(p3);
                    break;
                }
                case 10014:
                {
                    ListTrainingSets();
                    break;
                }
                case 10015:
                {
                    ListTrainingSetDetail(p3);
                    break;
                }
                case 10016:
                {
                    break;
                }
                default:
                    CBaseApplication::GetJson(p1, p2, p3);
                    break;
                }
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, ex.what());
                throw ex;
            }
        }
        
        ////////////////////Json Routines///////////////////////////////

        void CAdminApp::ListTrainingSetDetail(string setId) 
        {
            // json returned as below
            /* { 
             *      setid : 1, 
             *      result: { type: 'P', Value: 12 },
             *      attribs : [ { Type : 'E', Value: 11}, ... ]
             * }
             */
            
            if (setId.empty() || setId == "0")
            {
                response().out() << "{}";
            }
            
            CSmartConnection conn(SECONN);
            
            UID iid = strtol(setId.c_str(), 0, 10);
            Value root;
            root["setid"] = (U32)iid;
            Query q = conn->query();
            q << "select * from se_trainingsets_result where set_id = " << iid;
            StoreQueryResult sqr = q.store();
            if (!sqr.size())
                throw seexception("Not result data defined for setid " + setId);
            Value& resultdata = root["result"];
            resultdata["type"] = (CSTR)sqr[0]["source"];
            resultdata["value"] = (U32)sqr[0]["result_id"];
            
            q = conn->query();
            q << "select * from se_trainingsets_attribs where set_id = " << iid;
            sqr = q.store();
            if (!sqr.size())
                throw seexception("Not attribs data defined for setid " + setId);
            Value& attribs = root["attribs"];
            for (int i=0; i != sqr.size(); ++i)
            {
                const Row& r = sqr[i];
                Value& attrib = attribs[i];
                attrib["type"] = (CSTR)r["source"];
                attrib["value"] = (CSTR)r["attrib_id"];
            }
            
            FastWriter writer;
            response().out() << writer.write(root);
            
        }
        

        void CAdminApp::ListTrainingSets() 
        {
            CSmartConnection conn(SECONN);
            Query q = conn->query();
            q << "SELECT * FROM sa.se_trainingsets order by set_name";
            Value root;
            Value& header = root[0];
            header[0] = "setid";
            header[1] = "name";
            header[2] = "desc";
            header[3] = "algo";
            header[4] = "default";
            int i=1;
            for (const Row& r : q.store())
            {
                Value& row = root[i++];
                row[0] = (U32)r["set_id"];
                row[1] = (CSTR)r["set_name"];
                row[2] = (CSTR)r["set_desc"];
                row[3] = (CSTR)r["set_algo"];
                row[4] = r["set_default"]=="1"?true:false;
            }
            
            FastWriter writer;
            response().out() << writer.write(root);
            
        }


        void CAdminApp::ListGeneralAccounts() 
        {
            CSmartConnection conn(SECONN);
            Query q = conn->query();
            q << "select match_id, match_name from se_fin_xbrls_match order by match_name";
            Value root;
            Value& header = root[0];
            header[0] = "MatchId";
            header[1] = "MatchName";
            int i=1;
            auto uqr = q.store();
            for (const Row& r : uqr)
            {
                Value& row = root[i++];
                row[0] = (string)r["match_id"];
                row[1] = (string)r["match_name"];
            }
            FastWriter writer;
            MakeSerializable<string> ser(writer.write(root));
            cache().store_data("ListGeneralAccounts", ser);
            response().out() << ser.get();
        }

        void CAdminApp::ListUsgaapAccounts() 
        {
            CSmartConnection conn(SECONN);
            Query q = conn->query();
            q << "SELECT * FROM sa.list_usgaapAccounts order by xbrl_name;";
            Value root;
            Value& header = root[0];
            header[0] = "xbrlid";
            header[1] = "xbrlname";
            header[2] = "xbrllabel";
            int i=1;
            auto uqr = q.store();
            for (const Row& r : uqr)
            {
                Value& row = root[i++];
                row[0] = (string)r["xbrl_id"];
                row[1] = (string)r["xbrl_name"];
                row[2] = (string)r["xbrl_label"];
            }
            FastWriter writer;
            MakeSerializable<string> ser(writer.write(root));
            cache().store_data("ListUsgaapAccounts", ser);
            response().out() << ser.get();
        }

        
        void CAdminApp::ListCommonlyMappedAccounts(string matchid) 
        {
            CSmartConnection conn(SECONN);
            Query q = conn->query();
            q << "SELECT a.match_id, a.xbrl_id, b.xbrl_name, b.xbrl_label ";
            q << "FROM sa.se_fin_xbrl_matched_ids a join list_usgaapAccounts b on a.xbrl_id = b.xbrl_id ";
            q << "where a.match_id = " << matchid << " and cik = 0 order by priority";

            Value root;
            Value& header = root[0];
            header[0] = "matchid";
            header[1] = "xbrlid";
            header[2] = "xbrlname";
            header[3] = "xbrllabel";
            int i=1;
            auto uqr = q.store();
            for (const Row& r : uqr)
            {
                Value& row = root[i++];
                row[0] = (string)r["match_id"];
                row[1] = (string)r["xbrl_id"];
                row[2] = (string)r["xbrl_name"];
                row[3] = (string)r["xbrl_label"];
            }

            FastWriter writer;
            response().out() << writer.write(root);
            
        }
        
        void CAdminApp::ListCommonlyMappedAccounts(string matchid, string cik) 
        {
            
            Value root;
            Value& header = root[0];
            header[0] = "matchid";
            header[1] = "xbrlid";
            header[2] = "xbrlname";
            header[3] = "xbrllabel";

            if (!cik.empty())
            {
                CSmartConnection conn(SECONN);
                Query q = conn->query();
                q << "SELECT a.match_id, a.xbrl_id, b.xbrl_name, b.xbrl_label ";
                q << "FROM sa.se_fin_xbrl_matched_ids a join list_usgaapAccounts b on a.xbrl_id = b.xbrl_id ";
                q << "where a.match_id = " << matchid << " and cik = " << cik << " order by priority";

                int i=1;
                auto uqr = q.store();
                for (const Row& r : uqr)
                {
                    Value& row = root[i++];
                    row[0] = (string)r["match_id"];
                    row[1] = (string)r["xbrl_id"];
                    row[2] = (string)r["xbrl_name"];
                    row[3] = (string)r["xbrl_label"];
                }
            }
            
            FastWriter writer;
            response().set_content_header("application/json");
            response().out() << writer.write(root);
        }        

        void CAdminApp::ListCompanyAccounts(string cik) 
        {
            CSmartConnection conn(SECONN);
            Value root;
            Query q = conn->query();
            q << "call list_CompanyAccounts(" << cik << ")";
            Value& header = root[0];
            header[01] = "xbrlid";
            header[1] = "xbrlprefix";
            header[2] = "xbrlname";
            int i=1;
            for (const Row& r : q.store())
            {
                Value& row = root[i++];
                row[0] = (string)r[0];
                row[1] = (string)r[1];
                row[2] = (string)r[2];
            }
            
            while (q.more_results())
                q.store_next();
            
            FastWriter writer;
            response().out() << root;
        }

        
        ///////////////// Page Methods ///////////////////////////
        
        void CAdminApp::Default()
        {
            AdminModel model;
            model.Init(context());
            model.Title = "Site Administration";
            render("AdminView", model);
        }

        void CAdminApp::CommonAccounts() 
        {
            AccountMapModel page;
            page.PageSelection = AccountMapModel::CommonAccounts;
            AdminModel model;
            model.Init(context(), &page);
            model.Title = "Account Mapping View - Manage common accounts";
            render("AdminView", model);
        }

        void CAdminApp::CommonAccountsAdd() 
        {
            if (request().request_method() == "POST")
            {
                string account = request().post("Account");
                CSmartConnection conn(SECONN);
                Query q = conn->query();
                q << "insert into se_fin_xbrls_match (match_name) values (" << quote << account << ")";
                Value root;
                Value& header = root[0];
                header[0] = "MatchId";
                header[1] = "MatchName";
                try
                {
                    q.exec();
                    Value& row = root[1];
                    row[0] = q.insert_id();
                    row[1] = account;
                    cache().rise("ListGeneralAccounts");
                }
                catch(const std::exception& ex)
                {
                    // nothing to do.
                }
                response().set_content_header("application/json");
                response().out() << root;
            }
        }
        
        void CAdminApp::MapCommonAccounts() 
        {
            AccountMapModel page;
            page.PageSelection = AccountMapModel::MapCommonAccounts;
            AdminModel model;
            model.Init(context(), &page);
            model.Title = "Account Mapping View - Map common accounts";
            render("AdminView", model);
        }

        void CAdminApp::MapCommonAccountsAdd() 
        {
            if (request().request_method() == "POST")
            {
                string caccount = request().post("caccount");
                string account = request().post("account");
                CSmartConnection conn(SECONN);
                Query q = conn->query();
                q << "insert into se_fin_xbrl_matched_ids (match_id, xbrl_id, priority, cik) ";
                q << "values (" << caccount << ", " << account << ", 0, 0)";
                q.exec();
            }
        }

        void CAdminApp::MapCommonAccountsRemove() 
        {
            if (request().request_method() == "POST")
            {
                string caccount = request().post("caccount");
                string account = request().post("account");
                
                // delete this combination
                CSmartConnection conn(SECONN);
                Query q = conn->query();
                q << "delete from se_fin_xbrl_matched_ids where match_id = " << caccount << " and xbrl_id = " << account << " and cik = 0 ";
                q.exec();
            }
        }

        void CAdminApp::MapCommonAccountsSort() 
        {
            if (request().request_method() == "POST")
            {
                string caccount = request().post("caccount");
                string accounts = request().post("accounts");
                char_separator<char> sep("|", "", keep_empty_tokens);
                typedef tokenizer< char_separator<char> > Tokenizer;
                typedef vector<string> TokenVector;
                Tokenizer tok(accounts, sep);
                int priority = 0;
                CSmartConnection conn(SECONN);
                for (auto it = tok.begin(); it != tok.end(); ++it)
                {
                    Query q = conn->query();
                    q << "update se_fin_xbrl_matched_ids set priority = " << priority++;
                    q << " where match_id = " << caccount << " and xbrl_id = " << *it ;
                    q << " and cik = 0 ";
                   // LOGIP(q.str());
                    q.exec();
                }
            }
        }

        
        void CAdminApp::MapSpecificAccounts() 
        {
            if (request().request_method()=="POST")
            {
                string matchid = request().post("match");
                string cik = request().post("cik");
                ListCommonlyMappedAccounts(matchid, cik);
            }
            else
            {
                AccountMapModel page;
                page.PageSelection = AccountMapModel::MapSpecificAccounts;
                AdminModel model;
                model.Init(context(), &page);
                model.Title = "Account Mapping View - Map company specific accounts";
                render("AdminView", model);
            }
        }

        void CAdminApp::MapSpecificAccountsAdd() 
        {
            if (request().request_method() == "POST")
            {
                string caccount = request().post("caccount");
                string account = request().post("account");
                string cik = request().post("cik");
                
                if (!cik.empty())
                {
                    CSmartConnection conn(SECONN);
                    Query q = conn->query();
                    q << "insert into se_fin_xbrl_matched_ids (match_id, xbrl_id, priority, cik) ";
                    q << "values (" << caccount << ", " << account << ", 0, " << cik << ")";
                    q.exec();
                }
            }

        }

        void CAdminApp::MapSpecificAccountsRemove() 
        {
            if (request().request_method() == "POST")
            {
                string caccount = request().post("caccount");
                string account = request().post("account");
                string cik = request().post("cik");
                if (!cik.empty())
                {
                    CSmartConnection conn(SECONN);
                    Query q = conn->query();
                    q << "delete from se_fin_xbrl_matched_ids ";
                    q << "where match_id = " << caccount << " and xbrl_id = " << account << " and cik = " << cik;
                    q.exec();
                }
            }
        }

        void CAdminApp::MapSpecificAccountsSort() 
        {
            if (request().request_method() == "POST")
            {
                string caccount = request().post("caccount");
                string accounts = request().post("accounts");
                string cik = request().post("cik");
                char_separator<char> sep("|", "", keep_empty_tokens);
                typedef tokenizer< char_separator<char> > Tokenizer;
                typedef vector<string> TokenVector;
                Tokenizer tok(accounts, sep);
                int priority = 0;
                CSmartConnection conn(SECONN);
                for (auto it = tok.begin(); it != tok.end(); ++it)
                {
                    Query q = conn->query();
                    q << "update se_fin_xbrl_matched_ids set priority = " << priority++;
                    q << " where match_id = " << caccount << " and xbrl_id = " << *it ;
                    q << " and cik = " << cik;
                   // LOGIP(q.str());
                    q.exec();
                }
            }
        }

        void CAdminApp::RecursiveOutputGicHierarchy( ISecurityAPI* papi, std::ostringstream& strm, unsigned long gicid, CSmartConnection& conn) 
        {
            using namespace cgicc;
            Query q = conn->query();
            q << "SELECT * FROM sa.se_sec_gics where gics_parent_id = " << gicid;
            StoreQueryResult sqr = q.store();
            if (sqr.size())
            {
                strm << "<ul>";
                for (const Row& r : sqr)
                {
                    strm << "<li " ;
                    strm << "id='gics_" << (UID)r["gics_id"] << "' " ;
                    strm << "iid='" << (UID)r["gics_id"] << "'>" ;
                    strm << (CSTR)r["gics_name"] ;
                    RecursiveOutputGicHierarchy(papi, strm, (UID)r["gics_id"], conn);
                    strm << "</li>";
                }
                strm << "</ul>";
            }
        }


        void CAdminApp::PredictionSetup(string pagename) 
        {
            PredictionSetupModel page;
            AdminModel model;
            model.Init(context(), &page);
            if (pagename == "general")
            {
                page.PageSelection = PredictionSetupModel::GeneralSetup;
                model.Title = "Prediction - General Setup";
            }
            else if (pagename == "tie")
            {
                page.PageSelection = PredictionSetupModel::Tie;
                model.Title = "Prediction Configuration - Tie to a Sector/Industry/Company";
                
                // output tree content for industry/sector.
                SP<ISecurityAPI> papi;
                THROWIFFAILED(SecOpen(&papi), "Failed to open security gateway.");
                CSmartConnection conn(SECONN);
                RecursiveOutputGicHierarchy(papi, page.GicsTree, 0, conn);
                
            }
            else //if (pagename == "test")
            {
                page.PageSelection = PredictionSetupModel::Test;
                model.Title = "Prediction Configuration - Test a Model";
            }
            render("AdminView", model);
       }

        void CAdminApp::SetupPrediction() 
        {
            if (request().request_method() == "POST")
            {
                // input looks like this
                // SetDetails = { 
                //      setid : 1, 
                //      result: { type: 'P', Value: 12 },
                //      attribs : [ { Type : 'E', Value: 11}, ... ]
                // }
                // SetId = 0 or 1,2,3    -- if setid = zero, it is new or update existing
                // SetName = 'Name of the set'
                // SetDesc = 'Desc of set'
                // SetAlgo = 'mvreg' or 'ann'
                
                string sId = request().post("SetId");
                string sName = request().post("SetName");
                string sDesc = request().post("SetDesc");
                string strainingset = request().post("SetDetails");
                
                Value trainingset;
                Json::Reader reader;
                reader.parse(strainingset, trainingset, false);
                
                string sAlgo = request().post("SetAlgo");
                string sdefault = request().post("SetIsDefault");
                
                if (sName.empty())
                    throw seexception("Training set name must be provided.");
                
                if (sAlgo == "")
                    sAlgo = "mvreg";
                UID iid = strtol(sId.c_str(), 0, 10);
                
                CSmartConnection conn(SECONN);
                if (sdefault=="true")
                {
                    Query q = conn->query();
                    q << "update se_trainingsets set set_default = '0' where set_default != '0'";
                    q.exec();
                }
                
                if (iid == 0)
                {
                    Query q = conn->query();
                    q << "INSERT INTO se_trainingsets (set_name, set_desc, set_algo, set_default) ";
                    q << " VALUES (" << quote << sName << ", " << quote << sDesc << ", " << quote << sAlgo << ", " << quote << (sdefault=="true" ? 1 : 0) << ")";
                   // LOGIP(q.str());
                    q.exec();
                    iid = q.insert_id();
                }
                else
                {
                    Query q = conn->query();
                    q << "UPDATE se_trainingsets SET ";
                    q << "set_name = " << quote << sName << ", ";
                    q << "set_desc = " << quote << sDesc << ", ";
                    q << "set_algo = " << quote << sAlgo << ", ";
                    q << "set_default = " << quote << (sdefault=="true" ? 1 : 0);
                    q << " where set_id = " << iid;
                   // LOGIP(q.str());
                    q.exec();
                }
                
                
                Query q = conn->query();
                q << "delete from se_trainingsets_attribs where set_id = " << iid;
               // LOGIP(q.str());
                q.exec();
                
                q = conn->query();
                q << "delete from se_trainingsets_result where set_id = " << iid;
               // LOGIP(q.str());
                q.exec();                
                
                // split attribs
                const Value& attribs = trainingset["attribs"];
                for (int i=0; i != attribs.size(); ++i)
                {
                    const Value& attrib = attribs[i];
                    q = conn->query();
                    q << "insert into se_trainingsets_attribs ( set_id, attrib_id, source) ";
                    q << "values (" << iid << ", " << attrib["value"].asString() << ", " << quote << attrib["type"].asString() << ")";
                   // LOGIP(q.str());
                    q.exec();
                }
                
                const Value& result = trainingset["result"];
                q = conn->query();
                q << "insert into se_trainingsets_result ( set_id, result_id, source) ";
                q << "values (" << iid << ", " << result["value"].asString() << ", " << quote << result["type"].asString() << ")";
               // LOGIP(q.str());
                q.exec();
                
                
            }
        }

        void CAdminApp::DeletePrediction() 
        {
            if (request().request_method() != "POST")
                throw seexception(E_HttpError, "Only post method is permitted.");
            
            string sId = request().post("SetId");
            CSmartConnection conn(SECONN);

            Query q = conn->query();
            q << "delete from se_trainingsets_attribs where set_id = " << sId;
           // LOGIP(q.str());
            q.exec();

            q = conn->query();
            q << "delete from se_trainingsets_result where set_id = " << sId;
           // LOGIP(q.str());
            q.exec();                
            
            q = conn->query();
            q << "delete from se_trainingsets where set_id = " << sId;
           // LOGIP(q.str());
            q.exec();                
            
       }
        
        void CAdminApp::ListAllPredictionSetup() 
        {
            if (request().request_method() != "POST")
                throw seexception(E_HttpError, "Only post method is permitted.");
            
            string matchId = request().post("matchid");
            CSmartConnection conn(SECONN);
            Query q = conn->query();
            q << "SELECT * FROM sa.se_sec_allpredictions where match_id = " << matchId;
           
            Value root;
            Value& header = root[0];
            header[0] = "matchid";
            header[1] = "setid";
            int i=1;
            for (const Row& r : q.store())
            {
                Value& row = root[i++];
                row[0] = (U32) r["match_id"];
                row[1] = (U32) r["tset_id"];
            }
            
            FastWriter writer;
            response().out() << writer.write(root);
            
            
        }

        void CAdminApp::ListGicsPredictionSetup() 
        {
            if (request().request_method() != "POST")
                throw seexception(E_HttpError, "Only post method is permitted.");
            
            string gicsId = request().post("gicsid");
            string matchId = request().post("matchid");

            CSmartConnection conn(SECONN);
            Query q = conn->query();
            q << "SELECT * FROM sa.list_PredictionSetupGics where gics_id = " << gicsId;
            q << " and match_id = " << matchId;
            
            Value root;
            Value& header = root[0];
            header[0] = "gicsid";
            header[1] = "matchid";
            header[2] = "setid";
            int i=1;
            for (const Row& r : q.store())
            {
                Value& row = root[i++];
                row[0] = (U32) r["gics_id"];
                row[1] = (U32) r["match_id"];
                row[2] = (U32) r["set_id"];
            }
            FastWriter writer;
            response().out() << writer.write(root);

        }

        void CAdminApp::ListProductPredictionSetup() 
        {
            if (request().request_method() != "POST")
                throw seexception(E_HttpError, "Only post method is permitted.");

            string prodId = request().post("prodid");
            string matchId = request().post("matchid");
            
            CSmartConnection conn(SECONN);
            Query q = conn->query();
            q << "SELECT * FROM sa.list_PredictionSetupProducts where product_id = " << prodId;
            q << " and match_id = " << matchId;
            Value root;
            Value& header = root[0];
            header[0] = "productid";
            int i=1;
            for (const Row& r : q.store())
            {
                Value& row = root[i++];
                row[0] = (U32)r["set_id"];
            }
            FastWriter writer;
            response().out() << writer.write(root);
        }

        void CAdminApp::SaveGicsPredictionSetup() 
        {
            if (request().request_method() != "POST")
                throw seexception(E_HttpError, "Only post method is permitted.");

            string data = request().post("data");
            
            if (data.empty())
                return;
            
            Value jdata;
            Reader reader;
            reader.parse(data, jdata, false);

            CSmartConnection conn(SECONN);
            
            // delete all existing
            Query q = conn->query();
            q << "delete from se_sec_gicspredictions where ";
            q << " gics_id = " << jdata["gicsid"];
            q << " and match_id = " << jdata["matchid"];
            q.exec();
            for (int i=0; i != jdata["setids"].size(); ++i)
            {
                q = conn->query();
                q << "insert into se_sec_gicspredictions values (" << jdata["gicsid"] << ", " << jdata["setids"][i] <<  ", " << jdata["matchid"] << ")";
                q.exec();
            }

        }

        void CAdminApp::SaveAllPredictionSetup() 
        {
            if (request().request_method() != "POST")
                throw seexception(E_HttpError, "Only post method is permitted.");

            string data = request().post("data");
            
            if (data.empty())
                return;
            
            Value jdata;
            Reader reader;
            reader.parse(data, jdata, false);
            
            // delete all existing
            CSmartConnection conn(SECONN);
            Query q = conn->query();
            q << "delete from se_sec_allpredictions where match_id = " << jdata["matchid"];
            q.exec();
            
            q = conn->query();
            q << "insert into se_sec_allpredictions values (" << jdata["setid"] << ", " << jdata["matchid"] << ")";
            q.exec();

        }
        
        void CAdminApp::SaveProductPredictionSetup() 
        {
            if (request().request_method() != "POST")
                throw seexception(E_HttpError, "Only post method is permitted.");
            
            string data = request().post("data");
            if (data.empty())
                return;
            
            Value jdata;
            Reader reader;
            reader.parse(data, jdata, false);
            
            // delete all existing
            CSmartConnection conn(SECONN);
            Query q = conn->query();
            q << "delete from se_sec_copredictions where product_id = " << jdata["productid"];
            q << " and match_id = " << jdata["matchid"];
            q.exec();
            for (int i=0; i != jdata["setids"].size(); ++i)
            {
                q = conn->query();
                q << "insert into se_sec_copredictions values (" << jdata["productid"] << ", " << jdata["setids"][i] << ", " << jdata["matchid"] << ")";
                q.exec();
            }
        }

        void CAdminApp::StepThruModelTesting() 
        {
            if (request().request_method() != "POST")
                throw seexception(E_HttpError, "Only post method is permitted.");
                
            string step = request().post("step");
            int istep = strtol(step.c_str(), 0, 10);
            UID prodId = strtol(request().post("prodid").c_str(), 0, 10);
            SP<ISecurityAPI> psecapi;
            THROWIFFAILED(SecOpen(&psecapi), "Failed to open security api.");
            
            switch (istep)
            {
                case 1:         // fetch training records.
                {
                    SGSP<IPredictionSetupList> pSetups;
                    THROWIFFAILED(
                            psecapi->get_SecurityPredictionSetups(prodId, 25, &pSetups), 
                            "Failed to obtain trainingset data for the security.");
                    
                    // obtain the extended training set from the first know prediction setup.
                    if (!pSetups->get_Count())
                        throw seexception(E_TrainingDataSetNotFound, "Extended trainingset not available.");
                    
                    SP<IPredictionSetup> pSetup;
                    THROWIFFAILED(pSetups->get_Item(0, &pSetup), "Error getting extended training dataset");

                    se::training::TRAINING_RESULT tresult = pSetup->get_ResultInfo();
                    if (tresult.Source == 'P')
                    {
                        SP<IXbrl> pxbrl;
                        if (ISOK(XbrlOpen(&pxbrl)))
                        {
                            SP<IString> pcik;
                            if (ISOK(pxbrl->get_CIK(prodId, &pcik)))
                            {
                                CDate dtNow = CDate::Now();
                                SP<IFinancialRatio> pratio;
                                if (ISOK(pxbrl->GetCachedFinancialPerformance(pcik->get_Buffer(), tresult.Identifier, &pratio)))
                                {
                                    
                                }
                            }
                        }
                    }                    
                    break;
                }
                case 2:         // run training records, get the ruleset information.
                {
                    SGSP<IPredictionSetupList> pSetups;
                    THROWIFFAILED(
                            psecapi->get_SecurityPredictionSetups(prodId, 25, &pSetups), 
                            "Failed to obtain trainingset data for the security.");
                    
                    // obtain the extended training set from the first know prediction setup.
                    if (!pSetups->get_Count())
                        throw seexception(E_TrainingDataSetNotFound, "Extended trainingset not available.");
                    
                    SP<IPredictionSetup> pSetup;
                    THROWIFFAILED(pSetups->get_Item(0, &pSetup), "Error getting extended training dataset");
                    
                    SP<IExTrainingTable> pExtTable;
                    THROWIFFAILED(pSetup->get_TrainingData(DATAPERIOD(), DATAPERIOD(), &pExtTable), "Failed to obtain extended table.");
                    
                    Value root;
                    root["setid"] = (U32)pSetup->get_Id();
                    root["setname"] = pSetup->get_Name();
                    
                    // write
                    pExtTable->Write("predictiondata.txt");
                    
                    R8 predValue = RunCubeModel(istep, pExtTable, root);
                    
                    // finally save the prediction
                    se::training::TRAINING_RESULT tresult = pSetup->get_ResultInfo();
                    if (tresult.Source == 'P')
                    {
                        SP<IXbrl> pxbrl;
                        if (ISOK(XbrlOpen(&pxbrl)))
                        {
                            SP<IString> pcik;
                            if (ISOK(pxbrl->get_CIK(prodId, &pcik)))
                            {
                                CDate dtNow = CDate::Now();
                                DATAPERIOD dp;
                                dp.Year = dtNow.get_Year();
                                dp.Quarter = dtNow.get_Quarter();
                                
                                SE_CODE retcode = pxbrl->SavePredictedFinancialPerformance(
                                        pcik->get_Buffer(),
                                        tresult.Identifier,
                                        dp,
                                        predValue
                                        );
                                if (FAILED(retcode))
                                {
                                   // LOGE(_logger, "Failed to save predicted value. Error code reported: " << retcode);
                                }
                            }
                        }
                    }
                    FastWriter writer;
                    response().set_content_header("application/json");
                    response().out() << writer.write(root);
                    break;
                }
                case 3:         // submit future values and rerun prediction formulat for each submited future periods.
                {
                    break;
                }
                case 4:         // 
                {
                    break;
                }
            }
        }

        R8 CAdminApp::RunCubeModel(int step, IExTrainingTable* pExTable, Value& root) 
        {
            using namespace se::math;
            
            CDate dtNow = CDate::Now();
            
            CCubist cube;
            COptions& options = cube.get_Options();
            //options.set_Option(OPT_AllowUnbiasedRules);
            options.set_Option(OPT_RulesInstanceAuto);
            options.set_NumberOfNearestNeighbors(9);
            //options.set_MaximumNumOfRules(500);
            options.set_ExtrapolationLimit(5);
            options.set_CrossValidationFolds(10);
            //options.set_Sampling(50);
            
            
            int attrCount = pExTable->get_AttributeFieldsCount();
            for (int i=0; i!=attrCount; ++i)
            {
                // add attributes
                CAttributeNumeric attr(pExTable->get_AttributeFieldName(i));
                cube.AddAttribute(attr);
            }
            CAttributeNumeric attrTarget("RESULT");
            cube.AddAttribute(attrTarget, true);

            cube.BeginLoadTrainData();
            std::ostringstream ostr;
            R8 predictedvalue ;
            R8 lastKnownresult;
            
//            double davg[4];
//            davg[0] = -38.618406;
//            davg[1] = 1.6744663999999996;
//            davg[2] = 6.771560166666667;
//            davg[3] = 36.9375484;
            
            for (IExTrainingRecord* prec : *pExTable)
            {
                const DATAPERIOD& dp = prec->get_Period();
                const DATAPERIOD& dpRecent = pExTable->get_RecentResultPeriod();

                CDate dt(dp.Year, dp.Month, dp.Day);
                //CDate dtTmp = dt;
                //dtTmp = dtTmp.AddSubQuarters(3);
                CQuarter qtr(dt);
                
                cube.BeginAddRecord();
                cube.FeedAttrValues("Quarter", 100 * qtr.get_Quarter());
                for (int i=0; i != pExTable->get_AttributeFieldsCount(); ++i)
                {
                    OmniType ot = prec->get_Value(i);
                    if (ot.IsEmpty())
                        cube.FeedAttrValues(pExTable->get_AttributeFieldName(i), "?");
                    else
                        cube.FeedAttrValues(pExTable->get_AttributeFieldName(i), (R8)prec->get_Value(i));
                }
                const OmniType& oresult = prec->get_Result();
                if (oresult.IsEmpty())
                    cube.FeedAttrValues("RESULT", "?");
                else
                {                        
                    lastKnownresult = (R8)oresult;
                    cube.FeedAttrValues("RESULT", lastKnownresult);
                }
                cube.EndAddRecord();
            }
            cube.EndLoadTrainData();
            root["ruleset"] = cube.get_RuleText() ;
            root["knownresult"] = lastKnownresult;

            cube.BeginPredict();
            cube.FeedAttrValues("Quarter", 100 * dtNow.get_Quarter());

            SP<IExTrainingRecord> pfut;
            if (ISOK(pExTable->get_FutureRecord(&pfut)))
            {
                for (int i=0; i != pExTable->get_AttributeFieldsCount(); ++i)
                {
                    const auto& fv = pfut->get_Value(i);
                    if (fv.IsEmpty())
                        cube.FeedAttrValues(pExTable->get_AttributeFieldName(i), "?");
                    else
                        cube.FeedAttrValues(pExTable->get_AttributeFieldName(i), (R8)pfut->get_Value(i));
                }
            }
            else
            {
                for (int i=0; i != pExTable->get_AttributeFieldsCount(); ++i)
                {
                    cube.FeedAttrValues(pExTable->get_AttributeFieldName(i), "?");
                }
            }
            predictedvalue = cube.EndPredict();
            root["predictedValue"] = predictedvalue;
            
            if (pfut)
                pfut->set_Result(predictedvalue);

            return predictedvalue;
        }


        void CAdminApp::ListEconIndicatorCategories() 
        {

            try
            {
                SP<IEconomyApi> papi;
                THROWIFFAILED(EconOpen(&papi), "Econ api failed to open.");
                SP<IIndicatorCategoryList> pCatList;
                THROWIFFAILED(papi->get_IndicatorCategories(&pCatList),"Failed to get indicator categories from library.");

                Value root;
                Value& header = root[0];
                header[0] = "Id";
                header[1] = "Name";
                header[2] = "Description";
                header[3] = "IsDisplayed";
                int i=1;
                for (IIndicatorCategory* pcat : *pCatList)
                {
                    Value& row = root[i++];
                    row[0] = (I32)pcat->get_Id();
                    row[1] = pcat->get_Name();
                    row[2] = pcat->get_Description();
                    row[3] = pcat->get_IsDisplayed();
                }
                FastWriter fw;
                MakeSerializable<string> dataser(fw.write(root));
                cache().store_data("ListEconIndicatorCategories", dataser, 7200);
                
                response().out() << dataser.get();
                
            }
            catch(const seexception& ex)
            {
               // LOGE(_logger, "ListEconIndicatorCategories failed. Error: " << ex.what());
                response().out() << "{\"Status\":0}";
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "ListEconIndicatorCategories failed. Error: " << ex.what());
                response().out() << "{\"Status\":0}";
            }            
        }

        void CAdminApp::ListEconIndicator(unsigned long ecoId) 
        {
            
            SP<IEconomyApi> papi;
            THROWIFFAILED(EconOpen(&papi), "Econ api failed to open.");
            SP<IEconomyIndicator> p;
            THROWIFFAILED(papi->get_EconomicIndicator(ecoId, &p), "Error when retrieving economic indicator details.");
            
            Value root;
            Value& header = root[0];
            header[0] = "Id";
            header[1] = "Name";
            header[2] = "Description";
            header[3] = "IsDisplayed";
            header[4] = "PublishPeriod";
            header[5] = "RecentPeriod";
            header[6] = "RecentValue";
            header[8] = "Source";
            header[9] = "Category";
            header[10] = "Table";
            header[11] = "IsSeasonal";
            header[12] = "Copyright";
            header[13] = "DownloadLink";
            header[14] = "ReleaseDay";
            header[15] = "CatId";
            header[16] = "Determiner";

            Value& row = root[1];
            row[0] = (I32)p->get_Id();
            row[1] = p->get_Name();
            row[2] = p->get_Description();
            row[3] = p->get_IsDisplayed();
            row[4] = p->get_PublishPeriod();
            try
            {
                auto tpl = papi->get_EconomicIndicatorLatestValue(p->get_Id());
                std::ostringstream ostr;
                ostr << get<0>(tpl);
                row[5] = ostr.str();
                row[6] = get<1>(tpl);
            }
            catch(...)
            {
                row[5] = "";
                row[6] = 0;
                row[7] = 0;
            }
            row[8] = p->get_Source();
            SP<IIndicatorCategory> pInd;
            if (ISOK(p->get_Category(&pInd)))
            {
                row[9] = pInd->get_Name();
                row[15] = (Json::UInt64)pInd->get_Id();
            }
            else
                row[9] = "";

            row[10] = p->get_TableName();
            row[11] = p->get_IsSeasonal();
            row[12] = p->get_CopyRight();
            row[13] = p->get_DownloadLink();
            row[14] = p->get_ReleaseDates();
            row[16] = (I16)p->get_Determiner();

            FastWriter fw;
            response().set_content_header("application/json");
            response().out() << fw.write(root);   
        }

        
        void CAdminApp::ListEconIndicators(unsigned long catId) 
        {
            try
            {
                SP<IEconomyApi> papi;
                THROWIFFAILED(EconOpen(&papi), "Econ api failed to open.");
                SP<IEconomyIndicatorList> pIndicators;
                
                if (catId == 0)
                {
                    THROWIFFAILED(papi->get_EconomicIndicators(
                    &pIndicators,
                    nullptr
                    ), "Failed to get economic indicators from library.");
                }
                else
                {
                    THROWIFFAILED(papi->get_EconomicIndicators(
                    &pIndicators,
                    [catId](IEconomyIndicator::QUERYFLDS f, LGX& l, BOOL& i)
                    {
                        if (f == IEconomyIndicator::CategoryId)
                        {
                            i = true;
                            std::stringstream str;
                            str << catId;
                            return str.str().c_str();
                        }
                        return "";
                    }
                    ), "Failed to get economic indicators from library.");
                }
                Value root;
                Value& header = root[0];
                header[0] = "Id";
                header[1] = "Name";
                header[2] = "Description";
                header[3] = "IsDisplayed";
                header[4] = "PublishPeriod";
                header[5] = "RecentPeriod";
                header[6] = "RecentValue";
                header[8] = "Source";
                header[9] = "Category";
                header[10] = "Table";
                header[11] = "IsSeasonal";
                header[12] = "Copyright";
                header[13] = "DownloadLink";
                header[14] = "ReleaseDay";
                header[15] = "CatId";
                header[16] = "Determiner";
                int i=1;
                for (IEconomyIndicator * p : *pIndicators)
                {
                    Value& row = root[i++];
                    row[0] = (I32)p->get_Id();
                    row[1] = p->get_Name();
                    row[2] = p->get_Description();
                    row[3] = p->get_IsDisplayed();
                    row[4] = p->get_PublishPeriod();
                    
                    try
                    {
                        auto tpl = papi->get_EconomicIndicatorLatestValue(p->get_Id());
                        std::ostringstream ostr;
                        ostr << get<0>(tpl);
                        row[5] = ostr.str();
                        row[6] = get<1>(tpl);
                    }
                    catch(...)
                    {
                        row[5] = "";
                        row[6] = 0;
                        row[7] = 0;
                    }
                    row[8] = p->get_Source();
                    SP<IIndicatorCategory> pInd;
                    if (ISOK(p->get_Category(&pInd)))
                    {
                        row[9] = pInd->get_Name();
                        row[15] = (Json::UInt64)pInd->get_Id();
                    }
                    else
                        row[9] = "";
                    
                    row[10] = p->get_TableName();
                    row[11] = p->get_IsSeasonal();
                    row[12] = p->get_CopyRight();
                    row[13] = p->get_DownloadLink();
                    row[14] = p->get_ReleaseDates();
                    row[16] = (I16)p->get_Determiner();
                }
                FastWriter fw;
                response().out() << fw.write(root);

            }
            catch(const seexception& ex)
            {
               // LOGE(_logger, "ListEconIndicators failed. Error: " << ex.what());
                response().out() << "{\"Status\":0}";
            }
            catch(const std::exception& ex)
            {
               // LOGE(_logger, "ListEconIndicators failed. Error: " << ex.what());
                response().out() << "{\"Status\":0}";
            }            
        }
        
        void CAdminApp::EconomyManageAdd() 
        {
            if (request().request_method() != "POST")
                throw seexception(E_HttpError, "Only post method is permitted.");

            string eid = request().post("eid");
            string countryid = request().post("countryid");
            string stateid = request().post("stateid");
            string year = request().post("year");
            string month = request().post("month");
            string day = request().post("day");
            string value = request().post("value");
            
            CSmartConnection conn(SECONN);
            Query q = conn->query();
            q << "select TableName from se_eco_indicators where ei_id = " << eid;
            StoreQueryResult sqr = q.store();
            if (!sqr.size())
                throw seexception(E_NotFound, "Unknown economic indicator.");
            string tableName = (CSTR)sqr[0][0];
            
            q = conn->query();
            q << "insert into " << tableName ;
            q << " (country_id, state_id, year, month, day, value ) ";
            q << " values (";
            q << countryid << ", ";
            q << stateid << ", ";
            q << year << ", ";
            q << month << ", ";
            q << day << ", ";
            q << value;
            q << ")";
           // LOGIP(q.str());

            q.exec();
            
            cache().rise("ListEconIndicators" + eid);
            

        }

        void CAdminApp::EconomyManageEdit() 
        {
            if (request().request_method() != "POST")
                throw seexception(E_HttpError, "Only post method is permitted.");
            
            string eid = request().post("eid");
            string countryid = request().post("countryid");
            string stateid = request().post("stateid");
            string year = request().post("year");
            string month = request().post("month");
            string day = request().post("day");
            string value = request().post("value");
            
            CSmartConnection conn(SECONN);
            Query q = conn->query();
            q << "select TableName from se_eco_indicators where ei_id = " << eid;
            StoreQueryResult sqr = q.store();
            if (!sqr.size())
                throw seexception(E_NotFound, "Unknown economic indicator.");
            string tableName = (CSTR)sqr[0][0];
            
            q = conn->query();
            q << "update " << tableName << " set ";
            q << "value = " << value ;
            q << " where year = " << year; 
            q << " and month = " << month;
            q << " and day = " << day;
            q << " and state_id = " << stateid;
            q << " and country_id = " << countryid;
           // LOGIP(q.str());
            q.exec();
            
            cache().rise("ListEconIndicators" + eid);
            
        }

        void CAdminApp::EconomyGetPredictedValue() 
        {
            if (request().request_method() != "POST")
                throw seexception(E_HttpError, "Only post method is permitted.");
            
            // received data form
            /* 
             * param = 
             * {
             *      eid : 1,
             *      countryid : 1,
             *      stateid : 1, 
             *      year : 2014,
             *      month : 1,
             *      day : 1,
             *      predalgo : 'LR'
             * }
             */
            
            string param = request().post("params");
            if (param.empty())
                throw seexception(E_HttpError, "Posted parameter 'param' is empty.");
            
            Reader reader;
            Value jparam;
            reader.parse(param, jparam, false);
            string predalgo = jparam["predalgo"].asString();
            UID eid = strtol(jparam["eid"].asCString(), 0, 10);
            // get the training table 
            SP<IEconomyApi> pEcoApi;
            THROWIFFAILED(EconOpen(&pEcoApi), "Failed to open economy api.");
            SP<ITrainingTable> pTable;
            DATAPERIOD dpFrom;
            dpFrom.Year = 2008;
            dpFrom.Month = 1;
            dpFrom.Day = 1;
            THROWIFFAILED(pEcoApi->get_EconomicIndicatorTrainingTable(
                        eid, strtol(jparam["countryid"].asCString(), 0, 10), strtol(jparam["stateid"].asCString(), 0, 10), 
                        dpFrom, DATAPERIOD(), &pTable), "Failed to get economy indicator data table.");
            //load in the regression function.
            R8 c0, c1, cov0, cov1, cov2, chisq;
            
            if (predalgo == "LR")
            {
                THROWIFFAILED(pEcoApi->get_LinearFitEquation(
                    pTable, &c0, &c1, &cov0, &cov1, &cov2, &chisq), "Error while running regression function");
            }
            else if (predalgo == "WLR")
            {
                THROWIFFAILED(pEcoApi->get_WeightedLinearFitEquation(
                    pTable, &c0, &c1, &cov0, &cov1, &cov2, &chisq), "Error while running regression function");
            }
            else    // until fully implemeted
            {
                response().out() << 0.0;
                return;
            }
            
            // return predicted value.
            DATAPERIOD dpParam;
            dpParam.Year = strtol(jparam["year"].asCString(), 0, 10);
            dpParam.Month = strtol(jparam["month"].asCString(), 0, 10);
            dpParam.Day = strtol(jparam["day"].asCString(), 0, 10);
            response().out() << (c0 + (c1 * PERIODMASK(dpParam)));
       }

        void CAdminApp::GetRatioGroups() 
        {
            try
            {
                SP<IXbrl> papi;
                THROWIFFAILED(XbrlOpen(&papi), "Failed to open fin gateway.");
                SP<IFinancialRatioGroupList> pl;
                THROWIFFAILED(papi->GetRatioGroups(&pl), "Failed to get json output for ratio groups.");
                Value root;
                Value& header = root[0];
                header[0] = "Id";
                header[1] = "Name";
                int i = 1;
                for (IFinancialRatioGroup* g : *pl)
                {
                    Value& row = root[i++];
                    row[0] = (U32)g->get_Id();
                    row[1] = g->get_Name();
                }
                
                FastWriter writer;
                response().out() << writer.write(root);
            }
            catch(const std::exception ex)
            {
                throw ex;
            }
        }
        void CAdminApp::GetRatios(unsigned long groupid) {
         try
            {
                SP<IXbrl> papi;
                THROWIFFAILED(XbrlOpen(&papi), "Failed to open fin gateway.");
                SP<IFinancialRatioList> pl;
                THROWIFFAILED(papi->GetRatiosForGroup(groupid, &pl), "Failed to get json output for ratios.");
                Value root;
                Value& header = root[0];
                header[0] = "Id";
                header[1] = "Name";
                int i = 1;
                ostringstream ostr;
                for (IFinancialRatio* p : *pl)
                {
                    Value& row = root[i++];
                    row[0] = (U32)p->get_Id();
                    row[1] = p->get_Name();
                    ostr.str("");
                    ostr << "Description: " << p->get_Description() << std::endl;
                    if (p->get_MinimumPoint() != MINR8)
                    {
                        ostr << "Valid Range is between ";
                        ostr << p->get_MinimumPoint();
                        ostr << " and ";
                        ostr << p->get_MaximumPoint() << ". ";
                    }
                    if (p->get_MiddlePoint() != ER8)
                    {
                        ostr << "The value of a performing company is close to ";
                        ostr << p->get_MiddlePoint() << ".";
                    }
                    row[2] = ostr.str();
                }

                FastWriter writer;
                response().out() << writer.write(root);

            }
            catch(const std::exception ex)
            {
                throw ex;
            }

        }
        
        
        void CAdminApp::ManageEconomicIndicators() 
        {
            if (request().request_method() == "POST")
            {
                string eiid = request().post("eiid");
                if (!eiid.empty())
                {
                    SP<IEconomyApi> papi;
                    THROWIFFAILED(EconOpen(&papi), "Failed to open economy api.");
                    SP<IEconomyIndicator> p;
                    THROWIFFAILED(IEconomyIndicator::Create(&p), "Failed to create a new instance of indicator.");
                    p->set_Id(strtol(eiid.c_str(), nullptr, 10));
                    p->set_Name(request().post("name").c_str());
                    p->set_Description(request().post("desc").c_str());
                    p->set_TableName(request().post("tablename").c_str());
                    p->set_CopyRight(request().post("copyright").c_str());
                    p->set_Source(request().post("sourcelink").c_str());
                    p->set_DownloadLink(request().post("downloadlink").c_str());
                    p->set_ReleaseDates(request().post("releaseday").c_str());
                    p->set_IsDisplayed( request().post("isdisplayed")=="true" ? true : false );
                    p->set_IsSeasonal(request().post("isseasonal")=="true" ? true : false );
                    p->set_Category(strtol(request().post("category").c_str(), nullptr, 10));
                    p->set_PublishPeriod(request().post("publishperiod").c_str());
                    p->set_Determiner((DET)strtol(request().post("determiner").c_str(), nullptr, 10));

                    THROWIFFAILED(papi->set_EconomicIndicator(p), "Failed to save economic indicator.");
                    ListEconIndicator(p->get_Id());
                    return;
                }
                
            }
            
            EconomicIndicatorManagementModel page;
            AdminModel model;
            model.Init(context(), &page);
            model.Title = "Manage Economic Indicators";
            render("AdminView", model);
        }

        void CAdminApp::GetLatestAnnualAccountValue() 
        {
            if (request().request_method() != "POST")
                throw seexception("Invalid operation. Only post methods are accepted.");
            
            string cik = request().post("cik");
            string xbrlid = request().post("xid");
            
            if (cik.empty() || xbrlid.empty())
                throw seexception("Invalid posted parameters.");
            
            // load consolidated financials
            SP<IXbrl> pxbrl;
            THROWIFFAILED(XbrlOpen(&pxbrl), "Failed to open xbrl api");
            
            // we have globally turned off at api level all caches. but consolidated finance
            // is an expensive operation. we should cache it on the client side.
            SGSP<IFinancialsConsolidated> pcon;
            if (!cache().fetch_data(cik, pcon))
            {
                THROWIFFAILED(pxbrl->GetConsolidatedFinancials(cik.c_str(), &pcon), "No consolidated view definied.");
                cache().store_data(cik, pcon, 600);
            }
            pcon->AddRef();

            UID xid = strtol(xbrlid.c_str(), 0, 10);
            SP<IXbrlRecord> prec;
            THROWIFFAILED( pcon->get_MatchedAccountRecord(xid, &prec), "Failed to get xbrl record from consolidated view");
            
            
            SP<IXbrlField> pfld;
            THROWIFFAILED(prec->get_Financials(pcon->get_FinancialYearEnd().get_Year(), &pfld), "Failed to get financial field for given year from consolidated view");
            
            const OmniType& val = pfld->get_FY();
            
            Value root;
            root["cik"] = cik;
            root["matchid"] = (U32)xid;
            root["year"] = pcon->get_FinancialYearEnd().get_Year();
            Value& v = root["value"];
            if (val.dt == DT_R8)
                v["FY"] = (R8)val;
            else if (val.dt == DT_I64)
                v["FY"] = (Json::Int64)(I64)val;
            else
            {
                std::ostringstream ostr;
                ostr << val;
                v["FY"] = ostr.str();
            }
            
            for (int q=1; q<=4; ++q)
            {
                std::ostringstream oqstr;
                oqstr << "Q" << q;
                const OmniType& qval = pfld->get_Q(q);
                if (val.dt == DT_R8)
                    v[oqstr.str()] = (R8)qval;
                else if (val.dt == DT_I64)
                    v[oqstr.str()] = (Json::Int64)(I64)qval;
                else
                {
                    std::ostringstream ostr;
                    ostr << qval;
                    v[oqstr.str()] = ostr.str();
                }
            }
            
            FastWriter writer;
            response().out() << writer.write(root);
        }

        
        
    }
}