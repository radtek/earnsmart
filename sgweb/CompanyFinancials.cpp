
//#include <log4cxx/propertyconfigurator.h>

#include "models.h"
#include "sgwebinternal.h"
#include "../seglobal/exceptions.h"
#include <cgicc/HTMLClasses.h>

using namespace cgicc;

namespace sg
{
    namespace web
    {
        namespace models
        {
            CompanyFinancials::CompanyFinancials(cppcms::http::context& ctxt) : 
                Fundamentals(ctxt, "CompanyFinancialsView"), HasStatements(false), TabSelection("S")
            {
                //_logger = Logger::getLogger("sg.web.companyfinancials");
            }

            void CompanyFinancials::Init(UID prodId) 
            {
//                try
//                {
                    Fundamentals::Init(prodId);
                    
                    SP<IXbrl> papi;
                    THROWIFFAILED(XbrlOpen(&papi), "xbrl api cannot be obtained.");
                    SP<IString> pcik;
                    if (S_False == papi->get_CIK(prodId, &pcik))
                        throw seexception(E_NotFound, "CIK not found.");
                    
                    SP<IFinancialsConsolidated> pcon;
                    if (ISOK(papi->GetConsolidatedFinancials(pcik->get_Buffer(), &pcon)))
                    {
                        // get statements
                        if (ISOK(papi->GetFinancialStatements(pcik->get_Buffer(), &_pstmts)))
                        {
                            if (_pstmts->get_Count())
                            {
                                HasStatements = true;
                                _pstmts->get_Item(0, &_pcurstmt);
                            }
                        }
                    }
//                }
//                catch(const seexception& ex)
//                {
//                    LOGE(_logger, "CompanyFinancials::Init failed. Unknown error. " << ex.what());
//                    throw ex;
//                }
//                catch(const std::exception& ex)
//                {
//                    LOGE(_logger, "CompanyFinancials::Init failed. Unknown error. " << ex.what());
//                    throw ex;
//                }
            }

            IXbrlStatement* CompanyFinancials::get_CurrentStatement() const
            {
                return _pcurstmt;
            }
            
            IXbrlStatementList& CompanyFinancials::get_Statements() const
            {
                return *_pstmts;
            }

            void CompanyFinancials::SetCurrentStatement(unsigned long statementId) 
            {
                if (_pstmts->get_Count())
                {
                    _pcurstmt.Clear();
                    THROWIFFAILED(_pstmts->get_ItemById(statementId, &_pcurstmt), "Failed to set current statement.");
                }
            }

//
//            IXbrlRecordList* CompanyFinancials::get_IncomeRecords() 
//            {
//                return _pincomeList;
//            }
            
            // cgicc allows this to add new tags
            BOOLEAN_ELEMENT (text,        "text");        // block-level grouping

            void CompanyFinancials::WriteNode(std::ostream& os, int level, IXbrlAbstractRecord* node, bool isLastItem, I16 sy, I16 ey) const
            {
               // LOGI(_logger, "Node Name: " << node->get_Name() << " and Label : " << node->get_XbrlLabel());
                if (strstr(node->get_Name(), "Axis") || strstr(node->get_Name(), "Domain"))
                    return;
                
                struct my_numpunct : std::numpunct<char> 
                {
                    std::string do_grouping() const 
                    {
                        return "\03";
                    }
                    ~my_numpunct()
                    {
                    }
                };  
                
                std::locale orig = os.getloc();
                std::locale loc(orig, new my_numpunct());
                os.imbue(loc);
                
                bool colorQuarterRow = false;
                bool absfill = false;
                stringstream xbrlid;
                xbrlid << node->get_Id();
                if (node->get_IsAbstract())
                {
                    os << tr().set("style", "background: #94AF2C;");
                    // td start
                    stringstream strpadding;
                    strpadding << level * 10 << "px";
                    os << th().set("rowspan", "2")
                            .set("class", "fdm_fin_table_td_account")
                            .set("style", "padding-left: " + strpadding.str() + ";");
                    os << label().set("style", "color:blue")
                            .set("title", xbrlid.str() + "/" + node->get_Name());
                   // LOGI(_logger, "Xbrl Label : " << node->get_XbrlLabel());
                    os << node->get_XbrlLabel() ;
                    os << label();
                    //node->set_Property("color", "blue");
                    os << th(); // td end
                    
                    colorQuarterRow = true;
                    absfill = true;
                }
                else
                {
                    IXbrlAbstractRecord* pParent = node->get_Parent();
                    if (!pParent)
                    {
                        os << tr();
                        // td start
                        stringstream strpadding;
                        strpadding << level * 10 << "px";
                        os << th().set("rowspan", "2")
                                .set("class", "fdm_fin_table_td_account")
                                .set("style", "padding-left: " + strpadding.str() + ";")
                                .set("title", xbrlid.str() + "/" + node->get_Name());
                        os << node->get_XbrlLabel() ;
                        os << th(); // td end
                    }
                    else
                    {
                        string pname = pParent->get_Name();
                        // remove abstract
                        pname = pname.substr(0, pname.find("Abstract"));
                        if (!strcmp(pname.c_str(), node->get_Name()) || isLastItem)
                        {
                            os << tr().set("style", "background: #DAF572;");
                            // td start
                            stringstream strpadding;
                            strpadding << level * 10 << "px";
                            os << th().set("rowspan", "2")
                                    .set("class", "fdm_fin_table_td_account")
                                    .set("style", "padding-left: " + strpadding.str() + ";")
                                    .set("title", xbrlid.str() + "/" + node->get_Name());

                            os << label().set("style", "color:blue");
                            os << node->get_XbrlLabel() ;
                            os << label();
                            os << th(); // td end

                            colorQuarterRow = true;

                        }
                        else
                        {
                            
                            os << tr();
                            // td start
                            stringstream strpadding;
                            strpadding << level * 10 << "px";
                            os << th().set("rowspan", "2")
                                    .set("class", "fdm_fin_table_td_account")
                                    .set("style", "padding-left: " + strpadding.str() + ";")
                                    .set("title", xbrlid.str() + "/" + node->get_Name());
                            os << node->get_XbrlLabel() ;
                            os << th(); // td end
                        }
                    }
                }
                
                for (I16 y = ey; y >= sy; --y)
                {
                    if (!node->get_IsAbstract())
                    {
                        IXbrlRecord* prec = static_cast<IXbrlRecord*>(node);
                        SP<IXbrlField> pfld;
                        prec->get_Financials(y, &pfld);
                        xbrlid.str("");
                        xbrlid << pfld->get_FY();
                        os << td()
                                .set("colspan", "4").set("class", "fdm_fin_table_td_year")
                                .set("title", xbrlid.str());
                        WriteFormattedDouble(os, pfld->get_FY(), 2);
                        os << td();
                    }
                    else
                    {
                        if (absfill)
                        {
                            os << td().set("colspan", "4")
                                    .set("class", "fdm_fin_table_td_abstract_empty_year");
                            os << td();
                        }
                        else
                        {
                            os << td().set("colspan", "4")
                                    .set("class", "fdm_fin_table_td_abstract_empty_year");
                            os << td();
                        }
                    }
                }
                os << tr();

                os << tr();
                for (I16 y = ey; y >= sy; --y)
                {
                    if (!node->get_IsAbstract())
                    {
                        IXbrlRecord* prec = static_cast<IXbrlRecord*>(node);
                        SP<IXbrlField> pfld;
                        prec->get_Financials(y, &pfld);
//                        
//#ifdef __DEBUG__                
//                        if (node->get_Id() == 18733 && y == 2014)
//                        {
//                            LOGI(_logger, "Quarter 1 value: " << pfld->get_Q1());
//                        }
//#endif
                        
                        
                        if (colorQuarterRow )
                        {
                            xbrlid.str("");
                            xbrlid << pfld->get_Q4();
                            os << td()
                                    .set("class", "fdm_fin_table_td_quarter")
                                    .set("style", "background: #DAF572;")
                                    .set("title", xbrlid.str());
                            WriteFormattedDouble(os, pfld->get_Q4(), 2);
                            os << td();
                            
                            xbrlid.str("");
                            xbrlid << pfld->get_Q3();
                            os << td()
                                    .set("class", "fdm_fin_table_td_quarter")
                                    .set("style", "background: #DAF572;")
                                    .set("title", xbrlid.str());
                            WriteFormattedDouble(os, pfld->get_Q3(), 2);
                            os << td();

                            xbrlid.str("");
                            xbrlid << pfld->get_Q2();
                            os << td()
                                    .set("class", "fdm_fin_table_td_quarter")
                                    .set("style", "background: #DAF572;")
                                    .set("title", xbrlid.str());
                            WriteFormattedDouble(os, pfld->get_Q2(), 2);
                            os << td();
                            
                            xbrlid.str("");
                            xbrlid << pfld->get_Q1();
                            os << td()
                                    .set("class", "fdm_fin_table_td_quarter")
                                    .set("style", "background: #DAF572;")
                                    .set("title", xbrlid.str());
                            WriteFormattedDouble(os, pfld->get_Q1(), 2);
                            os << td();
                        }
                        else
                        {
                            xbrlid.str("");
                            xbrlid << pfld->get_Q4();
                            os << td()
                                    .set("class", "fdm_fin_table_td_quarter")
                                    .set("title", xbrlid.str());
                            WriteFormattedDouble(os, pfld->get_Q4(), 2);
                            os << td();

                            xbrlid.str("");
                            xbrlid << pfld->get_Q3();
                            os << td()
                                    .set("class", "fdm_fin_table_td_quarter")
                                    .set("title", xbrlid.str());
                            WriteFormattedDouble(os, pfld->get_Q3(), 2);
                            os << td();

                            xbrlid.str("");
                            xbrlid << pfld->get_Q2();
                            os << td()
                                    .set("class", "fdm_fin_table_td_quarter")
                                    .set("title", xbrlid.str());
                            WriteFormattedDouble(os, pfld->get_Q2(), 2);
                            os << td();

                            xbrlid.str("");
                            xbrlid << pfld->get_Q1();
                            os << td()
                                    .set("class", "fdm_fin_table_td_quarter")
                                    .set("title", xbrlid.str());
                            WriteFormattedDouble(os, pfld->get_Q1(), 2);
                            os << td();
                        }
                    }
                    else
                    {
                        os << td()
                                .set("class", "fdm_fin_table_td_abstract_empty_quarter");
                        os << td();
                        
                        os << td()
                                .set("class", "fdm_fin_table_td_abstract_empty_quarter");
                        os << td();
                        
                        os << td()
                                .set("class", "fdm_fin_table_td_abstract_empty_quarter");
                        os << td();
                        
                        os << td()
                                .set("class", "fdm_fin_table_td_abstract_empty_quarter");
                        os << td();
                    }
                }
                os << tr();
                
                SP<IXbrlRecordList> psubitems;
                if (ISOK(node->get_Children(&psubitems)))
                {
                    int count = psubitems->get_Count();
                    int i = 0;
                    for (auto it = psubitems->begin(); it != psubitems->end(); ++it, i++)
                    {
                        WriteNode(os, level + 1, *it, i == (count-1), sy, ey);
                        i++;
                    }
                }
                os.imbue(orig);
            }
            
            std::ostream& operator<<(std::ostream& os, const CompanyFinancials& cfin)
            {
                string fintableframe;
                ostringstream ostrframename;
                ostrframename << "companyfinancials" << cfin.ProductId << cfin._pcurstmt->get_Id();
                if (cfin.httpctxt->cache().fetch_frame(ostrframename.str(), fintableframe))
                {
                    os << fintableframe;
                    return os;
                }
                
                cppcms::copy_filter tee(os);                  
                
                if(!cfin.HasStatements)
                {
                    os  << cgicc::div().set("class", "statuslabel") 
                        << "No statement available for this company. Sorry for the inconvenience."
                        << cgicc::div();
                }
                else if (!cfin.get_CurrentStatement())
                {
                    os  << cgicc::div().set("class", "statuslabel")
                        << "A statement is not selected. Use the drop down above to select a statement."
                        << cgicc::div();
                }
                else
                {
                    // recursively loop until first 'StatementTable' is located 
                    SP<IXbrlAbstractRecord> proot;
                    IXbrlStatement* pstmt = cfin.get_CurrentStatement();
                    if (FAILED(pstmt->get_Tree(&proot)) || !proot)
                    {
                        os  << cgicc::div().set("class", "statuslabel")
                            << "Selected statement has no data."
                            << cgicc::div();
                        return os;
                    }
                    SP<IXbrlRecordList> precs;
                    if (FAILED(proot->get_Children(&precs)))
                    {
                        os  << cgicc::div().set("class", "statuslabel")
                            << "Selected statement has no data."
                            << cgicc::div();
                        return os;
                    }
                    
                    //ostringstream ostr;
                    ostream& ostr = os;
                    
                    // check if statement table element is available.
                    ostr << table().set("class", "fdm_fin_table").set("cellspacing", "10");
                    ostr << tr();
                    ostr << th().set("rowspan", "2").set("class", "fdm_fin_table_th_account") << "Accounts" << th();
                    for (I16 y = pstmt->get_HighestYear(); y >= pstmt->get_LowerYear(); --y)
                    {
                        ostr << th().set("colspan", "4").set("class", "fdm_fin_table_th_year") << y << th();
                    }
                    ostr << tr();
                    ostr << tr();
                    for (I16 y = pstmt->get_HighestYear(); y >= pstmt->get_LowerYear(); --y)
                    {
                        for (int i=4; i>= 1; --i)
                        {
                            ostr << th().set("class", "fdm_fin_table_th_quarter") << "Q" << i << th();
                        }
                    }
                    ostr << tr();
                    
                    int count0 = precs->get_Count();
                    int i0 = 0;
                    for (auto it = precs->begin(); it != precs->end(); ++it)
                    {
                        IXbrlAbstractRecord* pitem = *it;
                       // LOGI(cfin._logger, pitem->get_Name());
                        if (strstr(pitem->get_Name(), "StatementTable"))
                        {
                            SP<IXbrlRecordList> pStmtItems;
                            if (ISOK(pitem->get_Children(&pStmtItems)))
                            {
                                for (auto it1 = pStmtItems->begin(); it1 != pStmtItems->end(); ++it1)
                                {
                                    IXbrlAbstractRecord* pline = *it1;
                                    if (strstr(pline->get_Name(), "StatementLineItems"))
                                    {
                                        // pass children to WriteNode.
                                        SP<IXbrlRecordList> pLineItems;
                                        if (ISOK(pline->get_Children(&pLineItems)))
                                        {
                                            int count = pLineItems->get_Count();
                                            int i = 0;
                                            for (auto it2 = pLineItems->begin(); it2 != pLineItems->end(); ++it2, i++)
                                            {
                                                cfin.WriteNode(ostr, 0, *it2, i == (count-1), pstmt->get_LowerYear(), pstmt->get_HighestYear());
                                                i++;
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                        else if (strstr(pitem->get_Name(), "Axis") || strstr(pitem->get_Name(), "Domain"))
                        {
                            continue;
                        }
                        else
                        {
                            //goto ATTEMPTWITHOUTSTATEMENTABLE;
                            cfin.WriteNode(ostr, 0, pitem, i0 == (count0-1), pstmt->get_LowerYear(), pstmt->get_HighestYear());
                            i0++;
                        }
                    }
                    
                    ostr << tr();
                    ostr << th().set("class", "fdm_fin_table_th_account") << "Accounts" << th();
                    for (I16 y = pstmt->get_HighestYear(); y >= pstmt->get_LowerYear(); --y)
                    {
                        ostr << th().set("colspan", "4").set("class", "fdm_fin_table_th_year") << y << th();
                    }
                    ostr << tr();
                    ostr << table();

                    cfin.httpctxt->cache().store_frame(ostrframename.str(), tee.detach(), 3600);
                    
                    return os;
                }
            }
        }
    }
}


