/*

MIT License

Copyright (c) 2016 Guenter Prossliner

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef COBJPVT_GEN_PP_H_
#define COBJPVT_GEN_PP_H_


//////////////////////////////////////////////////////////////////////////
// Methods Generation Helper
//
//	To avoid writing dozens of macros to generate the various functions,
//	this helper does the dirty stuff of constructing the arguments.
//	
//	To use it, generators defines the macro 'COBJPVT_GEN_METHOD_TEMPLATE(GEN_RETURN_STATEMENT, GEN_RETURN_TYPE, GEN_METHODNAME, GEN_ARGS_SIGNATURE, GEN_ARGS_NAME)'
//	which defines how to generate a single given method.
//		* GEN_RETURN_STATEMENT: expands to an empty token if GEN_RETURN_TYPE == void, else it expands to 'return'. This is used in the thunk methods
//		* GEN_METHODNAME: expands to the name of the method
//		* GEN_ARGS_SIGNATURE: expands to a comma-separated list of arguments, including types
//		* GEN_ARGS_NAMES: expands to a comma-separated list of the names only
//		* GEN_ARGS_SEPERATOR: contains a comma, if there are any args. Use to separate arguments if necessary in template
//
//	How to use:
//	To invoke the generator, define the template, call COBJPVT_GEN_METHOD_GENERATOR() and undefine the template
//
//		#define COBJPVT_GEN_METHOD_TEMPLATE(GEN_RETURN_STATEMENT, GEN_RETURN_TYPE, GEN_METHODNAME,  GEN_ARGS_SEPERATOR, GEN_ARGS_SIGNATURE, GEN_ARGS_NAME) void GEN_METHODNAME(cobj_object * self GEN_ARGS_SEPERATOR GEN_ARGS_SIGNATURE);
//		
//		COBJPVT_GEN_METHOD_GENERATOR()
//		#undef COBJPVT_GEN_METHOD_TEMPLATE
//
//	How it works:
//		(0.1) the configuration in cobjpvt-generator-helper.h defines the public entry-macros
//			#define COBJ_INTERFACE_METHOD(GEN_METHOD_NAME, ...)	COBJPVT_GEN_INTERFACE_METHOD(GEN_METHOD_NAME, __VA_ARGS__)
//			#define COBJPVT_GEN_METHOD_GENERATOR() COBJ_INTERFACE_METHODS
//		(0.2) in the source-code (e.g interface.h), the user users this public macros to declare methods, like:
//			#define COBJ_INTERFACE_METHODS	COBJ_INTERFACE_METHOD(foo, int, i) COBJ_INTERFACE_METHOD(bar)
//			the source-code includes the generator, which includes cobjpvt-generator-helper.h and cobjpvt-generator-framework.h *after* this definitions
//		(1.2) the generator-source defines a template for generation:
//			#define COBJPVT_GEN_METHOD_TEMPLATE(GEN_RETURN_STATEMENT, GEN_RETURN_TYPE, GEN_METHODNAME,  GEN_ARGS_SEPERATOR, GEN_ARGS_SIGNATURE, GEN_ARGS_NAME)
//				void GEN_METHODNAME(cobj_object * self GEN_ARGS_SEPERATOR GEN_ARGS_SIGNATURE);
//		(1.2) the generator-source classe the generator:
//			COBJPVT_GEN_METHOD_GENERATOR()
//		(2.1) the magic begins....
//		(2.2) COBJPVT_GEN_METHOD_GENERATOR()	=> COBJ_INTERFACE_METHODS // (0.1)
//		(2.3) COBJ_INTERFACE_METHODS		=> COBJ_INTERFACE_METHOD(foo, int, i) COBJ_INTERFACE_METHOD(bar)	// (0.2)
//		(2.4) COBJ_INTERFACE_METHOD(foo, int, i) COBJ_INTERFACE_METHOD(bar) 
//			=> COBJPVT_GEN_INTERFACE_METHOD(foo, int, i) COBJPVT_GEN_INTERFACE_METHOD(bar)  // (0.1)
//		(2.5) some preprocessor tricks, not needed to show here, but in the end it's expanded to
//			COBJPVT_GEN_INTERFACE_METHOD(GEN_METHOD_NAME, ...) 
//			=> COBJPVT_GEN_METHOD_TEMPLATE(GEN_RETURN_STATEMENT, GEN_RETURN_TYPE, GEN_METHODNAME,  GEN_ARGS_SEPERATOR, GEN_ARGS_SIGNATURE, GEN_ARGS_NAME)
//			=> COBJPVT_GEN_METHOD_TEMPLATE(foo, <,> , <int i>) COBJPVT_GEN_METHOD_TEMPLATE(bar, <> , <>)	// cobjpvt-generator-framework.h
//		(2.6) finally the template in applied
//			COBJPVT_GEN_METHOD_TEMPLATE(foo, <,> , <int i>) => COBJPVT_GEN_METHOD_TEMPLATE(bar, <> , <>)	// cobjpvt-generator-framework.h
//			=> void GEN_METHODNAME(cobj_object * self GEN_ARGS_SEPERATOR GEN_ARGS_SIGNATURE);
//			=> void foo           (cobj_object * self , int i);
//			=> void bar           (cobj_object * self);
//		(1.3) the template is undefined
//			#undef COBJPVT_GEN_METHOD_TEMPLATE

#define COBJPVT_GEN_INTERFACE_METHOD(GEN_RETURN_TYPE, GEN_METHOD_NAME, ...) \
	COBJPVT_GEN_METHOD_TEMPLATE(	\
		/*GEN_RETURN_STATEMENT*/ COBJPVT_RETURN_STATMENT(GEN_RETURN_TYPE),	\
		GEN_RETURN_TYPE,						\
		GEN_METHOD_NAME,						\
		/*GEN_ARGS_SEPERATOR*/	COBJ_PP_CONCAT(COBJPVT_GEN_METHOD_ARGS_SEPERATOR_, COBJPVT_PP_NARG(__VA_ARGS__)), \
		/*GEN_ARGS_SIGNATURE*/	COBJ_PP_CONCAT(COBJPVT_GEN_METHOD_ARGS_SIGNATURE_, COBJPVT_PP_NARG(__VA_ARGS__))(__VA_ARGS__), \
		/*GEN_ARGS_NAMES*/		COBJ_PP_CONCAT(COBJPVT_GEN_METHOD_ARGS_NAME_, COBJPVT_PP_NARG(__VA_ARGS__))( __VA_ARGS__))


#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_0
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_2		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_4		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_6		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_8		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_10		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_12		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_14		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_16		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_18		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_20		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_22		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_24		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_26		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_28		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_30		,
#define COBJPVT_GEN_METHOD_ARGS_SEPERATOR_32		,

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_32(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08, GEN_ARGT_09, GEN_ARGN_09, GEN_ARGT_10, GEN_ARGN_10, GEN_ARGT_11, GEN_ARGN_11, GEN_ARGT_12, GEN_ARGN_12, GEN_ARGT_13, GEN_ARGN_13, GEN_ARGT_14, GEN_ARGN_14, GEN_ARGT_15, GEN_ARGN_15)	\
	 GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01, GEN_ARGT_02 GEN_ARGN_02, GEN_ARGT_03 GEN_ARGN_03, GEN_ARGT_04 GEN_ARGN_04, GEN_ARGT_05 GEN_ARGN_05, GEN_ARGT_06 GEN_ARGN_06, GEN_ARGT_07 GEN_ARGN_07, GEN_ARGT_08 GEN_ARGN_08, GEN_ARGT_09 GEN_ARGN_09, GEN_ARGT_10 GEN_ARGN_10, GEN_ARGT_11 GEN_ARGN_11, GEN_ARGT_12 GEN_ARGN_12, GEN_ARGT_13 GEN_ARGN_13, GEN_ARGT_14 GEN_ARGN_14, GEN_ARGT_15 GEN_ARGN_15
	
#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_30(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08, GEN_ARGT_09, GEN_ARGN_09, GEN_ARGT_10, GEN_ARGN_10, GEN_ARGT_11, GEN_ARGN_11, GEN_ARGT_12, GEN_ARGN_12, GEN_ARGT_13, GEN_ARGN_13, GEN_ARGT_14, GEN_ARGN_14)	\
	 GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01, GEN_ARGT_02 GEN_ARGN_02, GEN_ARGT_03 GEN_ARGN_03, GEN_ARGT_04 GEN_ARGN_04, GEN_ARGT_05 GEN_ARGN_05, GEN_ARGT_06 GEN_ARGN_06, GEN_ARGT_07 GEN_ARGN_07, GEN_ARGT_08 GEN_ARGN_08, GEN_ARGT_09 GEN_ARGN_09, GEN_ARGT_10 GEN_ARGN_10, GEN_ARGT_11 GEN_ARGN_11, GEN_ARGT_12 GEN_ARGN_12, GEN_ARGT_13 GEN_ARGN_13, GEN_ARGT_14 GEN_ARGN_14

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_28(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08, GEN_ARGT_09, GEN_ARGN_09, GEN_ARGT_10, GEN_ARGN_10, GEN_ARGT_11, GEN_ARGN_11, GEN_ARGT_12, GEN_ARGN_12, GEN_ARGT_13, GEN_ARGN_13)	\
	 GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01, GEN_ARGT_02 GEN_ARGN_02, GEN_ARGT_03 GEN_ARGN_03, GEN_ARGT_04 GEN_ARGN_04, GEN_ARGT_05 GEN_ARGN_05, GEN_ARGT_06 GEN_ARGN_06, GEN_ARGT_07 GEN_ARGN_07, GEN_ARGT_08 GEN_ARGN_08, GEN_ARGT_09 GEN_ARGN_09, GEN_ARGT_10 GEN_ARGN_10, GEN_ARGT_11 GEN_ARGN_11, GEN_ARGT_12 GEN_ARGN_12, GEN_ARGT_13 GEN_ARGN_13

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_26(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08, GEN_ARGT_09, GEN_ARGN_09, GEN_ARGT_10, GEN_ARGN_10, GEN_ARGT_11, GEN_ARGN_11, GEN_ARGT_12, GEN_ARGN_12)	\
	 GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01, GEN_ARGT_02 GEN_ARGN_02, GEN_ARGT_03 GEN_ARGN_03, GEN_ARGT_04 GEN_ARGN_04, GEN_ARGT_05 GEN_ARGN_05, GEN_ARGT_06 GEN_ARGN_06, GEN_ARGT_07 GEN_ARGN_07, GEN_ARGT_08 GEN_ARGN_08, GEN_ARGT_09 GEN_ARGN_09, GEN_ARGT_10 GEN_ARGN_10, GEN_ARGT_11 GEN_ARGN_11, GEN_ARGT_12 GEN_ARGN_12

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_24(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08, GEN_ARGT_09, GEN_ARGN_09, GEN_ARGT_10, GEN_ARGN_10, GEN_ARGT_11, GEN_ARGN_11)	\
	 GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01, GEN_ARGT_02 GEN_ARGN_02, GEN_ARGT_03 GEN_ARGN_03, GEN_ARGT_04 GEN_ARGN_04, GEN_ARGT_05 GEN_ARGN_05, GEN_ARGT_06 GEN_ARGN_06, GEN_ARGT_07 GEN_ARGN_07, GEN_ARGT_08 GEN_ARGN_08, GEN_ARGT_09 GEN_ARGN_09, GEN_ARGT_10 GEN_ARGN_10, GEN_ARGT_11 GEN_ARGN_11

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_22(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08, GEN_ARGT_09, GEN_ARGN_09, GEN_ARGT_10, GEN_ARGN_10)	\
	 GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01, GEN_ARGT_02 GEN_ARGN_02, GEN_ARGT_03 GEN_ARGN_03, GEN_ARGT_04 GEN_ARGN_04, GEN_ARGT_05 GEN_ARGN_05, GEN_ARGT_06 GEN_ARGN_06, GEN_ARGT_07 GEN_ARGN_07, GEN_ARGT_08 GEN_ARGN_08, GEN_ARGT_09 GEN_ARGN_09, GEN_ARGT_10 GEN_ARGN_10

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_20(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08, GEN_ARGT_09, GEN_ARGN_09)	\
	 GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01, GEN_ARGT_02 GEN_ARGN_02, GEN_ARGT_03 GEN_ARGN_03, GEN_ARGT_04 GEN_ARGN_04, GEN_ARGT_05 GEN_ARGN_05, GEN_ARGT_06 GEN_ARGN_06, GEN_ARGT_07 GEN_ARGN_07, GEN_ARGT_08 GEN_ARGN_08, GEN_ARGT_09 GEN_ARGN_09

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_18(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08)	\
	GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01, GEN_ARGT_02 GEN_ARGN_02, GEN_ARGT_03 GEN_ARGN_03, GEN_ARGT_04 GEN_ARGN_04, GEN_ARGT_05 GEN_ARGN_05, GEN_ARGT_06 GEN_ARGN_06, GEN_ARGT_07 GEN_ARGN_07, GEN_ARGT_08 GEN_ARGN_08

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_16(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07)	\
	GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01, GEN_ARGT_02 GEN_ARGN_02, GEN_ARGT_03 GEN_ARGN_03, GEN_ARGT_04 GEN_ARGN_04, GEN_ARGT_05 GEN_ARGN_05, GEN_ARGT_06 GEN_ARGN_06, GEN_ARGT_07 GEN_ARGN_07

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_14(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06)	\
	GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01, GEN_ARGT_02 GEN_ARGN_02, GEN_ARGT_03 GEN_ARGN_03, GEN_ARGT_04 GEN_ARGN_04, GEN_ARGT_05 GEN_ARGN_05, GEN_ARGT_06 GEN_ARGN_06

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_12(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05)	\
	GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01, GEN_ARGT_02 GEN_ARGN_02, GEN_ARGT_03 GEN_ARGN_03, GEN_ARGT_04 GEN_ARGN_04, GEN_ARGT_05 GEN_ARGN_05

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_10(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04)	\
	GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01, GEN_ARGT_02 GEN_ARGN_02, GEN_ARGT_03 GEN_ARGN_03, GEN_ARGT_04 GEN_ARGN_04

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_8(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03)	\
	GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01, GEN_ARGT_02 GEN_ARGN_02, GEN_ARGT_03 GEN_ARGN_03

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_6(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02)	\
	GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01, GEN_ARGT_02 GEN_ARGN_02

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_4(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01)	\
	GEN_ARGT_00 GEN_ARGN_00, GEN_ARGT_01 GEN_ARGN_01

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_2(GEN_ARGT_00, GEN_ARGN_00)	\
	GEN_ARGT_00 GEN_ARGN_00

#define COBJPVT_GEN_METHOD_ARGS_SIGNATURE_0()

#define COBJPVT_GEN_METHOD_ARGS_NAME_32(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08, GEN_ARGT_09, GEN_ARGN_09, GEN_ARGT_10, GEN_ARGN_10, GEN_ARGT_11, GEN_ARGN_11, GEN_ARGT_12, GEN_ARGN_12, GEN_ARGT_13, GEN_ARGN_13, GEN_ARGT_14, GEN_ARGN_14, GEN_ARGT_15, GEN_ARGN_15)	\
	 GEN_ARGN_00, GEN_ARGN_01, GEN_ARGN_02, GEN_ARGN_03, GEN_ARGN_04, GEN_ARGN_05, GEN_ARGN_06, GEN_ARGN_07, GEN_ARGN_08, GEN_ARGN_09, GEN_ARGN_10, GEN_ARGN_11, GEN_ARGN_12, GEN_ARGN_13, GEN_ARGN_14, GEN_ARGN_15
	
#define COBJPVT_GEN_METHOD_ARGS_NAME_30(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08, GEN_ARGT_09, GEN_ARGN_09, GEN_ARGT_10, GEN_ARGN_10, GEN_ARGT_11, GEN_ARGN_11, GEN_ARGT_12, GEN_ARGN_12, GEN_ARGT_13, GEN_ARGN_13, GEN_ARGT_14, GEN_ARGN_14)	\
	 GEN_ARGN_00, GEN_ARGN_01, GEN_ARGN_02, GEN_ARGN_03, GEN_ARGN_04, GEN_ARGN_05, GEN_ARGN_06, GEN_ARGN_07, GEN_ARGN_08, GEN_ARGN_09, GEN_ARGN_10, GEN_ARGN_11, GEN_ARGN_12, GEN_ARGN_13, GEN_ARGN_14

#define COBJPVT_GEN_METHOD_ARGS_NAME_28(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08, GEN_ARGT_09, GEN_ARGN_09, GEN_ARGT_10, GEN_ARGN_10, GEN_ARGT_11, GEN_ARGN_11, GEN_ARGT_12, GEN_ARGN_12, GEN_ARGT_13, GEN_ARGN_13)	\
	 GEN_ARGN_00, GEN_ARGN_01, GEN_ARGN_02, GEN_ARGN_03, GEN_ARGN_04, GEN_ARGN_05, GEN_ARGN_06, GEN_ARGN_07, GEN_ARGN_08, GEN_ARGN_09, GEN_ARGN_10, GEN_ARGN_11, GEN_ARGN_12, GEN_ARGN_13

#define COBJPVT_GEN_METHOD_ARGS_NAME_26(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08, GEN_ARGT_09, GEN_ARGN_09, GEN_ARGT_10, GEN_ARGN_10, GEN_ARGT_11, GEN_ARGN_11, GEN_ARGT_12, GEN_ARGN_12)	\
	 GEN_ARGN_00, GEN_ARGN_01, GEN_ARGN_02, GEN_ARGN_03, GEN_ARGN_04, GEN_ARGN_05, GEN_ARGN_06, GEN_ARGN_07, GEN_ARGN_08, GEN_ARGN_09, GEN_ARGN_10, GEN_ARGN_11, GEN_ARGN_12

#define COBJPVT_GEN_METHOD_ARGS_NAME_24(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08, GEN_ARGT_09, GEN_ARGN_09, GEN_ARGT_10, GEN_ARGN_10, GEN_ARGT_11, GEN_ARGN_11)	\
	 GEN_ARGN_00, GEN_ARGN_01, GEN_ARGN_02, GEN_ARGN_03, GEN_ARGN_04, GEN_ARGN_05, GEN_ARGN_06, GEN_ARGN_07, GEN_ARGN_08, GEN_ARGN_09, GEN_ARGN_10, GEN_ARGN_11

#define COBJPVT_GEN_METHOD_ARGS_NAME_22(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08, GEN_ARGT_09, GEN_ARGN_09, GEN_ARGT_10, GEN_ARGN_10)	\
	 GEN_ARGN_00, GEN_ARGN_01, GEN_ARGN_02, GEN_ARGN_03, GEN_ARGN_04, GEN_ARGN_05, GEN_ARGN_06, GEN_ARGN_07, GEN_ARGN_08, GEN_ARGN_09, GEN_ARGN_10

#define COBJPVT_GEN_METHOD_ARGS_NAME_20(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08, GEN_ARGT_09, GEN_ARGN_09)	\
	 GEN_ARGN_00, GEN_ARGN_01, GEN_ARGN_02, GEN_ARGN_03, GEN_ARGN_04, GEN_ARGN_05, GEN_ARGN_06, GEN_ARGN_07, GEN_ARGN_08, GEN_ARGN_09

#define COBJPVT_GEN_METHOD_ARGS_NAME_18(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07, GEN_ARGT_08, GEN_ARGN_08)	\
	GEN_ARGN_00, GEN_ARGN_01, GEN_ARGN_02, GEN_ARGN_03, GEN_ARGN_04, GEN_ARGN_05, GEN_ARGN_06, GEN_ARGN_07, GEN_ARGN_08

#define COBJPVT_GEN_METHOD_ARGS_NAME_16(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06, GEN_ARGT_07, GEN_ARGN_07)	\
	GEN_ARGN_00, GEN_ARGN_01, GEN_ARGN_02, GEN_ARGN_03, GEN_ARGN_04, GEN_ARGN_05, GEN_ARGN_06, GEN_ARGN_07

#define COBJPVT_GEN_METHOD_ARGS_NAME_14(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05, GEN_ARGT_06, GEN_ARGN_06)	\
	GEN_ARGN_00, GEN_ARGN_01, GEN_ARGN_02, GEN_ARGN_03, GEN_ARGN_04, GEN_ARGN_05, GEN_ARGN_06

#define COBJPVT_GEN_METHOD_ARGS_NAME_12(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04, GEN_ARGT_05, GEN_ARGN_05)	\
	GEN_ARGN_00, GEN_ARGN_01, GEN_ARGN_02, GEN_ARGN_03, GEN_ARGN_04, GEN_ARGN_05

#define COBJPVT_GEN_METHOD_ARGS_NAME_10(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03, GEN_ARGT_04, GEN_ARGN_04)	\
	GEN_ARGN_00, GEN_ARGN_01, GEN_ARGN_02, GEN_ARGN_03, GEN_ARGN_04

#define COBJPVT_GEN_METHOD_ARGS_NAME_8(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02, GEN_ARGT_03, GEN_ARGN_03)	\
	GEN_ARGN_00, GEN_ARGN_01, GEN_ARGN_02, GEN_ARGN_03

#define COBJPVT_GEN_METHOD_ARGS_NAME_6(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01, GEN_ARGT_02, GEN_ARGN_02)	\
	GEN_ARGN_00, GEN_ARGN_01, GEN_ARGN_02

#define COBJPVT_GEN_METHOD_ARGS_NAME_4(GEN_ARGT_00, GEN_ARGN_00, GEN_ARGT_01, GEN_ARGN_01)	\
	GEN_ARGN_00, GEN_ARGN_01

#define COBJPVT_GEN_METHOD_ARGS_NAME_2(GEN_ARGT_00, GEN_ARGN_00)	\
	GEN_ARGN_00

#define COBJPVT_GEN_METHOD_ARGS_NAME_0()

#define COBJPVT_RETURN_STATMENT(GEN_RETURN_TYPE)	\
	COBJ_PP_CONCAT(COBJPVT_HLP_IS_VOID_, COBJPVT_HLP_LST_COUNT(GEN_RETURN_TYPE))
	
#define COBJPVT_HLP_IS_VOID_0
#define COBJPVT_HLP_IS_VOID_1	return

#define COBJPVT_HLP_IS_VOID_void
#define COBJPVT_HLP_LST(GEN_RETURN_TYPE)	\
	COBJ_PP_CONCAT(COBJPVT_HLP_IS_VOID_, GEN_RETURN_TYPE)
#define COBJPVT_HLP_LST_COUNT(GEN_RETURN_TYPE)	\
	COBJPVT_PP_NARG(COBJPVT_HLP_LST(GEN_RETURN_TYPE))



//////////////////////////////////////////////////////////////////////////
//	Variables-Generation
//	This macros defines the naming of the variables
#define COBJPVT_GEN_CLASS_VARIABLE(GEN_VARIABLE_TYPE, GEN_VARIABLE_NAME)	\
	COBJPVT_GEN_CLASS_VARIABLE_TEMPLATE(GEN_VARIABLE_TYPE, GEN_VARIABLE_NAME)


//////////////////////////////////////////////////////////////////////////
//	Parameters-Generation
//	This macros defines the naming of the variables
#define COBJPVT_GEN_CLASS_PARAMETER(GEN_PARAMETER_TYPE, GEN_PARAMETER_NAME)	\
	COBJPVT_GEN_CLASS_PARAMETER_TEMPLATE(GEN_PARAMETER_TYPE, GEN_PARAMETER_NAME)
		

//////////////////////////////////////////////////////////////////////////
//	Interfaces-Generation
#define COBJPVT_GEN_CLASS_INTERFACE(GEN_INTERFACE_NAME)	\
	COBJPVT_GEN_CLASS_INTERFACE_TEMPLATE(GEN_INTERFACE_NAME)



#endif