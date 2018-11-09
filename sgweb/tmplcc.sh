#!/bin/bash

# compilation instructions for template files

# 1 Public Website
cppcms_tmpl_cc compsearch.xhtml header.xhtml footer.xhtml recentfinds.xhtml relatedlinks.xhtml home.xhtml sitemap.xhtml search.xhtml subscriptions.xhtml fdm_prospects_chart_sector.xhtml fdm_prospects_chart_future.xhtml fdm_cosummary.xhtml fdm_allyears.xhtml fdm_perf.xhtml fdm_prospects.xhtml fdm_news.xhtml companypicks.xhtml companyscreener.xhtml fundamentals.xhtml ecnmynews.xhtml ecnmy.xhtml about.xhtml master.xhtml error.xhtml AdminAccountMapView.xhtml PredictionSetupView.xhtml AdminHomeView.xhtml manageecoindicators.xhtml AdminView.xhtml register.xhtml ppapproved.xhtml ppcancel.xhtml ppfinal.xhtml signin.xhtml signout.xhtml myaccount.xhtml portfoliowiz.xhtml maintain.xhtml -o SGSkin.cpp

# 2 Internal Website
#cppcms_tmpl_cc AdminView.xhtml -s sgadmskin -o SGAdmSkin.cpp
