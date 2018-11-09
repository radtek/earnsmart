
#include "sgwebinternal.h"
#include "models.h"
#include "../seglobal/exceptions.h"


namespace sg
{
    namespace web
    {
        namespace models
        {
            Economy::Economy(cppcms::http::context& ctxt) : 
                BaseContent(ctxt, "EconomyView"), _selectedCategoryId(0),
                    //_logger(Logger::getLogger("se.web.models.economy")),
                    News(EconomicNews(ctxt))
            {
            }
            
            void Economy::Init()
            {
                THROWIFFAILED(EconOpen(&_papi), "Econ api failed to open.");
            }

            IIndicatorCategoryList* Economy::getCategories() 
            {
                if (!_pCategories)
                {
                    THROWIFFAILED(_papi->get_IndicatorCategories(&_pCategories),"Failed to get indicator categories from library.");
                }
                return _pCategories._ptr;
            }
            
            UID Economy::getSelectedCategoryId()
            {
                return _selectedCategoryId;
            }
            
            void Economy::setSelectedCategoryId(UID v)
            {
                _selectedCategoryId = v;
            }
            
            IEconomyIndicatorList* Economy::getIndicators()
            {
               // LOGIP("Getting indicators.");
                if (!_pIndicators && getSelectedCategoryId())
                {
                   // LOGI(_logger, "Getting indicators for " << getSelectedCategoryId());
                    SP<IIndicatorCategory> pCat;
                    THROWIFFAILED(_pCategories->get_ItemById(getSelectedCategoryId(), &pCat), "Failed to get a category from list.");
                   // LOGI(_logger, "Received category for " << getSelectedCategoryId());
                    THROWIFFAILED(pCat->get_Indicators(true, &_pIndicators), "Failed to get indicators from category.");
                   // LOGI(_logger, "Received indicators for category " << getSelectedCategoryId());
                }
               // LOGI(_logger, "Returning indicators: " << _pIndicators._ptr);
                return _pIndicators._ptr;
            }

            IEconomicNewsFeeds* Economy::getNewsFeeds() 
            {
               // LOGIP("Getting news rss feeds collection...");
                if (!_pnewFeeds)
                {
                    THROWIFFAILED(_papi->get_EconomicNewsFeeds(&_pnewFeeds), "Failed to retrieve news feeds instance.");
                }
               // LOGIP("Done with getting news rss feeds collection...");
            }

            /////////////////////////////EconomicNews//////////////////////////////
            EconomicNews::EconomicNews(cppcms::http::context& httpctxt) : 
                BaseContent(httpctxt, "EconomicNewsView"),
                //_logger(Logger::getLogger("se.web.models.economicnews")),
                HasNews(false)
            {

            }

            EconomicNews::~EconomicNews()
            {

            }

            IEconomicNewsFeeds* EconomicNews::get_News() 
            {
               // LOGIP("Getting news feeds from api");
                if (!_pnews)
                {
                    SP<IEconomyApi> papi;
                    if (ISOK(se::economy::EconOpen(&papi)))
                    {
                       // LOGIP("Successfully opened economy api.");
                        if (ISOK(papi->get_EconomicNewsFeeds(&_pnews)) && _pnews)
                        {
                           // LOGIP("Successfully received news feeds from api.");
                            HasNews = true;
                        }
                        else
                        {
                           // LOGEP("Failed to news feeds from api.");
                        }
                    }
                    else
                    {
                       // LOGEP("Failed to open economic api.");
                    }
                }
                return _pnews._ptr;
            }

        }
        
    }
}

