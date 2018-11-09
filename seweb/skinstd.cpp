#line 1 "footer.html"
#include "../sewebmodels/CFooter.h" 
#line 2 "footer.html"
namespace skinstd {
	#line 3 "footer.html"
	struct FooterView :public cppcms::base_view
	#line 3 "footer.html"
	{
	#line 3 "footer.html"
		CFooter &content;
	#line 3 "footer.html"
		FooterView(std::ostream &_s,CFooter &_content): cppcms::base_view(_s),content(_content)
	#line 3 "footer.html"
		{
	#line 3 "footer.html"
		}
		#line 4 "footer.html"
		virtual void render() {
			#line 8 "footer.html"
			out()<<"\n"
				"<div id=\"dvFooter\">\n"
				"Footer goes here\n"
				"</div>\n"
				"";
		#line 8 "footer.html"
		} // end of template render
	#line 9 "footer.html"
	}; // end of class FooterView
#line 10 "footer.html"
} // end of namespace skinstd
#line 1 "toolbar.html"
#include "../sewebmodels/CToolbar.h" 
#line 2 "toolbar.html"
namespace skinstd {
	#line 3 "toolbar.html"
	struct ToolbarView :public cppcms::base_view
	#line 3 "toolbar.html"
	{
	#line 3 "toolbar.html"
		CToolbar &content;
	#line 3 "toolbar.html"
		ToolbarView(std::ostream &_s,CToolbar &_content): cppcms::base_view(_s),content(_content)
	#line 3 "toolbar.html"
		{
	#line 3 "toolbar.html"
		}
		#line 4 "toolbar.html"
		virtual void render() {
			#line 14 "toolbar.html"
			out()<<"\n"
				"<div id=\"dvToolbar\">\n"
				"    <nav>\n"
				"        <a href=\"#\">Menu 1</a>\n"
				"        <a href=\"#\">Menu 2</a>\n"
				"        <a href=\"#\">Menu 3</a>\n"
				"        <a href=\"#\">Menu 4</a>\n"
				"        <a href=\"#\">Menu 5</a>\n"
				"    </nav>\n"
				"</div>\n"
				"";
		#line 14 "toolbar.html"
		} // end of template render
	#line 15 "toolbar.html"
	}; // end of class ToolbarView
#line 16 "toolbar.html"
} // end of namespace skinstd
#line 1 "header.html"
#include "../sewebmodels/CHeader.h" 
#line 2 "header.html"
namespace skinstd {
	#line 3 "header.html"
	struct HeaderView :public cppcms::base_view
	#line 3 "header.html"
	{
	#line 3 "header.html"
		CHeader &content;
	#line 3 "header.html"
		HeaderView(std::ostream &_s,CHeader &_content): cppcms::base_view(_s),content(_content)
	#line 3 "header.html"
		{
	#line 3 "header.html"
		}
		#line 4 "header.html"
		virtual void render() {
			#line 18 "header.html"
			out()<<"\n"
				"<div id=\"dvHeader\">\n"
				"    <div id=\"dvLogo\">\n"
				"        <span class=\"logoGreen spLogoE\">e<sup id=\"fontLogoS\" class=\"logoRed\">$</sup>\n"
				"        </span>\n"
				"        <div id=\"dvLogoName\">\n"
				"            <span class=\"logoGreen\">Earn</span><span class=\"logoRed\">Smart</span>\n"
				"        </div>\n"
				"    </div>\n"
				"    <div id=\"dvSiteName\">\n"
				"        <span class=\"logoGreen\">Earn</span><span class=\"logoRed\">Smart</span><br><span id=\"spSiteMotto\">A Better Path to Security Analysis</span>\n"
				"    </div>\n"
				"    \n"
				"</div>\n"
				"";
		#line 18 "header.html"
		} // end of template render
	#line 19 "header.html"
	}; // end of class HeaderView
#line 20 "header.html"
} // end of namespace skinstd
#line 1 "master.html"
#include "../sewebmodels/CMaster.h" 
#line 2 "master.html"
namespace skinstd {
	#line 3 "master.html"
	struct MasterView :public cppcms::base_view
	#line 3 "master.html"
	{
	#line 3 "master.html"
		CMaster &content;
	#line 3 "master.html"
		MasterView(std::ostream &_s,CMaster &_content): cppcms::base_view(_s),content(_content)
	#line 3 "master.html"
		{
	#line 3 "master.html"
		}
		#line 5 "master.html"
		virtual void title() {
			#line 5 "master.html"
			out()<<"Earn Smart";
		#line 5 "master.html"
		} // end of template title
		#line 7 "master.html"
		virtual void mainContent() {
			#line 7 "master.html"
			out()<<"Overridden area";
		#line 7 "master.html"
		} // end of template mainContent
		#line 9 "master.html"
		virtual void render() {
			#line 13 "master.html"
			out()<<"\n"
				"\n"
				"<html class=\"no-js lt-ie9 lt-ie8\" lang=\"en\">\n"
				"    <head>\n"
				"        <title>";
			#line 13 "master.html"
			title();
			#line 19 "master.html"
			out()<<"</title>\n"
				"        <meta charset=\"UTF-8\">\n"
				"        <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
				"        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
				"        <link rel=\"icon\" href=\"images/eulerlogo.png\">\n"
				"        <link rel=\"shortcut icon\" href=\"images/eulerlogo.png\">\n"
				"        ";
			#line 19 "master.html"
			#ifdef __DEBUG__ 
			#line 24 "master.html"
			out()<<"\n"
				"        <script lang=\"javascript\" src=\"prog/modernzr.js\" ></script>\n"
				"        <link rel=\"stylesheet\" href=\"css/jquery-ui.structure.css\" type=\"text/css\">\n"
				"        <link rel=\"stylesheet\" href=\"css/jquery-ui.theme.css\" type=\"text/css\">\n"
				"        <link rel=\"stylesheet\" href=\"css/jquery-ui.css\" type=\"text/css\">\n"
				"        ";
			#line 24 "master.html"
			#else 
			#line 29 "master.html"
			out()<<"\n"
				"        <script lang=\"javascript\" src=\"prog/modernzr-min.js\" ></script>\n"
				"        <link rel=\"stylesheet\" href=\"css/jquery-ui.structure.min.css\" type=\"text/css\">\n"
				"        <link rel=\"stylesheet\" href=\"css/jquery-ui.theme.min.css\" type=\"text/css\">\n"
				"        <link rel=\"stylesheet\" href=\"css/jquery-ui.min.css\" type=\"text/css\">\n"
				"        ";
			#line 29 "master.html"
			#endif 
			#line 34 "master.html"
			out()<<"\n"
				"        <link rel=\"stylesheet\" href=\"css/earnsmart.css\" type=\"text/css\">\n"
				"    </head>\n"
				"    <body>\n"
				"        <header id=\"header\">\n"
				"            ";
			#line 34 "master.html"
			{
			#line 34 "master.html"
			  cppcms::base_content::app_guard _g(content.Header,content);
			#line 34 "master.html"
			  HeaderView _using(out(),content.Header);
			#line 34 "master.html"
			_using.render();
			#line 34 "master.html"
			}
			#line 35 "master.html"
			out()<<"\n"
				"            ";
			#line 35 "master.html"
			{
			#line 35 "master.html"
			  cppcms::base_content::app_guard _g(content.Toolbar,content);
			#line 35 "master.html"
			  ToolbarView _using(out(),content.Toolbar);
			#line 35 "master.html"
			_using.render();
			#line 35 "master.html"
			}
			#line 38 "master.html"
			out()<<"\n"
				"        </header>\n"
				"        <div title=\"Content\" id=\"dvContent\">\n"
				"            ";
			#line 38 "master.html"
			mainContent();
			#line 42 "master.html"
			out()<<"\n"
				"        </div>\n"
				"        <footer id=\"footer\">\n"
				"            <div title=\"Footer\">\n"
				"                ";
			#line 42 "master.html"
			{
			#line 42 "master.html"
			  cppcms::base_content::app_guard _g(content.Footer,content);
			#line 42 "master.html"
			  FooterView _using(out(),content.Footer);
			#line 42 "master.html"
			_using.render();
			#line 42 "master.html"
			}
			#line 45 "master.html"
			out()<<"\n"
				"            </div>\n"
				"        </footer>\n"
				"        ";
			#line 45 "master.html"
			#ifdef __DEBUG__ 
			#line 48 "master.html"
			out()<<"\n"
				"        <script lang=\"javascript\" src=\"prog/jquery-1.11.3.js\" ></script>\n"
				"        <script lang=\"javascript\" src=\"prog/jquery-ui.js\" ></script>\n"
				"        ";
			#line 48 "master.html"
			#else 
			#line 51 "master.html"
			out()<<"\n"
				"        <script lang=\"javascript\" src=\"prog/jquery-1.11.3.min.js\" ></script>\n"
				"        <script lang=\"javascript\" src=\"prog/jquery-ui.min.js\" ></script>\n"
				"        ";
			#line 51 "master.html"
			#endif 
			#line 56 "master.html"
			out()<<"\n"
				"        <script lang=\"javascript\" src=\"prog/earnsmart.js\"></script>\n"
				"    </body>\n"
				"</html>\n"
				"\n"
				"";
		#line 56 "master.html"
		} // end of template render
	#line 57 "master.html"
	}; // end of class MasterView
#line 58 "master.html"
} // end of namespace skinstd
#line 1 "home.html"
#include "../sewebmodels/CHome.h" 
#line 2 "home.html"
namespace skinstd {
	#line 3 "home.html"
	struct HomeView :public MasterView
	#line 3 "home.html"
	{
	#line 3 "home.html"
		CHome &content;
	#line 3 "home.html"
		HomeView(std::ostream &_s,CHome &_content): MasterView(_s,_content),content(_content)
	#line 3 "home.html"
		{
	#line 3 "home.html"
		}
		#line 5 "home.html"
		virtual void title() {
			#line 5 "home.html"
			out()<<"Earn Smart | Home";
		#line 5 "home.html"
		} // end of template title
		#line 8 "home.html"
		virtual void mainContent() {
			#line 12 "home.html"
			out()<<"\n"
				"\n"
				"    Home view\n"
				"\n"
				"";
		#line 12 "home.html"
		} // end of template mainContent
	#line 14 "home.html"
	}; // end of class HomeView
#line 15 "home.html"
} // end of namespace skinstd
#line 1 "error.html"
#include "../sewebmodels/CError.h" 
#line 2 "error.html"
namespace skinstd {
	#line 3 "error.html"
	struct ErrorView :public MasterView
	#line 3 "error.html"
	{
	#line 3 "error.html"
		CError &content;
	#line 3 "error.html"
		ErrorView(std::ostream &_s,CError &_content): MasterView(_s,_content),content(_content)
	#line 3 "error.html"
		{
	#line 3 "error.html"
		}
		#line 5 "error.html"
		virtual void title() {
			#line 5 "error.html"
			out()<<"Earn Smart | Error";
		#line 5 "error.html"
		} // end of template title
		#line 8 "error.html"
		virtual void mainContent() {
			#line 11 "error.html"
			out()<<"\n"
				"\n"
				"<span style=\"color:red\">\n"
				"    ";
			#line 11 "error.html"
			out()<<cppcms::filters::escape(content.get_Error().what());
			#line 14 "error.html"
			out()<<"\n"
				"</span>\n"
				"\n"
				"";
		#line 14 "error.html"
		} // end of template mainContent
	#line 17 "error.html"
	}; // end of class ErrorView
#line 18 "error.html"
} // end of namespace skinstd
#line 19 "error.html"
namespace {
#line 19 "error.html"
 cppcms::views::generator my_generator; 
#line 19 "error.html"
 struct loader { 
#line 19 "error.html"
  loader() { 
#line 19 "error.html"
   my_generator.name("skinstd");
#line 19 "error.html"
   my_generator.add_view<skinstd::FooterView,CFooter>("FooterView",true);
#line 19 "error.html"
   my_generator.add_view<skinstd::ToolbarView,CToolbar>("ToolbarView",true);
#line 19 "error.html"
   my_generator.add_view<skinstd::HeaderView,CHeader>("HeaderView",true);
#line 19 "error.html"
   my_generator.add_view<skinstd::MasterView,CMaster>("MasterView",true);
#line 19 "error.html"
   my_generator.add_view<skinstd::HomeView,CHome>("HomeView",true);
#line 19 "error.html"
   my_generator.add_view<skinstd::ErrorView,CError>("ErrorView",true);
#line 19 "error.html"
    cppcms::views::pool::instance().add(my_generator);
#line 19 "error.html"
 }
#line 19 "error.html"
 ~loader() {  cppcms::views::pool::instance().remove(my_generator); }
#line 19 "error.html"
} a_loader;
#line 19 "error.html"
} // anon 
