#line 1 "home.html"
#include "models.h" 
#line 2 "home.html"
namespace sapredictor {
	#line 3 "home.html"
	struct home :public cppcms::base_view
	#line 3 "home.html"
	{
	#line 3 "home.html"
		homemodel &content;
	#line 3 "home.html"
		home(std::ostream &_s,homemodel &_content): cppcms::base_view(_s),content(_content)
	#line 3 "home.html"
		{
	#line 3 "home.html"
		}
		#line 4 "home.html"
		virtual void render() {
			#line 24 "home.html"
			out()<<"\n"
				"\n"
				"\n"
				"<!DOCTYPE html>\n"
				"<!--\n"
				"To change this license header, choose License Headers in Project Properties.\n"
				"To change this template file, choose Tools | Templates\n"
				"and open the template in the editor.\n"
				"-->\n"
				"<html>\n"
				"    <head>\n"
				"        <title>TODO supply a title</title>\n"
				"        <meta charset=\"UTF-8\">\n"
				"        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
				"    </head>\n"
				"    <body>\n"
				"        <div>TODO write content</div>\n"
				"    </body>\n"
				"</html>\n"
				"\n"
				"";
		#line 24 "home.html"
		} // end of template render
	#line 25 "home.html"
	}; // end of class home
#line 26 "home.html"
} // end of namespace sapredictor
#line 27 "home.html"
namespace {
#line 27 "home.html"
 cppcms::views::generator my_generator; 
#line 27 "home.html"
 struct loader { 
#line 27 "home.html"
  loader() { 
#line 27 "home.html"
   my_generator.name("sapredictor");
#line 27 "home.html"
   my_generator.add_view<sapredictor::home,homemodel>("home",true);
#line 27 "home.html"
    cppcms::views::pool::instance().add(my_generator);
#line 27 "home.html"
 }
#line 27 "home.html"
 ~loader() {  cppcms::views::pool::instance().remove(my_generator); }
#line 27 "home.html"
} a_loader;
#line 27 "home.html"
} // anon 
