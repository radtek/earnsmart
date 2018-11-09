#include "sgwebinternal.h"
#include "models.h"
#include "../seglobal/exceptions.h"

namespace sg
{
    namespace web
    {
        namespace models
        {
            CompanyProspects::CompanyProspects(cppcms::http::context& ctxt) : 
                Fundamentals(ctxt, "CompanyProspectsView"), 
                    CompanyProspectsChartView("CompanyProspectsChartView")
            {
                //_logger = Logger::getLogger("sg.web.companyprospects");
            }
            
            CompanyProspects::~CompanyProspects()
            {
                for (auto p : charts)
                    delete p;
                charts.clear();
            }
            
            void CompanyProspects::Init(unsigned long prodId) 
            {
                Fundamentals::Init(prodId);
                
                try
                {
                    // logic for building chart controls goes here....
                    CProspectChartFuture* chartRevenue = new CProspectChartFuture(*this->httpctxt, *this);
                    chartRevenue->ProductId = prodId;
                    chartRevenue->title = "Revenue Expectation";
                    chartRevenue->RefId = 17;

                    SP<IXbrl> pxbrl;
                    THROWIFFAILED(XbrlOpen(&pxbrl), "Failed to open financials api.");

                    SP<se::training::ITrainingTable> ptbl;
                    THROWIFFAILED(pxbrl->GetPerformanceTrainingData(
                            prodId, 
                            21, 
                            DATAPERIOD(),
                            DATAPERIOD(),
                            &ptbl), "Failed to get prospects view");
                    ptbl->get_GMean(false, &chartRevenue->RevAvg);
                    ptbl->get_StdDev(false, &chartRevenue->Risk);

                    SP<se::training::ITrainingTable> pTblProspects;
                    THROWIFFAILED(pxbrl->GetProspectsView(prodId, 25, PERIOD_Quarterly, &pTblProspects), "Failed to get prospects view");


                    SP<ITrainingRecord> pfut;
                    THROWIFFAILED(pTblProspects->get_FutureRecord(&pfut), "Failed to get predicted revenue.");
                    chartRevenue->PredRevGrowth = pfut->get_Change();

                    AddChart(chartRevenue);
                    
                    CProspectChartSector* chartSector = new CProspectChartSector(*this->httpctxt, *this);
                    chartSector->ProductId = prodId;
                    chartSector->title = "Revenue Performance Comparison with Sector/Industry";
                    chartSector->RefId = 18;
                    AddChart(chartSector);
                    
                }
                catch (const std::exception &ex)
                {
                    BERR << ex.what();
                }
                catch (...)
                {
                    set_Error("Failed to render estimates page. Sorry for the inconvenience. We will fix this issue as soon as possible.", true);
                   // LOGE(_logger, "Failed to retrieve estimates data.");
                }
            }

            void CompanyProspects::AddChart(CProspectChart* c) 
            {
                c->chartid = charts.size() + 1;
                charts.push_back(c);
            }

            /////////////////////////////BaseChartModel - CProspectChart //////////////
            CProspectChart::CProspectChart(
                    cppcms::http::context& httpctxt, 
                    const string& viewName,
                    CompanyProspects& prospects
                ) : BaseContent(httpctxt, viewName),
                    Prospects(prospects)
            {
                
            }
            
            
            
            /////////////////////////////CProspectChart//////////////////////
            CProspectChartSector::CProspectChartSector(cppcms::http::context& httpctxt, CompanyProspects& prospects) : 
                    CProspectChart(httpctxt, "CompanyProspectsSectorView", prospects)
            {
                
            }

            void CProspectChartSector::Init() 
            {
                
            }
            
            /////////////////////////////CProspectChart//////////////////////
            CProspectChartFuture::CProspectChartFuture(cppcms::http::context& httpctxt, CompanyProspects& prospects) : 
                    CProspectChart(httpctxt, "CompanyProspectsFutureView", prospects)
            {
            }

            void CProspectChartFuture::Init() 
            {
                
            }
            
        }
    }
}

//fdm_prospects_charts.xhtml 