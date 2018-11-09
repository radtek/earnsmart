#line 1 "emltempls/RegistrationEmailConfirmation.html"
#include "../sewebmodels/CRegistration.h" 
#line 2 "emltempls/RegistrationEmailConfirmation.html"
namespace skdefemailtemplates {
	#line 4 "emltempls/RegistrationEmailConfirmation.html"
	struct RegistrationEmailConfirmationView :public cppcms::base_view
	#line 4 "emltempls/RegistrationEmailConfirmation.html"
	{
	#line 4 "emltempls/RegistrationEmailConfirmation.html"
		CRegistration &content;
	#line 4 "emltempls/RegistrationEmailConfirmation.html"
		RegistrationEmailConfirmationView(std::ostream &_s,CRegistration &_content): cppcms::base_view(_s),content(_content)
	#line 4 "emltempls/RegistrationEmailConfirmation.html"
		{
	#line 4 "emltempls/RegistrationEmailConfirmation.html"
		}
		#line 7 "emltempls/RegistrationEmailConfirmation.html"
		virtual void render() {
			#line 13 "emltempls/RegistrationEmailConfirmation.html"
			out()<<"\n"
				"\n"
				"\n"
				"<html>\n"
				"    <body>\n"
				"        <p>\n"
				"            Dear ";
			#line 13 "emltempls/RegistrationEmailConfirmation.html"
			out()<<cppcms::filters::escape(content.Data.FirstName);
			#line 16 "emltempls/RegistrationEmailConfirmation.html"
			out()<<",\n"
				"        </p>\n"
				"        <p>\n"
				"            Thank you for joining Earnsmart. As a ";
			#line 16 "emltempls/RegistrationEmailConfirmation.html"
			if(content.Data.Basic) {
				#line 17 "emltempls/RegistrationEmailConfirmation.html"
				out()<<" basic member you can enjoy the free analytical products we offer at the site \n"
					"            ";
			#line 17 "emltempls/RegistrationEmailConfirmation.html"
			}else{
				#line 17 "emltempls/RegistrationEmailConfirmation.html"
				out()<<" licensed member you can enjoy advanced analytical products we offer at the site ";
			#line 17 "emltempls/RegistrationEmailConfirmation.html"
			}
			#line 26 "emltempls/RegistrationEmailConfirmation.html"
			out()<<"\n"
				"            and make clear decisions on what investment products that you should trade from time to time. \n"
				"            The projected information you get from this site will possibly assist you to eliminate you of risks \n"
				"            by diversification and hedging of your investment portfolio.\n"
				"        </p>\n"
				"        <p>\n"
				"            To complete the registration, click the below link to verify your email address.\n"
				"        </p>\n"
				"        <p>\n"
				"            <a href=\"";
			#line 26 "emltempls/RegistrationEmailConfirmation.html"
			out()<<cppcms::filters::escape(content.get_RootUrl());
			#line 26 "emltempls/RegistrationEmailConfirmation.html"
			out()<<"/register/1/";
			#line 26 "emltempls/RegistrationEmailConfirmation.html"
			out()<<cppcms::filters::escape(content.Data.Guid);
			#line 26 "emltempls/RegistrationEmailConfirmation.html"
			out()<<"\">http://www.earn-smart.com/register/1/";
			#line 26 "emltempls/RegistrationEmailConfirmation.html"
			out()<<cppcms::filters::escape(content.Data.Guid);
			#line 38 "emltempls/RegistrationEmailConfirmation.html"
			out()<<"</a>\n"
				"        </p>\n"
				"        <p>\n"
				"            Thank you once again.\n"
				"        </p>\n"
				"        <p>\n"
				"            Earnsmart Team.\n"
				"        </p>\n"
				"    </body>\n"
				"</html>\n"
				"\n"
				"\n"
				"";
		#line 38 "emltempls/RegistrationEmailConfirmation.html"
		} // end of template render
	#line 39 "emltempls/RegistrationEmailConfirmation.html"
	}; // end of class RegistrationEmailConfirmationView
#line 40 "emltempls/RegistrationEmailConfirmation.html"
} // end of namespace skdefemailtemplates
#line 41 "emltempls/RegistrationEmailConfirmation.html"
namespace {
#line 41 "emltempls/RegistrationEmailConfirmation.html"
 cppcms::views::generator my_generator; 
#line 41 "emltempls/RegistrationEmailConfirmation.html"
 struct loader { 
#line 41 "emltempls/RegistrationEmailConfirmation.html"
  loader() { 
#line 41 "emltempls/RegistrationEmailConfirmation.html"
   my_generator.name("skdefemailtemplates");
#line 41 "emltempls/RegistrationEmailConfirmation.html"
   my_generator.add_view<skdefemailtemplates::RegistrationEmailConfirmationView,CRegistration>("RegistrationEmailConfirmationView",true);
#line 41 "emltempls/RegistrationEmailConfirmation.html"
    cppcms::views::pool::instance().add(my_generator);
#line 41 "emltempls/RegistrationEmailConfirmation.html"
 }
#line 41 "emltempls/RegistrationEmailConfirmation.html"
 ~loader() {  cppcms::views::pool::instance().remove(my_generator); }
#line 41 "emltempls/RegistrationEmailConfirmation.html"
} a_loader;
#line 41 "emltempls/RegistrationEmailConfirmation.html"
} // anon 
