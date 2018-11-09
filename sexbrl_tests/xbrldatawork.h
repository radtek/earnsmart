/* 
 * File:   xbrldatawork.h
 * Author: santony
 *
 * Created on January 16, 2014, 12:20 PM
 */

#ifndef XBRLDATAWORK_H
#define	XBRLDATAWORK_H

#include <initializer_list>
#include <string>

using namespace std;

void CopyInstanceSegmentData();

void RepopulateListedCompanies(std::initializer_list<std::string> files);

void PurgeUnlistedCompanies();

void UploadSecurityWebLinks(const string& file);

#endif	/* XBRLDATAWORK_H */

