#!/usr/bin/perl -w

########################################################################
# ssqls.pl - Generates ssqls.h, as it defines many near-duplicate
#	functions and classes, varying only in trivial ways.
#
# Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
# (c) 2004-2010 by Educational Technology Resources, Inc.  Others may
# also hold copyrights on code in this file.  See the CREDITS.txt file
# in the top directory of the distribution for details.
#
# This file is part of MySQL++.
#
# MySQL++ is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# MySQL++ is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
# License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with MySQL++; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
# USA
########################################################################


# This is the limit on the number of SSQLS data members.  Higher values
# will make ssqls.h exponentially larger.  This will increase compile
# times and may even expose limits in your compiler.  Increase it only
# if and as far as you must.
my $max_data_members = 25;

# To make comparisons between floating point values, we subtract them,
# take the absolute value, and test to see if that delta is under this
# value.  If it is, we call the two values "equal".  Change this as fits
# your need for precision.  Note that we express it as a string because
# we want the value copied literally into ssqls.h, not "preprocessed" 
# by Perl as a floating-point value.
my $fp_min_delta = "0.00001";


# No user-serviceable parts below.

use strict;
use Getopt::Std;

our $opt_f;
getopts('f:') or die "usage: $0 [-f fields]\n\n";
$max_data_members = int($opt_f) if defined $opt_f;

open (OUT, ">ssqls.h");

print OUT << "---";

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// This file is generated by the Perl script ssqls.pl.  Do not modify
// it directly.  Change the script instead.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#if !defined(MYSQLPP_SSQLS_H)
#define MYSQLPP_SSQLS_H

#include "noexceptions.h"
#include "sql_types.h"

#if !defined(MYSQLPP_SSQLS_COMPATIBLE)
#	error Your compiler is not compatible with the SSQLS feature!
#endif

#include <string>

#include <math.h>

// Smallest difference between two floating point numbers recognized
// in making comparisons.  If the absolute delta is under this
// threshold, the two values are considered equal.  You can either
// override this permanently by changing ssqls.pl, or you can do it
// on a case-by-case basis at compile time by defining this to another
// value before #including this header.
#if !defined(MYSQLPP_FP_MIN_DELTA)
#	define MYSQLPP_FP_MIN_DELTA $fp_min_delta
#endif

namespace mysqlpp {

enum sql_dummy_type { sql_dummy };

#ifdef MYSQLPP_SSQLS_NO_STATICS
#	define MYSQLPP_SSQLS_CONDITIONAL_STATICS(...)
#else
#	define MYSQLPP_SSQLS_CONDITIONAL_STATICS(...) __VA_ARGS__
#endif

#define MYSQLPP_SSQLS_MAX_MEMBERS $max_data_members

---

my @types = ("Date", "DateTime", "Time", "String", "std::string");
foreach my $type (@types) {
	print OUT << "---";

inline int sql_cmp(const $type& a, const $type& b)
{
	return a.compare(b);
}
---
}

@types = (
		"signed char", "unsigned char",
		"sql_tinyint", "sql_tinyint_unsigned",
		"signed int", "unsigned",
		"signed short", "unsigned short",
		"signed long", "unsigned long");
foreach my $type (@types) {
	print OUT << "---";

inline int sql_cmp($type a, $type b)
{
	return a - b;
}
---
}

@types = ("longlong", "ulonglong");
foreach my $type (@types) {
	print OUT << "---";

inline int sql_cmp($type a, $type b) 
{
	if (a == b) return 0;
	if (a <  b) return -1;
	return 1;
}
---
}	

@types = ("double", "float");
foreach my $type (@types) {
	print OUT << "---";

inline int sql_cmp($type a, $type b) 
{
	if (fabs(a - b) < MYSQLPP_FP_MIN_DELTA) return 0;
	if (a <  b) return -1;
	return 1;
}
---
}	

print OUT << "---";

template <typename T>
inline int sql_cmp(const mysqlpp::Null<T>& a, const mysqlpp::Null<T>& b) 
{
	if (a == b) return 0;
	if (a <  b) return -1;
	return 1;
}


// ---------------------------------------------------
//                Begin Mandatory Compare 
// ---------------------------------------------------

#define sql_compare_define(NAME) \\
	bool operator == (const NAME &other) const \\
		{return sql_compare_##NAME<mysqlpp::sql_dummy>(*this,other) == 0;} \\
	bool operator != (const NAME &other) const \\
		{return sql_compare_##NAME<mysqlpp::sql_dummy>(*this,other) != 0;} \\
	bool operator > (const NAME &other) const \\
		{return sql_compare_##NAME<mysqlpp::sql_dummy>(*this,other) >  0;} \\
	bool operator < (const NAME &other) const \\
		{return sql_compare_##NAME<mysqlpp::sql_dummy>(*this,other) <  0;} \\
	bool operator >= (const NAME &other) const \\
		{return sql_compare_##NAME<mysqlpp::sql_dummy>(*this,other) >= 0;} \\
	bool operator <= (const NAME &other) const \\
		{return sql_compare_##NAME<mysqlpp::sql_dummy>(*this,other) <= 0;} \\
	int cmp (const NAME &other) const \\
		{return sql_compare_##NAME<mysqlpp::sql_dummy>(*this,other);} \\
	int compare (const NAME &other) const \\
		{return sql_compare_##NAME<mysqlpp::sql_dummy>(*this,other);}
---

my ($parm0, $parm1);

foreach my $j (1..$max_data_members) {
	$parm0 .= "T$j, C$j";
	$parm0 .= ", " unless $j == $max_data_members;
	$parm1 .= "C$j";
	$parm1 .= ", " unless $j == $max_data_members;
}

print OUT << "---";

#define sql_compare_define_0(NAME, $parm0)

#define sql_construct_define_0(NAME, $parm0)

#define sql_COMPARE__0(NAME, $parm1)

#define sql_compare_type_def_0(NAME, WHAT, NUM) \\
	sql_compare_type_def_##NUM(NAME, WHAT, NUM)

#define sql_compare_type_defe_0(NAME, WHAT, NUM) \\
	sql_compare_type_defe_##NUM(NAME, WHAT, NUM)

// ---------------------------------------------------
//                 End Mandatory Compare 
// ---------------------------------------------------
---


foreach my $i (1..$max_data_members) {
	my ($compr, $define, $compp, $set, $parm2);

	$compr = ""; $parm2 = ""; $define = "";
	$compr = "    int cmp; \\\n" unless $i == 1;
	$compp = "";
	$set = "";

	foreach my $j (1..$i) {
		if ($j != $i) {
			$compr .= "    cmp = mysqlpp::sql_cmp(x.C$j , y.C$j ); \\\n";
			$compr .= "    if (cmp) return cmp; \\\n";
		}

		$compr .= "    return mysqlpp::sql_cmp(x.C$j , y.C$j );" if $j == $i;
		$parm2 .= "const T$j &p$j";
		$parm2 .= ", " unless $j == $i;
		$define.= "C$j (p$j)";
		$define.= ", " unless $j == $i;
		$set   .= "    C$j = p$j;\\\n";
		$compp .= "true";
		$compp .= ", " unless $j == $i;
	}
	print OUT << "---";

// ---------------------------------------------------
//                   Begin Compare $i
// ---------------------------------------------------

#define sql_compare_define_$i(NAME, $parm0) \\
	NAME($parm2) : $define, table_override_(0) {} \\
	void set($parm2) { \\
	table_override_ = 0; \\
$set \\
	} \\
	sql_compare_define(NAME)

#define sql_construct_define_$i(NAME, $parm0) \\
	void set($parm2) { \\
	table_override_ = 0; \\
$set \\
	} \\
	NAME($parm2) : $define, table_override_(0) {}

#define sql_compare_type_def_$i(NAME, WHAT, NUM) \\
	return WHAT##_list(d, m, $compp)

#define sql_compare_type_defe_$i(NAME, WHAT, NUM) \\
	return WHAT##_list(d, c, m, $compp)

#define sql_COMPARE__$i(NAME, $parm1) \\
	template <mysqlpp::sql_dummy_type dummy> \\
	int sql_compare_##NAME(const NAME &x, const NAME &y) { \\
$compr \\
	} \\
	template <mysqlpp::sql_dummy_type dummy> \\
	int compare (const NAME &x, const NAME &y) { \\
$compr \\
	}

// ---------------------------------------------------
//                   End Compare $i
// ---------------------------------------------------

---
}


foreach my $i (1..$max_data_members) {
	my $create_bool = "";
	my $create_list = "";
	my $cus_equal_list = "";
	my $cus_field_list = "";
	my $cusparms1 = "";
	my $cusparms11 = "";
	my $cusparms2 = "";
	my $cusparms22 = "";
	my $cusparmsv = "";
	my $defs = "";
	my $enums = "";
	my $equal_list = "";
	my $field_list = "";
	my $names = "";
	my $parmc = "";
	my $parmC = "";
	my $parm_complete = "";
	my $parm_simple = "";
	my $parm_simple2c = "";
	my $parm_simple2c_b = "";
	my $parm_simple_b = "";
	my $popul = "";
	my $value_list = "";
	my $value_list_cus = "";

	foreach my $j (1 .. $i) {
		$parm_complete .= "T$j, I$j, N$j";
		$parm_complete .= ", " unless $j == $i;
		$parm_simple   .= "T$j, I$j";
		$parm_simple   .= ", " unless $j == $i;
		$parm_simple2c .= "T$j, I$j, #I$j";
		$parm_simple2c .= ", " unless $j == $i;
		$parm_simple_b   .= "T$j, I$j";
		$parm_simple_b   .= ", " unless $j == $i;
		$parm_simple2c_b .= "T$j, I$j";
		$parm_simple2c_b .= ", " unless $j == $i;

		$defs  .= "    T$j I$j;";
		$defs  .= "\n" unless $j == $i;

		$popul .= "    s->I$j = row[N$j].conv(T$j());";
		$popul .= "\n" unless $j == $i;

		$names .= "    N$j ";
		$names .= ",\n" unless $j == $i;
		$enums .= "    NAME##_##I$j";
		$enums .= ",\n" unless $j == $i;

		$field_list .= "    s << obj.manip << '`' << obj.obj->names[".($j-1)."] << '`'";
		$field_list .= " << obj.delim;\n" unless $j == $i;

		$value_list .= "    s << obj.manip << obj.obj->I$j";
		$value_list .= " << obj.delim;\n" unless $j == $i;

		$create_bool .= "    if (i$j) (*include)[".($j-1)."]=true;\n";

		$create_list .= "    if (i$j == NAME##_NULL) return;\n" unless $i == 1;
		$create_list .= "    (*include)[i$j]=true;\n";

		$value_list_cus .= "    if ((*obj.include)[".($j-1)."]) { \n";
		$value_list_cus .= "      if (before) s << obj.delim;\n" unless $j == 1;
		$value_list_cus .= "      s << obj.manip << obj.obj->I$j;\n";
		$value_list_cus .= "      before = true; \n" unless $j == $i;
		$value_list_cus .= "     } \n";

		$cus_field_list .= "    if ((*obj.include)[".($j-1)."]) { \n";
		$cus_field_list .= "      if (before) s << obj.delim;\n" unless $j == 1;
		$cus_field_list .= "      s << obj.manip << '`' << obj.obj->names[".($j-1)."] << '`';\n";
		$cus_field_list .= "      before = true; \n" unless $j == $i;
		$cus_field_list .= "     } \n";

		$cus_equal_list .= "    if ((*obj.include)[".($j-1)."]) { \n";
		$cus_equal_list .= "      if (before) s << obj.delim;\n" unless $j == 1;
		$cus_equal_list .= "      s << '`' << obj.obj->names[".($j-1)."] << '`' << obj.comp";
		$cus_equal_list .=        " << obj.manip << obj.obj->I$j;\n";
		$cus_equal_list .= "      before = true; \n" unless $j == $i;
		$cus_equal_list .= "     } \n";

		$equal_list .= "    s << '`' << obj.obj->names[".($j-1)."] << '`' << obj.comp";
		$equal_list .= " << obj.manip << obj.obj->I$j";
		$equal_list .= " << obj.delim;\n" unless $j == $i;

		$cusparms1  .= "bool i$j"         if     $j == 1;
		$cusparms1  .= "bool i$j = false" unless $j == 1;
		$cusparms1  .= ", " unless $j == $i;
		$cusparms11  .= "bool i$j" ;
		$cusparms11  .= ", " unless $j == $i;
		$cusparms2  .= "NAME##_enum i$j" if $j == 1;
		$cusparms2  .= "NAME##_enum i$j = NAME##_NULL" unless $j == 1;
		$cusparms2  .= ", " unless $j == $i;
		$cusparms22  .= "NAME##_enum i$j";
		$cusparms22  .= ", " unless $j == $i;
		$cusparmsv  .= "i$j";
		$cusparmsv  .= ", " unless $j == $i;

		$parmC .= "T$j, I$j";
		$parmC .= ", " unless $j == $max_data_members;
		$parmc .= "I$j";
		$parmc .= ", " unless $j == $max_data_members;
	}

	foreach my $j ($i + 1 .. $max_data_members) {
		$parmC .= "0, 0";
		$parmC .= ", " unless $j == $max_data_members;
		$parmc .= "0";
		$parmc .= ", " unless $j == $max_data_members;
	}

	print OUT << "---";
// ---------------------------------------------------
//                  Begin Create $i
// ---------------------------------------------------
---
	my $out = <<"---";
#define sql_create_complete_$i(NAME, CMP, CONTR, $parm_complete) 
	struct NAME; 

	enum NAME##_enum { 
$enums 
	,NAME##_NULL 
	}; 

	template <class Manip>
	class NAME##_value_list { 
	public: 
	const NAME* obj;
	const char* delim;
	Manip manip;
	public: 
	NAME##_value_list (const NAME* o, const char* d, Manip m) :
	obj(o), delim(d), manip(m) { } 
	};

	template <class Manip>
	class NAME##_##field_list {
	public: 
	const NAME* obj; 
	const char* delim;
	Manip manip;
	public: 
	NAME##_field_list (const NAME* o, const char* d, Manip m) :
	obj(o), delim(d), manip(m) { } 
	};

	template <class Manip>
	class NAME##_equal_list { 
	public: 
	const NAME* obj;
	const char* delim;
	const char* comp;
	Manip manip;
	public: 
	NAME##_equal_list (const NAME* o, const char* d, const char* c, Manip m) :
	obj(o), delim(d), comp(c), manip(m) { }
	};

	template <class Manip>
	class NAME##_cus_value_list {
	public:
	const NAME* obj;
	std::vector<bool> *include;
	bool del_vector;
	const char* delim;
	Manip manip;
	public: 
	~NAME##_cus_value_list () {if (del_vector) delete include;} 
	NAME##_cus_value_list (const NAME* o, const char* d, Manip m, $cusparms11);
	NAME##_cus_value_list (const NAME* o, const char* d, Manip m, $cusparms22); 
	NAME##_cus_value_list (const NAME* o, const char* d, Manip m ,std::vector<bool>* i) :
	obj(o), include(i), del_vector(false), delim(d), manip(m) { }
	};

	template <class Manip>
	class NAME##_cus_field_list { 
	public:
	const NAME* obj; 
	std::vector<bool> *include; 
	bool del_vector; 
	const char* delim;
	Manip manip;
	public: 
	~NAME##_cus_field_list () {if (del_vector) delete include;} 
	NAME##_cus_field_list (const NAME* o, const char* d, Manip m, $cusparms11); 
	NAME##_cus_field_list (const NAME* o, const char* d, Manip m, $cusparms22); 
	NAME##_cus_field_list (const NAME* o, const char* d, Manip m, std::vector<bool> *i) :
	obj(o), include(i), del_vector(false), delim(d), manip(m) { }
	};

	template <class Manip>
	class NAME##_cus_equal_list {
	public:
	const NAME* obj;
	std::vector<bool> *include;
	bool del_vector;
	const char* delim;
	const char* comp;
	Manip manip;
	public:
	~NAME##_##cus_equal_list () {if (del_vector) delete include;}
	NAME##_##cus_equal_list (const NAME* o, const char* d, const char* c, Manip m, $cusparms11); 
	NAME##_##cus_equal_list (const NAME* o, const char* d, const char* c, Manip m, $cusparms22); 
	NAME##_##cus_equal_list (const NAME* o, const char* d, const char* c, Manip m, std::vector<bool> *i) :
	obj(o), include(i), del_vector(false), delim(d), comp(c), manip(m) { }
	};

	template <mysqlpp::sql_dummy_type dummy> int sql_compare_##NAME(const NAME&, const NAME&);

	struct NAME {
$defs 
	NAME() : table_override_(0) { }
	NAME(const mysqlpp::Row& row);
	void set(const mysqlpp::Row &row);
	sql_compare_define_##CMP(NAME, $parmC)
	sql_construct_define_##CONTR(NAME, $parmC)
	static const char* names[];
	static void table(const char* t) { table_ = t; }
	const char* table() const
			{ return table_override_ ? table_override_ : NAME::table_; }
	void instance_table(const char* t) { table_override_ = t; }

	NAME##_value_list<mysqlpp::quote_type0> value_list() const {
		return value_list(",", mysqlpp::quote);}
	NAME##_value_list<mysqlpp::quote_type0> value_list(const char* d) const {
		return value_list(d, mysqlpp::quote);}
	template <class Manip>
	NAME##_value_list<Manip> value_list(const char* d, Manip m) const;

	NAME##_field_list<mysqlpp::do_nothing_type0> field_list() const {
		return field_list(",", mysqlpp::do_nothing);}
	NAME##_field_list<mysqlpp::do_nothing_type0> field_list(const char* d) const {
		return field_list(d, mysqlpp::do_nothing);}
	template <class Manip>
	NAME##_field_list<Manip> field_list(const char* d, Manip m) const;

	NAME##_equal_list<mysqlpp::quote_type0> equal_list(const char* d = ",",
			const char* c = " = ") const
			{ return equal_list(d, c, mysqlpp::quote); }
	template <class Manip>
	NAME##_equal_list<Manip> equal_list(const char* d, const char* c, Manip m) const;

	/* cus_data */

	NAME##_cus_value_list<mysqlpp::quote_type0> value_list($cusparms1) const
			{ return value_list(",", mysqlpp::quote, $cusparmsv); }
	NAME##_cus_value_list<mysqlpp::quote_type0> value_list($cusparms2) const
			{ return value_list(",", mysqlpp::quote, $cusparmsv); }
	NAME##_cus_value_list<mysqlpp::quote_type0> value_list(std::vector<bool> *i) const
			{ return value_list(",", mysqlpp::quote, i); }
	NAME##_cus_value_list<mysqlpp::quote_type0> value_list(mysqlpp::sql_cmp_type sc) const
			{ return value_list(",", mysqlpp::quote, sc); }

	NAME##_cus_value_list<mysqlpp::quote_type0> value_list(const char* d, $cusparms1) const
			{ return value_list(d, mysqlpp::quote, $cusparmsv); }
	NAME##_cus_value_list<mysqlpp::quote_type0> value_list(const char* d, $cusparms2) const
			{ return value_list(d, mysqlpp::quote, $cusparmsv); }
	NAME##_cus_value_list<mysqlpp::quote_type0> value_list(const char* d,
			std::vector<bool> *i) const
			{ return value_list(d, mysqlpp::quote, i); }
	NAME##_cus_value_list<mysqlpp::quote_type0> value_list(const char* d,
			mysqlpp::sql_cmp_type sc) const
			{ return value_list(d, mysqlpp::quote, sc); }

	template <class Manip>
	NAME##_cus_value_list<Manip> value_list(const char* d, Manip m,
			$cusparms1) const;
	template <class Manip>
	NAME##_cus_value_list<Manip> value_list(const char* d, Manip m,
			$cusparms2) const;
	template <class Manip>
	NAME##_cus_value_list<Manip> value_list(const char* d, Manip m,
			std::vector<bool>* i) const;
	template <class Manip>
	NAME##_cus_value_list<Manip> value_list(const char* d, Manip m, 
			mysqlpp::sql_cmp_type sc) const;
	/* cus field */

	NAME##_cus_field_list<mysqlpp::do_nothing_type0> field_list($cusparms1) const 
			{ return field_list(",", mysqlpp::do_nothing, $cusparmsv); }
	NAME##_cus_field_list<mysqlpp::do_nothing_type0> field_list($cusparms2) const
			{ return field_list(",", mysqlpp::do_nothing, $cusparmsv); }
	NAME##_cus_field_list<mysqlpp::do_nothing_type0> field_list(std::vector<bool> *i) const
			{ return field_list(",", mysqlpp::do_nothing, i); }
	NAME##_cus_field_list<mysqlpp::do_nothing_type0> field_list(mysqlpp::sql_cmp_type sc) const
			{ return field_list(",", mysqlpp::do_nothing, sc); }

	NAME##_cus_field_list<mysqlpp::do_nothing_type0> field_list(const char* d,
			$cusparms1) const
			{ return field_list(d, mysqlpp::do_nothing, $cusparmsv); }
	NAME##_cus_field_list<mysqlpp::do_nothing_type0> field_list(const char* d,
			$cusparms2) const
			{ return field_list(d, mysqlpp::do_nothing, $cusparmsv); }
	NAME##_cus_field_list<mysqlpp::do_nothing_type0> field_list(const char* d,
			std::vector<bool>* i) const
			{ return field_list(d, mysqlpp::do_nothing, i); }
	NAME##_cus_field_list<mysqlpp::do_nothing_type0> field_list(const char* d,
			mysqlpp::sql_cmp_type sc) const
			{ return field_list(d, mysqlpp::do_nothing, sc); }

	template <class Manip>
	NAME##_cus_field_list<Manip> field_list(const char* d, Manip m,
			$cusparms1) const;
	template <class Manip>
	NAME##_cus_field_list<Manip> field_list(const char* d, Manip m,
			$cusparms2) const;
	template <class Manip>
	NAME##_cus_field_list<Manip> field_list(const char* d, Manip m,
			std::vector<bool> *i) const;
	template <class Manip>
	NAME##_cus_field_list<Manip> field_list(const char* d, Manip m,
			mysqlpp::sql_cmp_type sc) const;

	/* cus equal */

	NAME##_cus_equal_list<mysqlpp::quote_type0> equal_list($cusparms1) const
			{ return equal_list(",", " = ", mysqlpp::quote, $cusparmsv); }
	NAME##_cus_equal_list<mysqlpp::quote_type0> equal_list($cusparms2) const
			{ return equal_list(",", " = ", mysqlpp::quote, $cusparmsv); }
	NAME##_cus_equal_list<mysqlpp::quote_type0> equal_list(std::vector<bool>* i) const
			{ return equal_list(",", " = ", mysqlpp::quote, i); }
	NAME##_cus_equal_list<mysqlpp::quote_type0> equal_list(mysqlpp::sql_cmp_type sc) const
			{ return equal_list(",", " = ", mysqlpp::quote, sc); }

	NAME##_cus_equal_list<mysqlpp::quote_type0> equal_list(const char* d, $cusparms1) const
			{ return equal_list(d, " = ", mysqlpp::quote, $cusparmsv); }
	NAME##_cus_equal_list<mysqlpp::quote_type0> equal_list(const char* d, $cusparms2) const
			{ return equal_list(d, " = ", mysqlpp::quote, $cusparmsv); }
	NAME##_cus_equal_list<mysqlpp::quote_type0> equal_list(const char* d,
			std::vector<bool> *i) const
			{ return equal_list(d, " = ", mysqlpp::quote, i); }
	NAME##_cus_equal_list<mysqlpp::quote_type0> equal_list(const char* d,
			mysqlpp::sql_cmp_type sc) const
			{ return equal_list(d, " = ", mysqlpp::quote, sc); }

	NAME##_cus_equal_list<mysqlpp::quote_type0> equal_list(const char* d, const char* c,
			$cusparms1) const
			{ return equal_list(d, c, mysqlpp::quote, $cusparmsv); }
	NAME##_cus_equal_list<mysqlpp::quote_type0> equal_list(const char* d, const char* c,
			$cusparms2) const
			{ return equal_list(d, c, mysqlpp::quote, $cusparmsv); }
	NAME##_cus_equal_list<mysqlpp::quote_type0> equal_list(const char* d, const char* c,
			std::vector<bool> *i) const
			{ return equal_list(d, c, mysqlpp::quote, i); }
	NAME##_cus_equal_list<mysqlpp::quote_type0> equal_list(const char* d, const char* c,
			mysqlpp::sql_cmp_type sc) const
			{ return equal_list(d, c, mysqlpp::quote, sc); }

	template <class Manip>
	NAME##_cus_equal_list<Manip> equal_list(const char* d, const char* c, Manip m, 
						$cusparms1) const;
	template <class Manip>
	NAME##_cus_equal_list<Manip> equal_list(const char* d, const char* c, Manip m, 
						$cusparms2) const;
	template <class Manip>
	NAME##_cus_equal_list<Manip> equal_list(const char* d, const char* c, Manip m, 
						std::vector<bool> *i) const;
	template <class Manip>
	NAME##_cus_equal_list<Manip> equal_list(const char* d, const char* c, Manip m, 
						mysqlpp::sql_cmp_type sc) const;

	private:
	static const char* table_;
	const char* table_override_;
	};
	MYSQLPP_SSQLS_CONDITIONAL_STATICS(
		const char* NAME::names[] = {
			$names
		};
		const char* NAME::table_ = #NAME;
	)

	template <class Manip>
	NAME##_cus_value_list<Manip>::NAME##_cus_value_list
			(const NAME* o, const char* d, Manip m, $cusparms11)
	{
		delim = d;
		manip = m;
		del_vector = true;
		obj = o;
		include = new std::vector<bool>($i, false);
$create_bool
	}

	template <class Manip>
	NAME##_cus_value_list<Manip>::NAME##_cus_value_list
			(const NAME* o, const char* d, Manip m, $cusparms22)
	{
		delim = d;
		manip = m;
		del_vector = true;
		obj = o;
		include = new std::vector<bool>($i, false);
$create_list
	}

	template <class Manip>
	NAME##_cus_field_list<Manip>::NAME##_cus_field_list
			(const NAME* o, const char* d, Manip m, $cusparms11)
	{
		delim = d;
		manip = m;
		del_vector = true;
		obj = o;
		include = new std::vector<bool>($i, false);
$create_bool
	}

	template <class Manip>
	NAME##_cus_field_list<Manip>::NAME##_cus_field_list
			(const NAME* o, const char* d, Manip m, $cusparms22)
	{
		delim = d;
		manip = m;
		del_vector = true;
		obj = o;
		include = new std::vector<bool>($i, false);
$create_list
	}

	template <class Manip>
	NAME##_cus_equal_list<Manip>::NAME##_cus_equal_list
			(const NAME* o, const char* d, const char* c, Manip m, $cusparms11)
	{
		delim = d;
		comp = c;
		manip = m;
		del_vector = true;
		obj = o;
		include = new std::vector<bool>($i, false);
$create_bool
	}

	template <class Manip>
	NAME##_cus_equal_list<Manip>::NAME##_cus_equal_list
			(const NAME* o, const char* d, const char* c, Manip m, $cusparms22)
	{
		delim = d;
		comp = c;
		manip = m;
		del_vector = true;
		obj = o;
		include = new std::vector<bool>($i, false);
$create_list
	}

	template <class Manip>
	std::ostream& operator <<(std::ostream& s, const NAME##_value_list<Manip>& obj)
	{
$value_list;
		return s;
	}

	template <class Manip>
	std::ostream& operator <<(std::ostream& s, const NAME##_field_list<Manip>& obj)
	{
$field_list;
		return s;
	}

	template <class Manip>
	std::ostream& operator <<(std::ostream& s, const NAME##_equal_list<Manip>& obj)
	{
$equal_list;
		return s;
	}

	template <class Manip>
	std::ostream& operator <<(std::ostream& s, const NAME##_cus_value_list<Manip>& obj)
	{
		bool before = false;
$value_list_cus
		return s;
	}

	template <class Manip>
	std::ostream& operator <<(std::ostream& s, const NAME##_cus_field_list<Manip>& obj)
	{
		bool before = false;
$cus_field_list
		return s;
	}

	template <class Manip>
	std::ostream& operator <<(std::ostream& s, const NAME##_cus_equal_list<Manip>& obj)
	{
		bool before = false;
$cus_equal_list
		return s;
	}

	template <class Manip>
	inline NAME##_value_list<Manip> NAME::value_list(const char* d, Manip m) const
			{ return NAME##_value_list<Manip> (this, d, m); } 

	template <class Manip>
	inline NAME##_field_list<Manip> NAME::field_list(const char* d, Manip m) const
			{ return NAME##_field_list<Manip> (this, d, m); } 

	template <class Manip>
	inline NAME##_equal_list<Manip> NAME::equal_list(const char* d, const char* c, Manip m) const
			{ return NAME##_equal_list<Manip> (this, d, c, m); } 

	template <class Manip>
	inline NAME##_cus_value_list<Manip> NAME::value_list(const char* d, Manip m,
			$cusparms11) const
			{ return NAME##_cus_value_list<Manip> (this, d, m, $cusparmsv); }

	template <class Manip>
	inline NAME##_cus_field_list<Manip> NAME::field_list(const char* d, Manip m,
			$cusparms11) const
			{ return NAME##_cus_field_list<Manip> (this, d, m, $cusparmsv); }

	template <class Manip>
	inline NAME##_cus_equal_list<Manip> NAME::equal_list(const char* d, const char* c, Manip m,
			$cusparms11) const
			{ return NAME##_cus_equal_list<Manip> (this, d, c, m, $cusparmsv); } 

	template <class Manip>
	inline NAME##_cus_value_list<Manip> NAME::value_list(const char* d, Manip m,
			$cusparms22) const
			{ return NAME##_cus_value_list<Manip> (this, d, m, $cusparmsv); } 

	template <class Manip>
	inline NAME##_cus_field_list<Manip> NAME::field_list(const char* d, Manip m,
			$cusparms22) const
			{ return NAME##_cus_field_list<Manip> (this, d, m, $cusparmsv); } 

	template <class Manip>
	inline NAME##_cus_equal_list<Manip> NAME::equal_list(const char* d, const char* c, Manip m, 
			$cusparms22) const
			{ return NAME##_cus_equal_list<Manip> (this, d, c, m, $cusparmsv); } 

	template <class Manip>
	inline NAME##_cus_value_list<Manip> NAME::value_list(const char* d, Manip m,
			std::vector<bool> *i) const
			{ return NAME##_cus_value_list<Manip> (this, d, m, i); }

	template <class Manip>
	inline NAME##_cus_field_list<Manip> NAME::field_list(const char* d, Manip m,
			std::vector<bool> *i) const
			{ return NAME##_cus_field_list<Manip> (this, d, m, i); }

	template <class Manip>
	inline NAME##_cus_equal_list<Manip> NAME::equal_list(const char* d, const char* c, Manip m,
			std::vector<bool> *i) const
			{ return NAME##_cus_equal_list<Manip> (this, d, c, m, i); }

	template <class Manip>
	inline NAME##_cus_value_list<Manip> 
	NAME::value_list(const char* d, Manip m, mysqlpp::sql_cmp_type /*sc*/) const
			{ sql_compare_type_def_##CMP(NAME, value, NUM); }

	template <class Manip>
	inline NAME##_cus_field_list<Manip> 
	NAME::field_list(const char* d, Manip m, mysqlpp::sql_cmp_type /*sc*/) const
			{ sql_compare_type_def_##CMP(NAME, field, NUM); }

	template <class Manip>
	inline NAME##_cus_equal_list<Manip> 
	NAME::equal_list(const char* d, const char* c, Manip m, mysqlpp::sql_cmp_type /*sc*/) const
			{ sql_compare_type_defe_##CMP(NAME, equal, NUM); }

	template <mysqlpp::sql_dummy_type dummy>
	void populate_##NAME(NAME *s, const mysqlpp::Row &row)
	{
		mysqlpp::NoExceptions ignore_schema_mismatches(row);
$popul
	}

	inline NAME::NAME(const mysqlpp::Row& row) :
	table_override_(0)
			{ populate_##NAME<mysqlpp::sql_dummy>(this, row); }
	inline void NAME::set(const mysqlpp::Row& row)
	{
		table_override_ = 0;
		populate_##NAME<mysqlpp::sql_dummy>(this, row);
	}

	sql_COMPARE__##CMP(NAME, $parmc )

---
print OUT &prepare($out);

#
# short cut defs
#

print OUT << "---";
#define sql_create_$i(NAME, CMP, CONTR, $parm_simple) \\
		sql_create_complete_$i(NAME, CMP, CONTR, $parm_simple2c) \\

// ---------------------------------------------------
//                  End Create $i
// ---------------------------------------------------

---

}


print OUT << "---";

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_SSQLS_H)

---


sub prepare {
	local $_ = $_[0];
	s/\n+$//;
	s/\n[\n ]*\n/\n/g; 
	s/\n+/\\\n/g;
	$_ .= "\n\n";
	return $_;
}

