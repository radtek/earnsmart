#include "models.h"
#include "commontypes.h"

namespace sg
{
    namespace web
    {
        namespace models
        {

            FinancialStatement::FinancialStatement(cppcms::http::context& ctxt, BaseContent& content) : BaseContent(ctxt, "FinancialStatementsView"), _content(content)
            {
            }
            
            void FinancialStatement::Init()
            {
            }

            void FinancialStatement::Init(unsigned long productId)
            {
                
            }
            
            void FinancialStatement::SetList(IROSPList<IXbrlDocType>* pList)
            {
                _pList = pList;
            }
            
            IROSPList<IXbrlDocType>* FinancialStatement::get_List()
            {
                return _pList._ptr;
            }
            void FinancialStatement::SetPeriod(IROList<I16>*  period)
            {
                _period = period;
            }
            
            IROList<I16>* FinancialStatement::get_Period()
            {
                return _period._ptr;
            }
            
            void FinancialStatement::SetStatementTitles(IROSPList<IXbrlStatement>*  statement)
            {
                _statement = statement;
            }
            
            IROSPList<IXbrlStatement>* FinancialStatement::get_StatementTitles()
            {
                return _statement._ptr;
            }
            
            void FinancialStatement::SetStatement(IXbrlDataNode*  pStatement)
            {
                _pStatementNode = pStatement;
            }
            
            void FinancialStatement::RenderStatement(ostream& out)
            {
                
                if (!_pList)
                    return;
                
//                out << "<hr/>";
                out << "<div>";
                out << "<table id=\"stmtValues\" style=\"table-layout:fixed\">";
                if(_pStatementNode)
                {
                    out << "<caption>" << _pStatementNode->get_Name() << "</caption>";
                    IROSPList<IXbrlDataNode>*  children;
                    _pStatementNode->get_Children(&children);
                    out << "<tr> <th colspan='2'>" 
                            << _pStatementNode->get_Label()
                            << "( StatementId = " << _pStatementNode->get_Id() << " )"
                            << "</th></tr>";
//                    out <<"<tr><th COLSPAN=2><hr></th></tr>";
                    I64 Childsize = children->get_Count();
                    if(Childsize > 0)
                    {   
                        RenderChildren(out, children, 0);
                    }
                }
                else
                    out << "<tr class='stmtValues' > <th colspan='2'>No financial statements available. </th></tr>";
                out << "</table>";
                out << "</div>";                                
            }
            
            void FinancialStatement::RenderChildren(ostream& out, IROSPList<IXbrlDataNode>* child, int tabcount)
            {                
                ostream& str = out;
                I64 Childsize = child->get_Count();
                for(I64 i = 0; i<Childsize ; i++)
                {
                    SP<IXbrlDataNode>dataNode;
                    child->get_Item(i,&dataNode);
                    IROSPList<IXbrlDataNode>*  children;
                    dataNode->get_Children(&children);
                    I64 Childsize = children->get_Count();
                    if(Childsize > 0)
                    {
                        str << "<tr> <th align='left' colspan='2' style='text-indent:" << tabcount << "em'>"
                                << dataNode->get_Label() 
                                << "( XbrlId = " << dataNode->get_Id() << "/ xbrlname = " << dataNode->get_Name() << " )"
                                << " </th></tr>";
                        RenderChildren(out, children, tabcount + 1);
                        //str<<"<tr><th COLSPAN=2><hr></th></tr>";
                        str << "<tr><th align='left' style='word-wrap:break-word;text-indent:" << tabcount << "em'>"<<dataNode->get_Label()
                                << "( XbrlId = " << dataNode->get_Id() << "/ xbrlname = " << dataNode->get_Name() << " )"
                                <<"</th>";
                        str << "<th align='right'>";
                        str.unsetf(std::ios::floatfield);
                        str.precision(2);
                        str.setf(std::ios::fixed, std::ios::floatfield);
                        if(dataNode->get_Monetary() != 0)
                            str<<dataNode->get_Monetary();
                        else
                            str<<dataNode->get_Total();
                        str<<"</th></tr>";
                    }
                    
                    else
                    {
                        str << "<tr><td style='word-wrap:break-word;text-indent:"<< tabcount << "em'>" <<dataNode->get_Label()
                                << "( XbrlId = " << dataNode->get_Id() << "/ xbrlname = " << dataNode->get_Name() << " )"
                                <<"</td>";
                        str << "<td align='right'>";
                        str.unsetf(std::ios::floatfield);
                        str.precision(2);  
                        str.setf(std::ios::fixed, std::ios::floatfield);
                        str<<dataNode->get_Monetary();
                        str<<"</td></tr>";
                    }
                }
                
            }
            ErrorId FinancialStatement::get_HistoricalPrice(ISecurity* pSec, I16 year,I16 qtr,I32 dtype,  ISecurityPrice** ppOut)
            {
                CDate CurrentFrom,CurrentTo,PrevFrom,PrevTo;
                I32 startMonth,endMonth,EndYear,endDate,StartYear;
                if(dtype == 1)
                {
                    switch (qtr)
                    {
                        case 1:
                           startMonth =1;   // filing for first quarter usually done after the quarter from april to june
                           endMonth = 3;
                           EndYear = year;
                           endDate = 31;
                           break;
                        case 2:
                           startMonth =4;   // filing for 2nd quarter usually done after the quarter from july to sept
                           endMonth = 6;
                           EndYear = year;
                           endDate = 30;
                           break;
                        case 3:
                           startMonth =7;   // filing for 3rd quarter usually done after the quarter from oct to dec
                           endMonth = 9;
                           EndYear = year;
                           endDate = 30;
                           break;
                        case 4:
                           startMonth =10;
                           endMonth = 12;
                           EndYear = year;    // filing for fourth quarter done next first quarter only.
                           endDate = 31;
                           break;
                    }
                    StartYear = EndYear;
                }
                else if(dtype == 3)
                {
                    EndYear = year;
                    startMonth =1;
                    endMonth = 12;
                    endDate = 31;
                    StartYear = EndYear;
                }
                CurrentFrom.set_Year(StartYear);
                CurrentFrom.set_Month(startMonth);
                CurrentFrom.set_Day(1);
                CurrentTo.set_Year(EndYear);
                CurrentTo.set_Month(endMonth);
                CurrentTo.set_Day(endDate);                
                ErrorId retVal = pSec->get_AvgHistoricalPriceAndVolume(CurrentFrom,CurrentTo,ppOut);
                
                return retVal;
            }
            
            void FinancialStatement::HistoricalPrice(ISecurityPrice* prev,ISecurityPrice* current,I32 dtype)
            {
//                _hPrice<<"<table class='price' width='400' bgcolor='#d3d3d3'>";
//                _hPrice<<"<tr class='price'><th align='center' colspan='4'>Stock Price</th></tr>";
//                _hPrice<<"<trclass='price'><td colspan='2'>Current Price</td><td colspan='2'>"<<current->get_Price()<<"</td></tr>";
//                _hPrice<<"<trclass='price'><td colspan='2'>Current Volume</td><td colspan='2'>"<<current->get_Volume()<<"</td></tr>";
//                if(dtype == 1)
//                    _hPrice<<"<tr class='price'><th align='center' colspan='2'>Prev Quarter</th><th align='center' colspan='2'> Curr Quarter</th></tr>";
//                else
//                    _hPrice<<"<tr class='price'><th align='center' colspan='2'>Prev Year</th><th align='center' colspan='2'> Current Year</th></tr>";
//                _hPrice<<"<trclass='price'><td>Price</td><td>"<<prev->get_Price()<<"</td><td>Price</td><td>"<<current->get_Price()<<"</td></tr>";
//                _hPrice<<"<trclass='price'><td>Volume</td><td>"<<prev->get_Volume()<<"</td><td>Volume</td><td>"<<current->get_Volume()<<"</td></tr>";
//                _hPrice<<"</table>";
            }
            
            void FinancialStatement::getHistoricalPrice(ostream& out)
            {                
                out<<_hPrice.str();                   
            }
            
        }
    }
}



