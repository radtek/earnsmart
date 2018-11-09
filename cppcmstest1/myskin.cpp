#line 1 "master.tmpl"
#include "content.h" 
#line 2 "master.tmpl"
namespace myskin {
	#line 3 "master.tmpl"
	struct master :public cppcms::base_view
	#line 3 "master.tmpl"
	{
	#line 3 "master.tmpl"
		se::model::master &content;
	#line 3 "master.tmpl"
		master(std::ostream &_s,se::model::master &_content): cppcms::base_view(_s),content(_content)
	#line 3 "master.tmpl"
		{
	#line 3 "master.tmpl"
		}
		#line 4 "master.tmpl"
		virtual void title() {
			#line 5 "master.tmpl"
			out()<<"\n"
				"    ";
			#line 5 "master.tmpl"
			out()<<cppcms::filters::escape(content.title);
			#line 6 "master.tmpl"
			out()<<"\n"
				"";
		#line 6 "master.tmpl"
		} // end of template title
		#line 7 "master.tmpl"
		virtual void page_content() {
			#line 7 "master.tmpl"
			out()<<" Override me ";
		#line 7 "master.tmpl"
		} // end of template page_content
		#line 8 "master.tmpl"
		virtual void render() {
			#line 11 "master.tmpl"
			out()<<"\n"
				"<html>\n"
				"    <head>\n"
				"        <title> ";
			#line 11 "master.tmpl"
			title();
			#line 19 "master.tmpl"
			out()<<" </title>\n"
				"        <link href=\"/hello/style\" rel=\"stylesheet\" type=\"text/css\" media=\"screen\"/>\n"
				"        <script src=\"http://code.jquery.com/jquery-latest.min.js\"></script>\n"
				"        <script language=\"javascript\" src=\"/hello/script\"></script>\n"
				"\t<link rel=\"shortcut icon\" href=\"/hello/images/favicon.ico\" />\n"
				"        <link rel=\"icon\" href=\"/hello/images/favicon.ico\" />\n"
				"    </head>\n"
				"    <body>\n"
				"        <h1> ";
			#line 19 "master.tmpl"
			title();
			#line 22 "master.tmpl"
			out()<<" </h1>\n"
				"        <img src=\"/hello/images/favicon.ico\" />\n"
				"        <div id=\"content\">\n"
				"            ";
			#line 22 "master.tmpl"
			page_content();
			#line 26 "master.tmpl"
			out()<<"\n"
				"        </div>\n"
				"    </body>\n"
				"</html>\n"
				"";
		#line 26 "master.tmpl"
		} // end of template render
	#line 27 "master.tmpl"
	}; // end of class master
#line 28 "master.tmpl"
} // end of namespace myskin
#line 1 "page.tmpl"
namespace myskin {
	#line 2 "page.tmpl"
	struct page :public master
	#line 2 "page.tmpl"
	{
	#line 2 "page.tmpl"
		se::model::page &content;
	#line 2 "page.tmpl"
		page(std::ostream &_s,se::model::page &_content): master(_s,_content),content(_content)
	#line 2 "page.tmpl"
		{
	#line 2 "page.tmpl"
		}
		#line 3 "page.tmpl"
		virtual void title() {
			#line 4 "page.tmpl"
			out()<<"\n"
				"";
			#line 4 "page.tmpl"
			master::title();
			#line 4 "page.tmpl"
			out()<<" :: ";
			#line 4 "page.tmpl"
			out()<<cppcms::filters::escape(content.page_title);
			#line 5 "page.tmpl"
			out()<<" \n"
				"";
		#line 5 "page.tmpl"
		} // end of template title
		#line 6 "page.tmpl"
		virtual void page_content() {
			#line 8 "page.tmpl"
			out()<<"\n"
				"<span>\n"
				"";
			#line 8 "page.tmpl"
			out()<<cppcms::filters::raw(content.page_content);
			#line 10 "page.tmpl"
			out()<<"\n"
				"</span><span>\n"
				"";
			#line 10 "page.tmpl"
			out()<<cppcms::filters::escape(content.test.teststr);
			#line 12 "page.tmpl"
			out()<<"\n"
				"</span>\n"
				"";
		#line 12 "page.tmpl"
		} // end of template page_content
	#line 13 "page.tmpl"
	}; // end of class page
#line 14 "page.tmpl"
} // end of namespace myskin
#line 1 "news.tmpl"
namespace myskin {
	#line 2 "news.tmpl"
	struct news :public master
	#line 2 "news.tmpl"
	{
	#line 2 "news.tmpl"
		se::model::news &content;
	#line 2 "news.tmpl"
		news(std::ostream &_s,se::model::news &_content): master(_s,_content),content(_content)
	#line 2 "news.tmpl"
		{
	#line 2 "news.tmpl"
		}
		#line 3 "news.tmpl"
		virtual void title() {
			#line 4 "news.tmpl"
			out()<<"\n"
				"    ";
			#line 4 "news.tmpl"
			master::title();
			#line 5 "news.tmpl"
			out()<<" :: News \n"
				"";
		#line 5 "news.tmpl"
		} // end of template title
		#line 6 "news.tmpl"
		virtual void page_content() {
			#line 7 "news.tmpl"
			out()<<"\n"
				"";
			#line 7 "news.tmpl"
			if((content.news_list).begin()!=(content.news_list).end()) {
				#line 9 "news.tmpl"
				out()<<"\n"
					"    <ul>\n"
					"        ";
				#line 9 "news.tmpl"
				for(CPPCMS_TYPEOF((content.news_list).begin()) message_ptr=(content.news_list).begin(),message_ptr_end=(content.news_list).end();message_ptr!=message_ptr_end;++message_ptr) {
				#line 9 "news.tmpl"
				CPPCMS_TYPEOF(*message_ptr) &message=*message_ptr;
					#line 9 "news.tmpl"
					out()<<" <li> ";
					#line 9 "news.tmpl"
					out()<<cppcms::filters::escape(message);
					#line 9 "news.tmpl"
					out()<<" </li> ";
				#line 9 "news.tmpl"
				} // end of item
				#line 11 "news.tmpl"
				out()<<"\n"
					"    </ul>\n"
					"";
			#line 11 "news.tmpl"
			}
			#line 12 "news.tmpl"
			out()<<"\n"
				"";
		#line 12 "news.tmpl"
		} // end of template page_content
	#line 13 "news.tmpl"
	}; // end of class news
#line 14 "news.tmpl"
} // end of namespace myskin
#line 1 "intro.tmpl"
namespace myskin {
	#line 2 "intro.tmpl"
	struct intro :public master
	#line 2 "intro.tmpl"
	{
	#line 2 "intro.tmpl"
		se::model::master &content;
	#line 2 "intro.tmpl"
		intro(std::ostream &_s,se::model::master &_content): master(_s,_content),content(_content)
	#line 2 "intro.tmpl"
		{
	#line 2 "intro.tmpl"
		}
		#line 3 "intro.tmpl"
		virtual void page_content() {
			#line 4 "intro.tmpl"
			out()<<"\n"
				"<p><a href='";
			#line 4 "intro.tmpl"
			content.app().mapper().map(out(),"/page");
			#line 5 "intro.tmpl"
			out()<<"'>The Page</a></p>\n"
				"<p><a href='";
			#line 5 "intro.tmpl"
			content.app().mapper().map(out(),"/news");
			#line 6 "intro.tmpl"
			out()<<"'>The News</a></p>\n"
				"";
		#line 6 "intro.tmpl"
		} // end of template page_content
	#line 7 "intro.tmpl"
	}; // end of class intro
#line 8 "intro.tmpl"
} // end of namespace myskin
#line 8 "intro.tmpl"
namespace {
#line 8 "intro.tmpl"
 cppcms::views::generator my_generator; 
#line 8 "intro.tmpl"
 struct loader { 
#line 8 "intro.tmpl"
  loader() { 
#line 8 "intro.tmpl"
   my_generator.name("myskin");
#line 8 "intro.tmpl"
   my_generator.add_view<myskin::master,se::model::master>("master",true);
#line 8 "intro.tmpl"
   my_generator.add_view<myskin::page,se::model::page>("page",true);
#line 8 "intro.tmpl"
   my_generator.add_view<myskin::news,se::model::news>("news",true);
#line 8 "intro.tmpl"
   my_generator.add_view<myskin::intro,se::model::master>("intro",true);
#line 8 "intro.tmpl"
    cppcms::views::pool::instance().add(my_generator);
#line 8 "intro.tmpl"
 }
#line 8 "intro.tmpl"
 ~loader() {  cppcms::views::pool::instance().remove(my_generator); }
#line 8 "intro.tmpl"
} a_loader;
#line 8 "intro.tmpl"
} // anon 
