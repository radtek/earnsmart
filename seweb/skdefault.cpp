#line 1 "footer.html"
#include "../sewebmodels/CFooter.h" 
#line 2 "footer.html"
namespace skdefault {
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
				"<div id=\"dvDiscl\">\n"
				"    &COPY; 2015 Smartgaze LLC, New Jersey, USA. | Sitemap\n"
				"</div>\n"
				"";
		#line 8 "footer.html"
		} // end of template render
	#line 9 "footer.html"
	}; // end of class FooterView
#line 10 "footer.html"
} // end of namespace skdefault
#line 1 "toolbar.html"
#include "../sewebmodels/CToolbar.h" 
#line 2 "toolbar.html"
namespace skdefault {
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
			#line 7 "toolbar.html"
			out()<<"\n"
				"<div id=\"dvToolbar\">\n"
				"    <nav>\n"
				"        ";
			#line 7 "toolbar.html"
			for (const Json::Value& menu : content.RootMenu["Menus"]) { 
			#line 8 "toolbar.html"
			out()<<"\n"
				"        ";
			#line 8 "toolbar.html"
			if (menu["id"].asInt() == 1) /* home page */ { 
			#line 9 "toolbar.html"
			out()<<"\n"
				"            <a id='topmenu";
			#line 9 "toolbar.html"
			out() << menu["id"].asInt(); 
			#line 9 "toolbar.html"
			out()<<"' href='";
			#line 9 "toolbar.html"
			out() << menu["Url"].asString(); 
			#line 9 "toolbar.html"
			out()<<"'>";
			#line 9 "toolbar.html"
			out() << menu["Name"].asString(); 
			#line 10 "toolbar.html"
			out()<<"</a> &nbsp; &nbsp;\n"
				"        ";
			#line 10 "toolbar.html"
			} else { 
			#line 11 "toolbar.html"
			out()<<"\n"
				"            <a id='topmenu";
			#line 11 "toolbar.html"
			out() << menu["id"].asInt(); 
			#line 11 "toolbar.html"
			out()<<"' submenu='topsubmenu";
			#line 11 "toolbar.html"
			out() << menu["id"].asInt(); 
			#line 11 "toolbar.html"
			out()<<"' href='";
			#line 11 "toolbar.html"
			out() << menu["Url"].asString(); 
			#line 11 "toolbar.html"
			out()<<"'>";
			#line 11 "toolbar.html"
			out() << menu["Name"].asString(); 
			#line 12 "toolbar.html"
			out()<<"</a> &nbsp; &nbsp;\n"
				"        ";
			#line 12 "toolbar.html"
			} 
			#line 13 "toolbar.html"
			out()<<"\n"
				"        ";
			#line 13 "toolbar.html"
			} 
			#line 15 "toolbar.html"
			out()<<"\n"
				"        \n"
				"        ";
			#line 15 "toolbar.html"
			for (const Json::Value& menu : content.RootMenu["Menus"]) { 
			#line 16 "toolbar.html"
			out()<<"\n"
				"        ";
			#line 16 "toolbar.html"
			if (menu["id"].asInt() == 1) continue; 
			#line 17 "toolbar.html"
			out()<<"\n"
				"        <ul id='topsubmenu";
			#line 17 "toolbar.html"
			out() << menu["id"].asInt(); 
			#line 18 "toolbar.html"
			out()<<"' style=\"min-width:150px;visibility:hidden;position:absolute\">\n"
				"            ";
			#line 18 "toolbar.html"
			for (const Json::Value& child : menu["Children"]) { 
			#line 19 "toolbar.html"
			out()<<"\n"
				"            <li href='";
			#line 19 "toolbar.html"
			out() << child["Url"].asString(); 
			#line 19 "toolbar.html"
			out()<<"'>";
			#line 19 "toolbar.html"
			out() << child["Name"].asString(); 
			#line 20 "toolbar.html"
			out()<<"</li>\n"
				"            ";
			#line 20 "toolbar.html"
			} 
			#line 22 "toolbar.html"
			out()<<"\n"
				"        </ul>        \n"
				"        ";
			#line 22 "toolbar.html"
			} 
			#line 25 "toolbar.html"
			out()<<"\n"
				"    </nav>\n"
				"</div>\n"
				"";
		#line 25 "toolbar.html"
		} // end of template render
	#line 26 "toolbar.html"
	}; // end of class ToolbarView
#line 27 "toolbar.html"
} // end of namespace skdefault
#line 1 "appsearch.html"
#include "../sewebmodels/CAppSearch.h" 
#line 2 "appsearch.html"
namespace skdefault {
	#line 3 "appsearch.html"
	struct AppSearchView :public cppcms::base_view
	#line 3 "appsearch.html"
	{
	#line 3 "appsearch.html"
		CAppSearch &content;
	#line 3 "appsearch.html"
		AppSearchView(std::ostream &_s,CAppSearch &_content): cppcms::base_view(_s),content(_content)
	#line 3 "appsearch.html"
		{
	#line 3 "appsearch.html"
		}
		#line 4 "appsearch.html"
		virtual void render() {
			#line 9 "appsearch.html"
			out()<<"\n"
				"<div id=\"dvAppSearch\">\n"
				"    <label for=\"txtAppSearch\" title=\"Enter a partial or complete text for a ticker, product name or any other information. A popup will then guide you to selected nearest match.\">Search: </label>\n"
				"    <input type=\"text\" id=\"txtAppSearch\">\n"
				"</div>\n"
				"";
		#line 9 "appsearch.html"
		} // end of template render
	#line 10 "appsearch.html"
	}; // end of class AppSearchView
#line 11 "appsearch.html"
} // end of namespace skdefault
#line 1 "loginorregister.html"
#include "../sewebmodels/CLoginOrRegister.h" 
#line 2 "loginorregister.html"
namespace skdefault {
	#line 3 "loginorregister.html"
	struct LoginOrRegisterView :public cppcms::base_view
	#line 3 "loginorregister.html"
	{
	#line 3 "loginorregister.html"
		CLoginOrRegister &content;
	#line 3 "loginorregister.html"
		LoginOrRegisterView(std::ostream &_s,CLoginOrRegister &_content): cppcms::base_view(_s),content(_content)
	#line 3 "loginorregister.html"
		{
	#line 3 "loginorregister.html"
		}
		#line 4 "loginorregister.html"
		virtual void render() {
			#line 6 "loginorregister.html"
			out()<<"\n"
				"<div id=\"dvLoginRegister\">\n"
				"    ";
			#line 6 "loginorregister.html"
			if (!content.UserSession or content.get_IsAnon()){ 
			#line 8 "loginorregister.html"
			out()<<"\n"
				"    <a href=\"/register\">Register</a><a href=\"/signon\">Log on</a>\n"
				"    ";
			#line 8 "loginorregister.html"
			} else { 
			#line 9 "loginorregister.html"
			out()<<"\n"
				"    <a href=\"/account\">";
			#line 9 "loginorregister.html"
			out()<<cppcms::filters::escape(content.UserSession->get_User()->get_FullName());
			#line 10 "loginorregister.html"
			out()<<" </a><a href=\"/logout\">Exit</a>\n"
				"    ";
			#line 10 "loginorregister.html"
			} 
			#line 12 "loginorregister.html"
			out()<<"\n"
				"</div>\n"
				"";
		#line 12 "loginorregister.html"
		} // end of template render
	#line 13 "loginorregister.html"
	}; // end of class LoginOrRegisterView
#line 14 "loginorregister.html"
} // end of namespace skdefault
#line 1 "header.html"
#include "../sewebmodels/CHeader.h" 
#line 2 "header.html"
namespace skdefault {
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
			#line 21 "header.html"
			out()<<"\n"
				"<div id=\"dvHeader\">\n"
				"    <div id=\"dvLogo\">\n"
				"        <span class=\"logoGreen spLogoE\">e<sup id=\"fontLogoS\" class=\"logoRed\">$</sup>\n"
				"        </span>\n"
				"        <div id=\"dvLogoName\">\n"
				"            <span class=\"logoGreen\">Earn</span><span class=\"splogonamedash\">-</span><span class=\"logoRed\">Smart</span>\n"
				"        </div>\n"
				"    </div>\n"
				"    <div id=\"dvSiteName\">\n"
				"        <a href=\"/\">\n"
				"            <span class=\"logoGreen\">Earn</span><span class=\"splogonamedash\">-</span><span class=\"logoRed\">Smart</span>\n"
				"        </a>\n"
				"        <br>\n"
				"        <span id=\"spSiteMotto\">A Better Path to Security Analysis</span>\n"
				"    </div>\n"
				"\n"
				"    ";
			#line 21 "header.html"
			{
			#line 21 "header.html"
			  cppcms::base_content::app_guard _g(content.AppSearch,content);
			#line 21 "header.html"
			  AppSearchView _using(out(),content.AppSearch);
			#line 21 "header.html"
			_using.render();
			#line 21 "header.html"
			}
			#line 23 "header.html"
			out()<<"\n"
				"\n"
				"    ";
			#line 23 "header.html"
			{
			#line 23 "header.html"
			  cppcms::base_content::app_guard _g(content.LoginOrRegister,content);
			#line 23 "header.html"
			  LoginOrRegisterView _using(out(),content.LoginOrRegister);
			#line 23 "header.html"
			_using.render();
			#line 23 "header.html"
			}
			#line 26 "header.html"
			out()<<"\n"
				"    \n"
				"</div>\n"
				"";
		#line 26 "header.html"
		} // end of template render
	#line 27 "header.html"
	}; // end of class HeaderView
#line 28 "header.html"
} // end of namespace skdefault
#line 1 "master.html"
#include "../sewebmodels/CMaster.h" 
#line 2 "master.html"
namespace skdefault {
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
				"        <link rel=\"icon\" href=\"/images/eulerlogo.png\">\n"
				"        <link rel=\"shortcut icon\" href=\"/images/eulerlogo.png\">\n"
				"        ";
			#line 19 "master.html"
			#ifdef __DEBUG__ 
			#line 24 "master.html"
			out()<<"\n"
				"        <script lang=\"javascript\" src=\"/prog/modernzr.js\" ></script>\n"
				"        <link rel=\"stylesheet\" href=\"/css/jquery-ui.structure.css\" type=\"text/css\">\n"
				"        <link rel=\"stylesheet\" href=\"/css/jquery-ui.theme.css\" type=\"text/css\">\n"
				"        <link rel=\"stylesheet\" href=\"/css/jquery-ui.css\" type=\"text/css\">\n"
				"        ";
			#line 24 "master.html"
			#else 
			#line 29 "master.html"
			out()<<"\n"
				"        <script lang=\"javascript\" src=\"/prog/modernzr-min.js\" ></script>\n"
				"        <link rel=\"stylesheet\" href=\"/css/jquery-ui.structure.min.css\" type=\"text/css\">\n"
				"        <link rel=\"stylesheet\" href=\"/css/jquery-ui.theme.min.css\" type=\"text/css\">\n"
				"        <link rel=\"stylesheet\" href=\"/css/jquery-ui.min.css\" type=\"text/css\">\n"
				"        ";
			#line 29 "master.html"
			#endif 
			#line 45 "master.html"
			out()<<"\n"
				"        <link rel=\"stylesheet\" href=\"/css/earnsmart.css\" type=\"text/css\">\n"
				"        <!--[if !IE 7]\n"
				"            <style type=\"text/css\">\n"
				"                #dvMaster { \n"
				"                    display:table; \n"
				"                    height:100%;\n"
				"                }\n"
				"            </style>\n"
				"        <![endif]-->\n"
				"    </head>\n"
				"    <body>\n"
				"        <div id=\"dvMaster\">\n"
				"            <form id=\"formMain\" method=\"post\" name=\"formMain\">\n"
				"                <header id=\"header\">\n"
				"                    <div id=\"dvHeaderTb\">\n"
				"                        ";
			#line 45 "master.html"
			{
			#line 45 "master.html"
			  cppcms::base_content::app_guard _g(content.Header,content);
			#line 45 "master.html"
			  HeaderView _using(out(),content.Header);
			#line 45 "master.html"
			_using.render();
			#line 45 "master.html"
			}
			#line 46 "master.html"
			out()<<"\n"
				"                        ";
			#line 46 "master.html"
			{
			#line 46 "master.html"
			  cppcms::base_content::app_guard _g(content.Toolbar,content);
			#line 46 "master.html"
			  ToolbarView _using(out(),content.Toolbar);
			#line 46 "master.html"
			_using.render();
			#line 46 "master.html"
			}
			#line 50 "master.html"
			out()<<"\n"
				"                    </div>\n"
				"                </header>\n"
				"                <div title=\"Content\" id=\"dvContent\">\n"
				"                    ";
			#line 50 "master.html"
			mainContent();
			#line 56 "master.html"
			out()<<"\n"
				"                </div>\n"
				"            </form>\n"
				"        </div>\n"
				"        <footer id=\"footer\">\n"
				"            <div id=\"dvFooter\">\n"
				"                ";
			#line 56 "master.html"
			{
			#line 56 "master.html"
			  cppcms::base_content::app_guard _g(content.Footer,content);
			#line 56 "master.html"
			  FooterView _using(out(),content.Footer);
			#line 56 "master.html"
			_using.render();
			#line 56 "master.html"
			}
			#line 59 "master.html"
			out()<<"\n"
				"            </div>\n"
				"        </footer>\n"
				"        ";
			#line 59 "master.html"
			#ifdef __DEBUG__ 
			#line 64 "master.html"
			out()<<"\n"
				"        <script lang=\"javascript\" src=\"/prog/jquery-1.11.3.js\" ></script>\n"
				"        <script lang=\"javascript\" src=\"/prog/jquery-ui.js\" ></script>\n"
				"        <script lang=\"javascript\" src=\"/ext/js-webshim/dev/polyfiller.js\"></script>\n"
				"        <script lang=\"javascript\" src=\"/prog/oboe-browser.js\"></script>\n"
				"        ";
			#line 64 "master.html"
			#else 
			#line 69 "master.html"
			out()<<"\n"
				"        <script lang=\"javascript\" src=\"/prog/jquery-1.11.3.min.js\" ></script>\n"
				"        <script lang=\"javascript\" src=\"/prog/jquery-ui.min.js\" ></script>\n"
				"        <script lang=\"javascript\" src=\"/ext/js-webshim/minified/polyfiller.js\"></script>\n"
				"        <script lang=\"javascript\" src=\"/prog/oboe-browser.min.js\"></script>\n"
				"        ";
			#line 69 "master.html"
			#endif 
			#line 77 "master.html"
			out()<<"\n"
				"        <script lang=\"javascript\">\n"
				"            webshims.polyfill(\"canvas es5 forms forms-ext mediaelement\");\n"
				"        </script>\n"
				"        <script lang=\"javascript\" src=\"/prog/earnsmart.js\"></script>\n"
				"    </body>\n"
				"</html>\n"
				"\n"
				"";
		#line 77 "master.html"
		} // end of template render
	#line 78 "master.html"
	}; // end of class MasterView
#line 79 "master.html"
} // end of namespace skdefault
#line 1 "home.html"
#include "../sewebmodels/CHome.h" 
#line 2 "home.html"
namespace skdefault {
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
			#line 87 "home.html"
			out()<<"\n"
				"\n"
				"ormer Greek Finance Minister Yanis Varoufakis  has, as Macbeth put it, “strutted and fretted his hour upon the stage.” But he will still be heard some more. \n"
				"\n"
				"While Prime Minister Alexis Tsipras pleaded for support Wednesday for a European Union “rescue” plan in which he said he didn’t believe, Varoufakis was busy ripping it apart. In a widely circulated blog, Varoufakis boiled down his belief to this: Greece had been reduced to the status of a slave state.\n"
				"\n"
				"While his words were clearly driven by anger and spite, he’s not entirely out of line. The agreement is, as Tsipras said, a kind of blackmail. The economist Simon Tilford described it as an order to “acquiesce to all our demands or we will evict you from the currency union.” Pensions will be cut further, labor markets liberalized, working lives extended, collective bargaining “modernized,” and hiring and firing made easier. For a government that takes its inspiration from Karl Marx, this is a neo-liberal dousing.\n"
				"\n"
				"There are few enthusiasts for the deal: the most important of the skeptics is the International Monetary Fund, which called for the euro zone creditors to allow a partial write-off of its 300-billion-euro-plus debt, or at least permit a repayment pause for 30 years. In an ironic twist, the IMF, the creditor the Tsipras government most despised, is now its (partial) friend.\n"
				"\n"
				"Skeptics have focused not just on the impossibility of debt repayment, but also on the deepening poverty that will result from the agreement. Francois Cabeau, an economist in Barclays Bank, told the French daily Figaro that the economy would continue to shrink by between 6 and 8 percent a year. Because the Greek economy has so few sectors where significant value is added other than shipping and tourism, it depends heavily on consumption — which is being further cut, thus prompting a vicious cycle and a further immiseration of the poor, elderly and sick.\n"
				"\n"
				"These conditions validate Varoufakis’ analysis. Greece is a country so firmly under the unremitting pressure of its creditors and so tied to foreign demands, that it may soon resemble an East European communist state in the high tide of Soviet power. Like two of these states — Hungary in 1956, Czechoslovakia in 1968 — Syriza made a failed attempt at a revolt, and was crushed.\n"
				"\n"
				"Could such a state make a convincing effort to find the internal strengths and discipline to mount the kind of recovery it needs? Discipline has not been the hallmark of Greek economic management for the past 14 years since it joined the euro. Rather it has behaved like a spoiled child, taking all that low interest loans could deliver, padding the state bureaucracies with political clients and allowing the rich, the holiday-destination islands and the Orthodox Church to avoid paying taxes. Creditors happily advanced the loans, winked at the corruption, ignored warnings of unsustainable debt, and now, after decades of indulgence, they turn on the hapless little state with a whip.\n"
				"\n"
				"So should it leave the euro zone? The objections to a Grexit are twofold: first, that its currency — presumably a newly issued drachma — would be walloped by an unfavorable exchange rate as a result. Foreign goods and foreign travel would be priced out of many families’ reach. At the same time, as euro zone leaders have warned continually, a Grexit would also shake the euro to its foundations — and though the remaining 18 members could be protected, a precedent would be set that this is a contingent currency, with membership dependent on national conditions.\n"
				"\n"
				"That it would be bad is certain: but how much worse than staying in and swallowing bitter medicine? As for the future of the euro, it would no longer be the Greeks’ problem. What, they may say, has the euro done for us?\n"
				"\n"
				"A Grexit would allow Greece to focus its national pride on reviving the economy — rather than railing at Germany — tempting the most talented members of the wide Greek diaspora to come back to be part of a patriotic project, digging deep into the wells of national talent to fashion a new image for a country that has great beauty, the richest of histories and an economy waiting to be energized.\n"
				"\n"
				"To accomplish this would require a few self-mandates: for starters, an all-out assault on corruption and a determination to make the oligarchs of shipping and the hierarchs of the Orthodox Church pay their fair share of taxes. Socialism would not be part of it: Syriza would have to be disabused of that notion. Tsipras has shown he’s flexible enough to bend every which way, and if he is as attached to power (and skillful in keeping it) as he seems, then he may yet prove a useful politician, a late convert to social democracy.\n"
				"\n"
				"The Greeks say that they are a “proud people.” But that’s been somewhat hollow through the years of free riding. Now, nothing could make that boast more concrete than taking control of their own destiny, and showing Europe that the country is not the pain in the backside  it has proven to be this past year, but a nation, once again, of creators.\n"
				"\n"
				"The euro has crushed them, and other small and relatively poor economies have taken note. But Greece can step out of all of that. The cause that the British romantic poet Lord Byron made his own in the early 19th century should be revived. Liberation for the Greeks!\n"
				"\n"
				"ormer Greek Finance Minister Yanis Varoufakis  has, as Macbeth put it, “strutted and fretted his hour upon the stage.” But he will still be heard some more. \n"
				"\n"
				"While Prime Minister Alexis Tsipras pleaded for support Wednesday for a European Union “rescue” plan in which he said he didn’t believe, Varoufakis was busy ripping it apart. In a widely circulated blog, Varoufakis boiled down his belief to this: Greece had been reduced to the status of a slave state.\n"
				"\n"
				"While his words were clearly driven by anger and spite, he’s not entirely out of line. The agreement is, as Tsipras said, a kind of blackmail. The economist Simon Tilford described it as an order to “acquiesce to all our demands or we will evict you from the currency union.” Pensions will be cut further, labor markets liberalized, working lives extended, collective bargaining “modernized,” and hiring and firing made easier. For a government that takes its inspiration from Karl Marx, this is a neo-liberal dousing.\n"
				"\n"
				"There are few enthusiasts for the deal: the most important of the skeptics is the International Monetary Fund, which called for the euro zone creditors to allow a partial write-off of its 300-billion-euro-plus debt, or at least permit a repayment pause for 30 years. In an ironic twist, the IMF, the creditor the Tsipras government most despised, is now its (partial) friend.\n"
				"\n"
				"Skeptics have focused not just on the impossibility of debt repayment, but also on the deepening poverty that will result from the agreement. Francois Cabeau, an economist in Barclays Bank, told the French daily Figaro that the economy would continue to shrink by between 6 and 8 percent a year. Because the Greek economy has so few sectors where significant value is added other than shipping and tourism, it depends heavily on consumption — which is being further cut, thus prompting a vicious cycle and a further immiseration of the poor, elderly and sick.\n"
				"\n"
				"These conditions validate Varoufakis’ analysis. Greece is a country so firmly under the unremitting pressure of its creditors and so tied to foreign demands, that it may soon resemble an East European communist state in the high tide of Soviet power. Like two of these states — Hungary in 1956, Czechoslovakia in 1968 — Syriza made a failed attempt at a revolt, and was crushed.\n"
				"\n"
				"Could such a state make a convincing effort to find the internal strengths and discipline to mount the kind of recovery it needs? Discipline has not been the hallmark of Greek economic management for the past 14 years since it joined the euro. Rather it has behaved like a spoiled child, taking all that low interest loans could deliver, padding the state bureaucracies with political clients and allowing the rich, the holiday-destination islands and the Orthodox Church to avoid paying taxes. Creditors happily advanced the loans, winked at the corruption, ignored warnings of unsustainable debt, and now, after decades of indulgence, they turn on the hapless little state with a whip.\n"
				"\n"
				"So should it leave the euro zone? The objections to a Grexit are twofold: first, that its currency — presumably a newly issued drachma — would be walloped by an unfavorable exchange rate as a result. Foreign goods and foreign travel would be priced out of many families’ reach. At the same time, as euro zone leaders have warned continually, a Grexit would also shake the euro to its foundations — and though the remaining 18 members could be protected, a precedent would be set that this is a contingent currency, with membership dependent on national conditions.\n"
				"\n"
				"That it would be bad is certain: but how much worse than staying in and swallowing bitter medicine? As for the future of the euro, it would no longer be the Greeks’ problem. What, they may say, has the euro done for us?\n"
				"\n"
				"A Grexit would allow Greece to focus its national pride on reviving the economy — rather than railing at Germany — tempting the most talented members of the wide Greek diaspora to come back to be part of a patriotic project, digging deep into the wells of national talent to fashion a new image for a country that has great beauty, the richest of histories and an economy waiting to be energized.\n"
				"\n"
				"To accomplish this would require a few self-mandates: for starters, an all-out assault on corruption and a determination to make the oligarchs of shipping and the hierarchs of the Orthodox Church pay their fair share of taxes. Socialism would not be part of it: Syriza would have to be disabused of that notion. Tsipras has shown he’s flexible enough to bend every which way, and if he is as attached to power (and skillful in keeping it) as he seems, then he may yet prove a useful politician, a late convert to social democracy.\n"
				"\n"
				"The Greeks say that they are a “proud people.” But that’s been somewhat hollow through the years of free riding. Now, nothing could make that boast more concrete than taking control of their own destiny, and showing Europe that the country is not the pain in the backside  it has proven to be this past year, but a nation, once again, of creators.\n"
				"\n"
				"The euro has crushed them, and other small and relatively poor economies have taken note. But Greece can step out of all of that. The cause that the British romantic poet Lord Byron made his own in the early 19th century should be revived. Liberation for the Greeks!\n"
				"\n"
				"ormer Greek Finance Minister Yanis Varoufakis  has, as Macbeth put it, “strutted and fretted his hour upon the stage.” But he will still be heard some more. \n"
				"\n"
				"While Prime Minister Alexis Tsipras pleaded for support Wednesday for a European Union “rescue” plan in which he said he didn’t believe, Varoufakis was busy ripping it apart. In a widely circulated blog, Varoufakis boiled down his belief to this: Greece had been reduced to the status of a slave state.\n"
				"\n"
				"While his words were clearly driven by anger and spite, he’s not entirely out of line. The agreement is, as Tsipras said, a kind of blackmail. The economist Simon Tilford described it as an order to “acquiesce to all our demands or we will evict you from the currency union.” Pensions will be cut further, labor markets liberalized, working lives extended, collective bargaining “modernized,” and hiring and firing made easier. For a government that takes its inspiration from Karl Marx, this is a neo-liberal dousing.\n"
				"\n"
				"There are few enthusiasts for the deal: the most important of the skeptics is the International Monetary Fund, which called for the euro zone creditors to allow a partial write-off of its 300-billion-euro-plus debt, or at least permit a repayment pause for 30 years. In an ironic twist, the IMF, the creditor the Tsipras government most despised, is now its (partial) friend.\n"
				"\n"
				"Skeptics have focused not just on the impossibility of debt repayment, but also on the deepening poverty that will result from the agreement. Francois Cabeau, an economist in Barclays Bank, told the French daily Figaro that the economy would continue to shrink by between 6 and 8 percent a year. Because the Greek economy has so few sectors where significant value is added other than shipping and tourism, it depends heavily on consumption — which is being further cut, thus prompting a vicious cycle and a further immiseration of the poor, elderly and sick.\n"
				"\n"
				"These conditions validate Varoufakis’ analysis. Greece is a country so firmly under the unremitting pressure of its creditors and so tied to foreign demands, that it may soon resemble an East European communist state in the high tide of Soviet power. Like two of these states — Hungary in 1956, Czechoslovakia in 1968 — Syriza made a failed attempt at a revolt, and was crushed.\n"
				"\n"
				"Could such a state make a convincing effort to find the internal strengths and discipline to mount the kind of recovery it needs? Discipline has not been the hallmark of Greek economic management for the past 14 years since it joined the euro. Rather it has behaved like a spoiled child, taking all that low interest loans could deliver, padding the state bureaucracies with political clients and allowing the rich, the holiday-destination islands and the Orthodox Church to avoid paying taxes. Creditors happily advanced the loans, winked at the corruption, ignored warnings of unsustainable debt, and now, after decades of indulgence, they turn on the hapless little state with a whip.\n"
				"\n"
				"So should it leave the euro zone? The objections to a Grexit are twofold: first, that its currency — presumably a newly issued drachma — would be walloped by an unfavorable exchange rate as a result. Foreign goods and foreign travel would be priced out of many families’ reach. At the same time, as euro zone leaders have warned continually, a Grexit would also shake the euro to its foundations — and though the remaining 18 members could be protected, a precedent would be set that this is a contingent currency, with membership dependent on national conditions.\n"
				"\n"
				"That it would be bad is certain: but how much worse than staying in and swallowing bitter medicine? As for the future of the euro, it would no longer be the Greeks’ problem. What, they may say, has the euro done for us?\n"
				"\n"
				"A Grexit would allow Greece to focus its national pride on reviving the economy — rather than railing at Germany — tempting the most talented members of the wide Greek diaspora to come back to be part of a patriotic project, digging deep into the wells of national talent to fashion a new image for a country that has great beauty, the richest of histories and an economy waiting to be energized.\n"
				"\n"
				"To accomplish this would require a few self-mandates: for starters, an all-out assault on corruption and a determination to make the oligarchs of shipping and the hierarchs of the Orthodox Church pay their fair share of taxes. Socialism would not be part of it: Syriza would have to be disabused of that notion. Tsipras has shown he’s flexible enough to bend every which way, and if he is as attached to power (and skillful in keeping it) as he seems, then he may yet prove a useful politician, a late convert to social democracy.\n"
				"\n"
				"The Greeks say that they are a “proud people.” But that’s been somewhat hollow through the years of free riding. Now, nothing could make that boast more concrete than taking control of their own destiny, and showing Europe that the country is not the pain in the backside  it has proven to be this past year, but a nation, once again, of creators.\n"
				"\n"
				"The euro has crushed them, and other small and relatively poor economies have taken note. But Greece can step out of all of that. The cause that the British romantic poet Lord Byron made his own in the early 19th century should be revived. Liberation for the Greeks!\n"
				"";
		#line 87 "home.html"
		} // end of template mainContent
	#line 89 "home.html"
	}; // end of class HomeView
#line 90 "home.html"
} // end of namespace skdefault
#line 1 "error.html"
#include "../sewebmodels/CError.h" 
#line 2 "error.html"
namespace skdefault {
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
} // end of namespace skdefault
#line 1 "signon.html"
#include "../sewebmodels/CLogOn.h" 
#line 2 "signon.html"
namespace skdefault {
	#line 3 "signon.html"
	struct SignOnView :public MasterView
	#line 3 "signon.html"
	{
	#line 3 "signon.html"
		CLogOn &content;
	#line 3 "signon.html"
		SignOnView(std::ostream &_s,CLogOn &_content): MasterView(_s,_content),content(_content)
	#line 3 "signon.html"
		{
	#line 3 "signon.html"
		}
		#line 5 "signon.html"
		virtual void title() {
			#line 5 "signon.html"
			out()<<"Earn Smart | Sign On";
		#line 5 "signon.html"
		} // end of template title
		#line 8 "signon.html"
		virtual void mainContent() {
			#line 17 "signon.html"
			out()<<"\n"
				"\n"
				"<div id=\"dvSigonControls\">\n"
				"    \n"
				"    <h2>Welcome</h2>\n"
				"    <h3>Sign into your account</h3>\n"
				"    <label class=\"requireslabel\">* - required</label>\n"
				"    <div>\n"
				"        <div>\n"
				"            <input name=\"signEmail\" type=\"email\" placeholder=\"email address\" value=\"";
			#line 17 "signon.html"
			out()<<cppcms::filters::escape(content.EmailAddress);
			#line 18 "signon.html"
			out()<<"\" title=\"Enter a valid email address.\">\n"
				"            <label class=\"requireslabel\">";
			#line 18 "signon.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 19 "signon.html"
			out()<<"</label>\n"
				"            ";
			#line 19 "signon.html"
			if(!(content.get_IsValid())) {
				#line 20 "signon.html"
				out()<<"\n"
					"            <label class=\"validationtextlabel\">";
				#line 20 "signon.html"
				out()<<cppcms::filters::escape(content.EmailValidationText());
				#line 21 "signon.html"
				out()<<"</label>\n"
					"            ";
			#line 21 "signon.html"
			} // endif
			#line 25 "signon.html"
			out()<<"\n"
				"        </div>\n"
				"        <div>\n"
				"            <input name=\"signPwd\" type=\"password\" placeholder='password' title=\"Enter a password between 8 and 25 characters\">\n"
				"            <label class=\"requireslabel\">";
			#line 25 "signon.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 26 "signon.html"
			out()<<"</label>\n"
				"            ";
			#line 26 "signon.html"
			if(!(content.get_IsValid())) {
				#line 27 "signon.html"
				out()<<"\n"
					"            <label class=\"validationtextlabel\">";
				#line 27 "signon.html"
				out()<<cppcms::filters::escape(content.PwdValidationText());
				#line 28 "signon.html"
				out()<<"</label>\n"
					"            ";
			#line 28 "signon.html"
			} // endif
			#line 31 "signon.html"
			out()<<"\n"
				"        </div>\n"
				"        <div>\n"
				"            ";
			#line 31 "signon.html"
			if(content.Remember) {
				#line 33 "signon.html"
				out()<<"\n"
					"            <input name=\"signRem\" type=\"checkbox\" value=\"Remember\" checked>Remember me\n"
					"            ";
			#line 33 "signon.html"
			}else{
				#line 35 "signon.html"
				out()<<"\n"
					"            <input name=\"signRem\" type=\"checkbox\" value=\"yes\">Remember me\n"
					"            ";
			#line 35 "signon.html"
			}
			#line 40 "signon.html"
			out()<<"\n"
				"        </div>\n"
				"        <div>\n"
				"            <button>Login</button>\n"
				"        </div>\n"
				"        ";
			#line 40 "signon.html"
			if(!(content.get_IsValid())) {
				#line 41 "signon.html"
				out()<<"\n"
					"        ";
				#line 41 "signon.html"
				string s = content.CommonValidationText(); 
				#line 42 "signon.html"
				out()<<"\n"
					"        ";
				#line 42 "signon.html"
				if (!s.empty()) { 
				#line 44 "signon.html"
				out()<<"\n"
					"        <div>\n"
					"            <label class=\"validationtextlabel\">";
				#line 44 "signon.html"
				out() << s; 
				#line 46 "signon.html"
				out()<<"</label>\n"
					"        </div>\n"
					"        ";
				#line 46 "signon.html"
				} 
				#line 47 "signon.html"
				out()<<"\n"
					"        ";
			#line 47 "signon.html"
			} // endif
			#line 62 "signon.html"
			out()<<"\n"
				"        <div>\n"
				"            <h4>\n"
				"            Have you forgot your password? \n"
				"            </h4>\n"
				"            Click <a href=\"/resetpwd\">here</a> to reset your password.\n"
				"            <hr>\n"
				"            <h4>\n"
				"            Don't have an account yet? \n"
				"            </h4>\n"
				"            Click <a href=\"/register\">here</a> to register.\n"
				"        </div>\n"
				"    </div>\n"
				"</div>\n"
				"\n"
				"";
		#line 62 "signon.html"
		} // end of template mainContent
	#line 65 "signon.html"
	}; // end of class SignOnView
#line 66 "signon.html"
} // end of namespace skdefault
#line 1 "registration.html"
#include "../sewebmodels/CRegistration.h" 
#line 2 "registration.html"
namespace skdefault {
	#line 3 "registration.html"
	struct RegistrationView :public MasterView
	#line 3 "registration.html"
	{
	#line 3 "registration.html"
		CRegistration &content;
	#line 3 "registration.html"
		RegistrationView(std::ostream &_s,CRegistration &_content): MasterView(_s,_content),content(_content)
	#line 3 "registration.html"
		{
	#line 3 "registration.html"
		}
		#line 5 "registration.html"
		virtual void title() {
			#line 5 "registration.html"
			out()<<"Earn Smart | Register";
		#line 5 "registration.html"
		} // end of template title
		#line 8 "registration.html"
		virtual void mainContent() {
			#line 11 "registration.html"
			out()<<"\n"
				"\n"
				"<div id=\"dvSigonControls\">\n"
				"    <input type=\"hidden\" name=\"regStage\" value=\"";
			#line 11 "registration.html"
			out() << (I32)content.Data.Stage; 
			#line 12 "registration.html"
			out()<<"\"/>\n"
				"    ";
			#line 12 "registration.html"
			if ((I32)content.Data.Stage == (I32)CRegistration::RegStages::UserTypeAndAddress) { 
			#line 19 "registration.html"
			out()<<"\n"
				"    <h2>Welcome</h2>\n"
				"    <h3>Register as a basic or licensed customer</h3>\n"
				"    <label class=\"requireslabel\">* - required</label>\n"
				"    <div>\n"
				"        <ul>\n"
				"            <li>\n"
				"                ";
			#line 19 "registration.html"
			if(content.Data.Basic) {
				#line 21 "registration.html"
				out()<<"\n"
					"                <input type=\"radio\" name=\"regUsertype\" value=\"Basic\" checked>Basic\n"
					"                ";
			#line 21 "registration.html"
			}else{
				#line 23 "registration.html"
				out()<<"\n"
					"                <input type=\"radio\" name=\"regUsertype\" value=\"Basic\">Basic\n"
					"                ";
			#line 23 "registration.html"
			}
			#line 31 "registration.html"
			out()<<"\n"
				"                <div>\n"
				"                    A basic user provides an email address and a password to login to the site.\n"
				"                    Basic user can view advanced predictions on the performance of publicly listed companies\n"
				"                    registered in the USA and build portfolios based on fundamental performance.\n"
				"                </div>\n"
				"            </li>\n"
				"            <li>\n"
				"                ";
			#line 31 "registration.html"
			if(content.Data.Basic) {
				#line 33 "registration.html"
				out()<<"\n"
					"                <input type=\"radio\" name=\"regUsertype\" value=\"Licensed\">Licensed\n"
					"                ";
			#line 33 "registration.html"
			}else{
				#line 35 "registration.html"
				out()<<"\n"
					"                <input type=\"radio\" name=\"regUsertype\" value=\"Licensed\" checked>Licensed\n"
					"                ";
			#line 35 "registration.html"
			}
			#line 49 "registration.html"
			out()<<"\n"
				"                <div>\n"
				"                    A licensed user is a monthly paid subscriber. While registering as a licensed user, the user\n"
				"                    is also simultaneously registering with an exchange market data provider for \n"
				"                    daily end of the day market prices. The exchanges are selected in the process the user.\n"
				"                    The licensed user will provide an email address and password to login to the site \n"
				"                    in addition to full address and a Paypal subscription account. The account can be \n"
				"                    closed or switched to a basic account at any time by going to the user's accounts page.\n"
				"                </div>\n"
				"            </li>\n"
				"        </ul>\n"
				"    </div>\n"
				"    <div>\n"
				"        <div>\n"
				"            <input name=\"regFirstName\" type=\"text\" placeholder=\"first name\" value=\"";
			#line 49 "registration.html"
			out()<<cppcms::filters::escape(content.Data.FirstName);
			#line 50 "registration.html"
			out()<<"\" title=\"Enter First Name.\">\n"
				"            <label class=\"requireslabel\">";
			#line 50 "registration.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 51 "registration.html"
			out()<<"</label>\n"
				"            ";
			#line 51 "registration.html"
			if(!(content.get_IsValid())) {
				#line 52 "registration.html"
				out()<<"\n"
					"            <label class=\"validationtextlabel\">";
				#line 52 "registration.html"
				out()<<cppcms::filters::escape(content.FirstNameValidationText());
				#line 53 "registration.html"
				out()<<"</label>\n"
					"            ";
			#line 53 "registration.html"
			} // endif
			#line 56 "registration.html"
			out()<<"\n"
				"        </div>\n"
				"        <div>\n"
				"            <input name=\"regLastName\" type=\"text\" placeholder=\"last name\" value=\"";
			#line 56 "registration.html"
			out()<<cppcms::filters::escape(content.Data.LastName);
			#line 57 "registration.html"
			out()<<"\" title=\"Enter Last Name.\">\n"
				"            <label class=\"requireslabel\">";
			#line 57 "registration.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 58 "registration.html"
			out()<<"</label>\n"
				"            ";
			#line 58 "registration.html"
			if(!(content.get_IsValid())) {
				#line 59 "registration.html"
				out()<<"\n"
					"            <label class=\"validationtextlabel\">";
				#line 59 "registration.html"
				out()<<cppcms::filters::escape(content.SecondNameValidationText());
				#line 60 "registration.html"
				out()<<"</label>\n"
					"            ";
			#line 60 "registration.html"
			} // endif
			#line 63 "registration.html"
			out()<<"\n"
				"        </div>\n"
				"        <div>\n"
				"            <input name=\"regEmail\" type=\"email\" placeholder=\"email address\" value=\"";
			#line 63 "registration.html"
			out()<<cppcms::filters::escape(content.Data.EmailAddress);
			#line 64 "registration.html"
			out()<<"\" title=\"Enter a valid email address.\">\n"
				"            <label class=\"requireslabel\">";
			#line 64 "registration.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 65 "registration.html"
			out()<<"</label>\n"
				"            <label id=\"lblRegEmail\" class=\"validationtextlabel\">";
			#line 65 "registration.html"
			out()<<cppcms::filters::escape(content.EmailValidationText());
			#line 69 "registration.html"
			out()<<"</label>\n"
				"        </div>\n"
				"        <div>\n"
				"            <input name=\"regPwd\" type=\"password\" placeholder='password' title=\"Enter a password between 8 and 25 characters\">\n"
				"            <label class=\"requireslabel\">";
			#line 69 "registration.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 70 "registration.html"
			out()<<"</label>\n"
				"            ";
			#line 70 "registration.html"
			if(!(content.get_IsValid())) {
				#line 71 "registration.html"
				out()<<"\n"
					"            <label class=\"validationtextlabel\">";
				#line 71 "registration.html"
				out()<<cppcms::filters::escape(content.PwdValidationText());
				#line 72 "registration.html"
				out()<<"</label>\n"
					"            ";
			#line 72 "registration.html"
			} // endif
			#line 76 "registration.html"
			out()<<"\n"
				"        </div>\n"
				"        <div>\n"
				"            <input name=\"regPwdConfirm\" type=\"password\" placeholder='confirm password' title=\"Confirm new password\">\n"
				"            <label class=\"requireslabel\">";
			#line 76 "registration.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 77 "registration.html"
			out()<<"</label>\n"
				"            ";
			#line 77 "registration.html"
			if(!(content.get_IsValid())) {
				#line 78 "registration.html"
				out()<<"\n"
					"            <label class=\"validationtextlabel\">";
				#line 78 "registration.html"
				out()<<cppcms::filters::escape(content.PwdConfirmValidationText());
				#line 79 "registration.html"
				out()<<"</label>\n"
					"            ";
			#line 79 "registration.html"
			} // endif
			#line 82 "registration.html"
			out()<<"\n"
				"        </div>\n"
				"        <div>\n"
				"            ";
			#line 82 "registration.html"
			if(content.Data.Remember) {
				#line 84 "registration.html"
				out()<<"\n"
					"            <input name=\"regRem\" type=\"checkbox\" checked value=\"rem\">Remember me\n"
					"            ";
			#line 84 "registration.html"
			}else{
				#line 86 "registration.html"
				out()<<"\n"
					"            <input name=\"regRem\" type=\"checkbox\"  value=\"rem\">Remember me\n"
					"            ";
			#line 86 "registration.html"
			}
			#line 88 "registration.html"
			out()<<"\n"
				"        </div>\n"
				"        ";
			#line 88 "registration.html"
			if (!content.Data.Basic) { 
			#line 90 "registration.html"
			out()<<"\n"
				"        <div>\n"
				"            <input name=\"regStreet1\" type=\"text\" placeholder=\"house/apt/street\" value=\"";
			#line 90 "registration.html"
			out()<<cppcms::filters::escape(content.Data.Street1);
			#line 91 "registration.html"
			out()<<"\" title=\"Enter House or Apartment # and Street Name.\">\n"
				"            <label class=\"requireslabel\">";
			#line 91 "registration.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 92 "registration.html"
			out()<<"</label>\n"
				"            ";
			#line 92 "registration.html"
			if(!(content.get_IsValid())) {
				#line 93 "registration.html"
				out()<<"\n"
					"            <label class=\"validationtextlabel\">";
				#line 93 "registration.html"
				out()<<cppcms::filters::escape(content.Street1ValidationText());
				#line 94 "registration.html"
				out()<<"</label>\n"
					"            ";
			#line 94 "registration.html"
			} // endif
			#line 97 "registration.html"
			out()<<"\n"
				"        </div>\n"
				"        <div>\n"
				"            <input name=\"regStreet2\" type=\"text\" placeholder=\"optional address\" value=\"";
			#line 97 "registration.html"
			out()<<cppcms::filters::escape(content.Data.Street2);
			#line 100 "registration.html"
			out()<<"\" title=\"Optinally Enter Additional Address Line.\">\n"
				"        </div>\n"
				"        <div>\n"
				"            <input name=\"regTown\" type=\"text\" placeholder=\"town or city\" value=\"";
			#line 100 "registration.html"
			out()<<cppcms::filters::escape(content.Data.Town);
			#line 101 "registration.html"
			out()<<"\" title=\"Enter Town or City Name.\">\n"
				"            <label class=\"requireslabel\">";
			#line 101 "registration.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 102 "registration.html"
			out()<<"</label>\n"
				"            ";
			#line 102 "registration.html"
			if(!(content.get_IsValid())) {
				#line 103 "registration.html"
				out()<<"\n"
					"            <label class=\"validationtextlabel\">";
				#line 103 "registration.html"
				out()<<cppcms::filters::escape(content.TownValidationText());
				#line 104 "registration.html"
				out()<<"</label>\n"
					"            ";
			#line 104 "registration.html"
			} // endif
			#line 110 "registration.html"
			out()<<"\n"
				"        </div>\n"
				"        <div>\n"
				"            <div><label for=\"regCountries\">Country:</label></div>\n"
				"            <select name=\"regCountries\" title=\"Select Country\">\n"
				"                <option value=\"\">--Not Selected--</option>\n"
				"                ";
			#line 110 "registration.html"
			if(content.Countries) {
				#line 111 "registration.html"
				out()<<"\n"
					"                ";
				#line 111 "registration.html"
				if((*content.Countries).begin()!=(*content.Countries).end()) {
					#line 112 "registration.html"
					out()<<"\n"
						"                ";
					#line 112 "registration.html"
					for(CPPCMS_TYPEOF((*content.Countries).begin()) country_ptr=(*content.Countries).begin(),country_ptr_end=(*content.Countries).end();country_ptr!=country_ptr_end;++country_ptr) {
					#line 112 "registration.html"
					CPPCMS_TYPEOF(*country_ptr) &country=*country_ptr;
						#line 113 "registration.html"
						out()<<"\n"
							"                <option value=\"";
						#line 113 "registration.html"
						out()<<cppcms::filters::escape(country->get_TwoCharCode());
						#line 113 "registration.html"
						out()<<"\" ";
						#line 113 "registration.html"
						(content.Data.Country == country->get_TwoCharCode() ? out() << " selected " : out() << ""); 
						#line 114 "registration.html"
						out()<<" >\n"
							"                    ";
						#line 114 "registration.html"
						out()<<cppcms::filters::escape(country->get_Name());
						#line 116 "registration.html"
						out()<<"\n"
							"                </option>\n"
							"                ";
					#line 116 "registration.html"
					} // end of item
					#line 117 "registration.html"
					out()<<"\n"
						"                ";
				#line 117 "registration.html"
				}
				#line 118 "registration.html"
				out()<<"\n"
					"                ";
			#line 118 "registration.html"
			} // endif
			#line 120 "registration.html"
			out()<<"\n"
				"            </select>\n"
				"            <label class=\"requireslabel\">";
			#line 120 "registration.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 121 "registration.html"
			out()<<"</label>\n"
				"            ";
			#line 121 "registration.html"
			if(!(content.get_IsValid())) {
				#line 122 "registration.html"
				out()<<"\n"
					"            <label class=\"validationtextlabel\">";
				#line 122 "registration.html"
				out()<<cppcms::filters::escape(content.CountryValidationText());
				#line 123 "registration.html"
				out()<<"</label>\n"
					"            ";
			#line 123 "registration.html"
			} // endif
			#line 129 "registration.html"
			out()<<"\n"
				"        </div>\n"
				"        <div>\n"
				"            <div><label for=\"regStates\">State:</label></div>\n"
				"            <select name=\"regStates\" title=\"Select State\">\n"
				"                <option value=\"\">--Not Selected--</option>\n"
				"                ";
			#line 129 "registration.html"
			if(content.States) {
				#line 130 "registration.html"
				out()<<"\n"
					"                ";
				#line 130 "registration.html"
				if((*content.States).begin()!=(*content.States).end()) {
					#line 131 "registration.html"
					out()<<"\n"
						"                ";
					#line 131 "registration.html"
					for(CPPCMS_TYPEOF((*content.States).begin()) state_ptr=(*content.States).begin(),state_ptr_end=(*content.States).end();state_ptr!=state_ptr_end;++state_ptr) {
					#line 131 "registration.html"
					CPPCMS_TYPEOF(*state_ptr) &state=*state_ptr;
						#line 132 "registration.html"
						out()<<"\n"
							"                <option value=\"";
						#line 132 "registration.html"
						out()<<cppcms::filters::escape(state->get_Code());
						#line 132 "registration.html"
						out()<<"\" ";
						#line 132 "registration.html"
						(content.Data.State == state->get_Code() ? out() << " selected " : out() << ""); 
						#line 133 "registration.html"
						out()<<" >\n"
							"                    ";
						#line 133 "registration.html"
						out()<<cppcms::filters::escape(state->get_Name());
						#line 135 "registration.html"
						out()<<"\n"
							"                </option>\n"
							"                ";
					#line 135 "registration.html"
					} // end of item
					#line 136 "registration.html"
					out()<<"\n"
						"                ";
				#line 136 "registration.html"
				}
				#line 137 "registration.html"
				out()<<"\n"
					"                ";
			#line 137 "registration.html"
			} // endif
			#line 139 "registration.html"
			out()<<"\n"
				"            </select>\n"
				"            <label class=\"requireslabel\">";
			#line 139 "registration.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 140 "registration.html"
			out()<<"</label>\n"
				"            ";
			#line 140 "registration.html"
			if(!(content.get_IsValid())) {
				#line 141 "registration.html"
				out()<<"\n"
					"            <label class=\"validationtextlabel\">";
				#line 141 "registration.html"
				out()<<cppcms::filters::escape(content.StateValidationText());
				#line 142 "registration.html"
				out()<<"</label>\n"
					"            ";
			#line 142 "registration.html"
			} // endif
			#line 145 "registration.html"
			out()<<"\n"
				"        </div>\n"
				"        <div>\n"
				"            <input name=\"regPostalCode\" type=\"text\" placeholder=\"postal code\" value=\"";
			#line 145 "registration.html"
			out()<<cppcms::filters::escape(content.Data.PostalCode);
			#line 146 "registration.html"
			out()<<"\" title=\"Enter Postal Code.\">\n"
				"            <label class=\"requireslabel\">";
			#line 146 "registration.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 147 "registration.html"
			out()<<"</label>\n"
				"            ";
			#line 147 "registration.html"
			if(!(content.get_IsValid())) {
				#line 148 "registration.html"
				out()<<"\n"
					"            <label class=\"validationtextlabel\">";
				#line 148 "registration.html"
				out()<<cppcms::filters::escape(content.PostalCodeValidationText());
				#line 149 "registration.html"
				out()<<"</label>\n"
					"            ";
			#line 149 "registration.html"
			} // endif
			#line 151 "registration.html"
			out()<<"\n"
				"        </div>\n"
				"        ";
			#line 151 "registration.html"
			} 
			#line 158 "registration.html"
			out()<<"\n"
				"        \n"
				"        \n"
				"        <div>\n"
				"            <button id=\"btnRegSteps\" type=\"button\">Register</button>\n"
				"        </div>\n"
				"        \n"
				"        ";
			#line 158 "registration.html"
			if(!(content.get_IsValid())) {
				#line 159 "registration.html"
				out()<<"\n"
					"        ";
				#line 159 "registration.html"
				string s = content.CommonValidationText(); 
				#line 160 "registration.html"
				out()<<"\n"
					"        ";
				#line 160 "registration.html"
				if (!s.empty()) { 
				#line 162 "registration.html"
				out()<<"\n"
					"        <div>\n"
					"            <label class=\"validationtextlabel\">";
				#line 162 "registration.html"
				out() << s; 
				#line 164 "registration.html"
				out()<<"</label>\n"
					"        </div>\n"
					"        ";
				#line 164 "registration.html"
				} 
				#line 165 "registration.html"
				out()<<"\n"
					"        ";
			#line 165 "registration.html"
			} // endif
			#line 174 "registration.html"
			out()<<"\n"
				"        <div>\n"
				"            <hr>\n"
				"            <h4>\n"
				"            Already registered ? \n"
				"            </h4>\n"
				"            Click <a href=\"/signon\">here</a> to sign in page.\n"
				"        </div>\n"
				"    </div>\n"
				"    ";
			#line 174 "registration.html"
			} else if ( (I32)content.Data.Stage == (I32)CRegistration::RegStages::SendEmail ) { 
			#line 178 "registration.html"
			out()<<"\n"
				"    <h2>Registration</h2>\n"
				"    <h3>Email Confirmation</h3>\n"
				"    <p>\n"
				"        An email verification is dispatched to your email address ";
			#line 178 "registration.html"
			out()<<cppcms::filters::escape(content.Data.EmailAddress);
			#line 183 "registration.html"
			out()<<". \n"
				"        Please click the registration link in the email message to open it in your default internet browser, \n"
				"        or copy that link on a browser address input box to continue with registration.\n"
				"    </p>\n"
				"    <p>\n"
				"        If the email address is wrong, click <a href=\"/register/";
			#line 183 "registration.html"
			out()<<cppcms::filters::escape(content.Data.Guid);
			#line 186 "registration.html"
			out()<<"\">here</a> to return to \n"
				"        registration page and provide correct email address.\n"
				"    </p>\n"
				"    ";
			#line 186 "registration.html"
			} else if ( (I32)content.Data.Stage == (I32)CRegistration::RegStages::EmailConfirmation ) { 
			#line 187 "registration.html"
			out()<<"\n"
				"    ";
			#line 187 "registration.html"
			if(content.Data.Basic) {
				#line 192 "registration.html"
				out()<<"\n"
					"    <h2>Registration</h2>\n"
					"    <h3>Register as a basic customer</h3>\n"
					"    <div>\n"
					"        <p>\n"
					"            Your email address ";
				#line 192 "registration.html"
				out()<<cppcms::filters::escape(content.Data.EmailAddress);
				#line 198 "registration.html"
				out()<<" is confirmed. You have successfully registered as a basic user. \n"
					"        </p>\n"
					"        <p>\n"
					"            Please click <a href=\"/signon\">here</a> to sign in.\n"
					"        </p>\n"
					"    </div>\n"
					"    ";
			#line 198 "registration.html"
			}else{
				#line 203 "registration.html"
				out()<<"\n"
					"    <h2>Registration</h2>\n"
					"    <h3>Register as a licensed customer</h3>\n"
					"    <div>\n"
					"        <p>\n"
					"            Your email address ";
				#line 203 "registration.html"
				out()<<cppcms::filters::escape(content.Data.EmailAddress);
				#line 244 "registration.html"
				out()<<" is confirmed. \n"
					"        </p>\n"
					"        <p>\n"
					"            To become a licensed user, please chose below the exchanges you would like\n"
					"            data from. We pay a license fee to the selected exchanges on your behalf.\n"
					"            The amount that you will pay monthly to us as a licensee will be summed at the total\n"
					"            line in the table below.\n"
					"        </p>\n"
					"        <table>\n"
					"            <caption>Select Services/Exchanges</caption>\n"
					"            <thead>\n"
					"                <th>\n"
					"                </th>\n"
					"                <th>\n"
					"                    Services/Exchanges\n"
					"                </th>\n"
					"                <th>\n"
					"                    Fee\n"
					"                </th>\n"
					"            </thead>\n"
					"            <tr>\n"
					"                <td colspan=\"2\">Total</td>\n"
					"                <td></td>\n"
					"            </tr>\n"
					"        </table>\n"
					"        <p>\n"
					"            After making selections above, please click below to provide the payment information. \n"
					"            At the moment only Paypal payment gateway is supported.\n"
					"        </p>\n"
					"        <p>\n"
					"            Your subscription will begin immediately after completion of registration steps. \n"
					"            You may cancel your service any time. After cancellation, you can continue\n"
					"            using our service until the end of the billing cycle.\n"
					"        </p>\n"
					"        <div>\n"
					"            <figure>\n"
					"                <img id=\"btnRegSteps\" src=\"/images/paypal_btn_subscribeCC_LG.gif\" title=\"Paypal subscription\">\n"
					"            </figure>\n"
					"        </div>\n"
					"        \n"
					"    </div>\n"
					"    ";
			#line 244 "registration.html"
			}
			#line 245 "registration.html"
			out()<<"\n"
				"    ";
			#line 245 "registration.html"
			} else if ( !content.Data.Basic and (I32)content.Data.Stage == (I32)CRegistration::RegStages::Payment ) { 
			#line 246 "registration.html"
			out()<<"\n"
				"    ";
			#line 246 "registration.html"
			} else if ( (I32)content.Data.Stage == (I32)CRegistration::RegStages::Completed ) { 
			#line 247 "registration.html"
			out()<<"\n"
				"    ";
			#line 247 "registration.html"
			} 
			#line 250 "registration.html"
			out()<<"\n"
				"</div>\n"
				"\n"
				"";
		#line 250 "registration.html"
		} // end of template mainContent
	#line 253 "registration.html"
	}; // end of class RegistrationView
#line 254 "registration.html"
} // end of namespace skdefault
#line 1 "resetpwd.html"
#include "../sewebmodels/CResetPwd.h" 
#line 2 "resetpwd.html"
namespace skdefault {
	#line 3 "resetpwd.html"
	struct ResetPwdView :public MasterView
	#line 3 "resetpwd.html"
	{
	#line 3 "resetpwd.html"
		CResetPwd &content;
	#line 3 "resetpwd.html"
		ResetPwdView(std::ostream &_s,CResetPwd &_content): MasterView(_s,_content),content(_content)
	#line 3 "resetpwd.html"
		{
	#line 3 "resetpwd.html"
		}
		#line 5 "resetpwd.html"
		virtual void title() {
			#line 5 "resetpwd.html"
			out()<<"Earn Smart | Reset Password";
		#line 5 "resetpwd.html"
		} // end of template title
		#line 8 "resetpwd.html"
		virtual void mainContent() {
			#line 17 "resetpwd.html"
			out()<<"\n"
				"\n"
				"<div id=\"dvSigonControls\">\n"
				"    \n"
				"    <h2>Reset Password</h2>\n"
				"    <h3>Enter your email address and we will email you the new password.</h3>\n"
				"    <label class=\"requireslabel\">* - required</label>\n"
				"    <div>\n"
				"        <div>\n"
				"            <input name=\"signEmail\" type=\"email\" placeholder=\"Email Address\" value=\"";
			#line 17 "resetpwd.html"
			out()<<cppcms::filters::escape(content.EmailAddress);
			#line 18 "resetpwd.html"
			out()<<"\" title=\"Enter a valid email address.\">\n"
				"            <label class=\"requireslabel\">";
			#line 18 "resetpwd.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 19 "resetpwd.html"
			out()<<"</label>\n"
				"            ";
			#line 19 "resetpwd.html"
			if(!(content.get_IsValid())) {
				#line 20 "resetpwd.html"
				out()<<"\n"
					"            <label class=\"validationtextlabel\">";
				#line 20 "resetpwd.html"
				out()<<cppcms::filters::escape(content.EmailValidationText());
				#line 21 "resetpwd.html"
				out()<<"</label>\n"
					"            ";
			#line 21 "resetpwd.html"
			} // endif
			#line 26 "resetpwd.html"
			out()<<"\n"
				"        </div>\n"
				"        <div>\n"
				"            <button>Reset Password</button>\n"
				"        </div>\n"
				"        ";
			#line 26 "resetpwd.html"
			if(!(content.get_IsValid())) {
				#line 27 "resetpwd.html"
				out()<<"\n"
					"        ";
				#line 27 "resetpwd.html"
				string s = content.CommonValidationText(); 
				#line 28 "resetpwd.html"
				out()<<"\n"
					"        ";
				#line 28 "resetpwd.html"
				if (!s.empty()) { 
				#line 30 "resetpwd.html"
				out()<<"\n"
					"        <div>\n"
					"            <label class=\"validationtextlabel\">";
				#line 30 "resetpwd.html"
				out() << s; 
				#line 32 "resetpwd.html"
				out()<<"</label>\n"
					"        </div>\n"
					"        ";
				#line 32 "resetpwd.html"
				} 
				#line 33 "resetpwd.html"
				out()<<"\n"
					"        ";
			#line 33 "resetpwd.html"
			} // endif
			#line 37 "resetpwd.html"
			out()<<"\n"
				"    </div>\n"
				"</div>\n"
				"    \n"
				"";
		#line 37 "resetpwd.html"
		} // end of template mainContent
	#line 40 "resetpwd.html"
	}; // end of class ResetPwdView
#line 41 "resetpwd.html"
} // end of namespace skdefault
#line 1 "account.html"
#include "../sewebmodels/CAccount.h" 
#line 2 "account.html"
namespace skdefault {
	#line 3 "account.html"
	struct AccountView :public MasterView
	#line 3 "account.html"
	{
	#line 3 "account.html"
		CAccount &content;
	#line 3 "account.html"
		AccountView(std::ostream &_s,CAccount &_content): MasterView(_s,_content),content(_content)
	#line 3 "account.html"
		{
	#line 3 "account.html"
		}
		#line 5 "account.html"
		virtual void title() {
			#line 5 "account.html"
			out()<<"Earn Smart | Account";
		#line 5 "account.html"
		} // end of template title
		#line 8 "account.html"
		virtual void mainContent() {
			#line 15 "account.html"
			out()<<"\n"
				"\n"
				"<span >\n"
				"\n"
				"\n"
				"</span>\n"
				"\n"
				"";
		#line 15 "account.html"
		} // end of template mainContent
	#line 18 "account.html"
	}; // end of class AccountView
#line 19 "account.html"
} // end of namespace skdefault
#line 1 "pwd.html"
#include "../sewebmodels/CPwd.h" 
#line 2 "pwd.html"
namespace skdefault {
	#line 3 "pwd.html"
	struct PwdView :public MasterView
	#line 3 "pwd.html"
	{
	#line 3 "pwd.html"
		CPwd &content;
	#line 3 "pwd.html"
		PwdView(std::ostream &_s,CPwd &_content): MasterView(_s,_content),content(_content)
	#line 3 "pwd.html"
		{
	#line 3 "pwd.html"
		}
		#line 5 "pwd.html"
		virtual void title() {
			#line 5 "pwd.html"
			out()<<"Earn Smart | Sign On";
		#line 5 "pwd.html"
		} // end of template title
		#line 8 "pwd.html"
		virtual void mainContent() {
			#line 13 "pwd.html"
			out()<<"\n"
				"\n"
				"<div id=\"dvSigonControls\">\n"
				"    \n"
				"    <h2>Change Password</h2>\n"
				"    <h4>Your email address is ";
			#line 13 "pwd.html"
			out()<<cppcms::filters::escape(content.EmailAddress);
			#line 18 "pwd.html"
			out()<<". Change your password below.</h4>\n"
				"    <label class=\"requireslabel\">* - required</label>\n"
				"    <div>\n"
				"        <div>\n"
				"            <input name=\"signPwd\" type=\"password\" placeholder='current password' title=\"Enter current password\">\n"
				"            <label class=\"requireslabel\">";
			#line 18 "pwd.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 22 "pwd.html"
			out()<<"</label>\n"
				"        </div>\n"
				"        <div>\n"
				"            <input name=\"signNewPwd\" type=\"password\" placeholder='password' title=\"Enter new password characters between 8 and 25 characters\">\n"
				"            <label class=\"requireslabel\">";
			#line 22 "pwd.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 26 "pwd.html"
			out()<<"</label>\n"
				"        </div>\n"
				"        <div>\n"
				"            <input name=\"signNewPwdConfirm\" type=\"password\" placeholder='confirm password' title=\"Confirm new password\">\n"
				"            <label class=\"requireslabel\">";
			#line 26 "pwd.html"
			out()<<cppcms::filters::escape(content.RequiredStr);
			#line 31 "pwd.html"
			out()<<"</label>\n"
				"        </div>\n"
				"        <div>\n"
				"            <button>Change Password</button>\n"
				"        </div>\n"
				"        ";
			#line 31 "pwd.html"
			if(!(content.get_IsValid())) {
				#line 32 "pwd.html"
				out()<<"\n"
					"        ";
				#line 32 "pwd.html"
				string s = content.CommonValidationText(); 
				#line 33 "pwd.html"
				out()<<"\n"
					"        ";
				#line 33 "pwd.html"
				if (!s.empty()) { 
				#line 35 "pwd.html"
				out()<<"\n"
					"        <div>\n"
					"            <label class=\"validationtextlabel\">";
				#line 35 "pwd.html"
				out() << s; 
				#line 37 "pwd.html"
				out()<<"</label>\n"
					"        </div>\n"
					"        ";
				#line 37 "pwd.html"
				} 
				#line 38 "pwd.html"
				out()<<"\n"
					"        ";
			#line 38 "pwd.html"
			} // endif
			#line 49 "pwd.html"
			out()<<"\n"
				"        <div>\n"
				"            <hr>\n"
				"            <h4>\n"
				"            Do you want to reset password again ? \n"
				"            </h4>\n"
				"            Click <a href=\"/resetpwd\">here</a> to reset your password again.\n"
				"        </div>\n"
				"    </div>\n"
				"</div>\n"
				"\n"
				"";
		#line 49 "pwd.html"
		} // end of template mainContent
	#line 52 "pwd.html"
	}; // end of class PwdView
#line 53 "pwd.html"
} // end of namespace skdefault
#line 1 "charts.html"
#include "../sewebmodels/CSecurityCharts.h" 
#line 2 "charts.html"
namespace skdefault {
	#line 3 "charts.html"
	struct ChartsView :public MasterView
	#line 3 "charts.html"
	{
	#line 3 "charts.html"
		CSecurityCharts &content;
	#line 3 "charts.html"
		ChartsView(std::ostream &_s,CSecurityCharts &_content): MasterView(_s,_content),content(_content)
	#line 3 "charts.html"
		{
	#line 3 "charts.html"
		}
		#line 5 "charts.html"
		virtual void title() {
			#line 5 "charts.html"
			out()<<" Earn Smart | Investment Products | Charts ";
		#line 5 "charts.html"
		} // end of template title
		#line 8 "charts.html"
		virtual void mainContent() {
			#line 12 "charts.html"
			out()<<"\n"
				"\n"
				"Charts view\n"
				"\n"
				"";
		#line 12 "charts.html"
		} // end of template mainContent
	#line 14 "charts.html"
	}; // end of class ChartsView
#line 15 "charts.html"
} // end of namespace skdefault
#line 16 "charts.html"
namespace {
#line 16 "charts.html"
 cppcms::views::generator my_generator; 
#line 16 "charts.html"
 struct loader { 
#line 16 "charts.html"
  loader() { 
#line 16 "charts.html"
   my_generator.name("skdefault");
#line 16 "charts.html"
   my_generator.add_view<skdefault::FooterView,CFooter>("FooterView",true);
#line 16 "charts.html"
   my_generator.add_view<skdefault::ToolbarView,CToolbar>("ToolbarView",true);
#line 16 "charts.html"
   my_generator.add_view<skdefault::AppSearchView,CAppSearch>("AppSearchView",true);
#line 16 "charts.html"
   my_generator.add_view<skdefault::LoginOrRegisterView,CLoginOrRegister>("LoginOrRegisterView",true);
#line 16 "charts.html"
   my_generator.add_view<skdefault::HeaderView,CHeader>("HeaderView",true);
#line 16 "charts.html"
   my_generator.add_view<skdefault::MasterView,CMaster>("MasterView",true);
#line 16 "charts.html"
   my_generator.add_view<skdefault::HomeView,CHome>("HomeView",true);
#line 16 "charts.html"
   my_generator.add_view<skdefault::ErrorView,CError>("ErrorView",true);
#line 16 "charts.html"
   my_generator.add_view<skdefault::SignOnView,CLogOn>("SignOnView",true);
#line 16 "charts.html"
   my_generator.add_view<skdefault::RegistrationView,CRegistration>("RegistrationView",true);
#line 16 "charts.html"
   my_generator.add_view<skdefault::ResetPwdView,CResetPwd>("ResetPwdView",true);
#line 16 "charts.html"
   my_generator.add_view<skdefault::AccountView,CAccount>("AccountView",true);
#line 16 "charts.html"
   my_generator.add_view<skdefault::PwdView,CPwd>("PwdView",true);
#line 16 "charts.html"
   my_generator.add_view<skdefault::ChartsView,CSecurityCharts>("ChartsView",true);
#line 16 "charts.html"
    cppcms::views::pool::instance().add(my_generator);
#line 16 "charts.html"
 }
#line 16 "charts.html"
 ~loader() {  cppcms::views::pool::instance().remove(my_generator); }
#line 16 "charts.html"
} a_loader;
#line 16 "charts.html"
} // anon 
