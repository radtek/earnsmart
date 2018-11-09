#include "models.h"


namespace sg
{
    namespace web
    {
        namespace models
        {
            Master::Master() : _ppageModel(nullptr)
            {
            }
            
            Master::Master(cppcms::http::context& ctxt)
                : BaseContent(ctxt), _ppageModel(nullptr)
            {
            }
            
            Master::Master(cppcms::http::context& ctxt, BaseContent& pageModel)
                : BaseContent(ctxt), 
                  footer(ctxt), 
                  header(ctxt),
                  recentFinds(ctxt),
                  _ppageModel(&pageModel)
            {
            }
            
            void Master::Init()
            {
                footer.Init();
                header.Init();
                recentFinds.Init();
                if (_ppageModel)
                    _ppageModel->Init();
            }
            
            void Master::Init(BaseContent& pageModel) 
            {
                auto& ctxt = *this->httpctxt;
                footer.Init(ctxt);
                header.Init(ctxt);
                recentFinds.Init(ctxt);
                this->_ppageModel = &pageModel;
                if (_ppageModel)
                    _ppageModel->Init(ctxt);
            }

            void Master::Init(cppcms::http::context& httpctxt, BaseContent& pageModel) 
            {
                BaseContent::Init(httpctxt);
                footer.Init(httpctxt);
                header.Init(httpctxt);
                recentFinds.Init(httpctxt);
                this->_ppageModel = &pageModel;
                if (_ppageModel)
                    _ppageModel->Init(httpctxt);
            }
            
            void Master::Init(cppcms::http::context& httpctxt, BaseContent& pageModel, const string& viewName)
            {
                BaseContent::Init(httpctxt, viewName);
                footer.Init();
                header.Init();
                recentFinds.Init();
                this->_ppageModel = &pageModel;
                if (_ppageModel)
                    _ppageModel->Init(httpctxt);
            }

            BaseContent& Master::pageModel() 
            {
                return *_ppageModel;
            }

        }
    }
}
