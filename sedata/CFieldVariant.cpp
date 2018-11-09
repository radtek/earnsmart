/* 
 * File:   CFieldVariantVisitor.cpp
 * Author: santony
 * 
 * Created on December 5, 2012, 8:43 AM
 */

#include "CFieldVariant.h"

using namespace se::data;

CFieldVariantVisitor::CFieldVariantVisitor() {
}

CFieldVariantVisitor::CFieldVariantVisitor(const CFieldVariantVisitor& orig) {
}

CFieldVariantVisitor::~CFieldVariantVisitor() {
}

int CFieldVariantVisitor::operator ++(int i)
{
    return ++i;
}
