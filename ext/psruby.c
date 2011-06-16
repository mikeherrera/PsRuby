#include "ruby.h"
#include <libps/pslib.h>

static VALUE rb_mPsRuby;
static VALUE rb_cPsDoc;

static VALUE
psdoc_new (VALUE class, VALUE filename )
{
	PSDoc *ps ;
	VALUE argv[1] ;
	ps = PS_new() ;
	VALUE ps_document = Data_Wrap_Struct(rb_cPsDoc, 0, free, ps) ;
	argv[0] = filename ;
	rb_obj_call_init(ps_document, 1, argv) ;
	return ps_document ;
}

static VALUE 
psdoc_init (VALUE self, VALUE filename )
{
	PSDoc *ps ;
	char *ps_filename = STR2CSTR(filename)  ;
	Data_Get_Struct(self, PSDoc, ps) ;	
	PS_open_file(ps, ps_filename) ;
	rb_iv_set(self, "@document_filename", filename) ;
	return self ;
}

static VALUE 
psdoc_document_filename (VALUE self)
{
	return rb_iv_get(self, "@document_filename") ;
}

static VALUE 
psdoc_set_info (VALUE self, VALUE key, VALUE value)
{
	PSDoc *ps ;
	char *ps_key = STR2CSTR(key) ;
	char *ps_value = STR2CSTR(value) ;
	Data_Get_Struct(self, PSDoc, ps) ;
	PS_set_info(ps, ps_key, ps_value) ;
	
	return self ;
}

static VALUE
psdoc_set_parameter (VALUE self, VALUE key, VALUE value)
{
	PSDoc *ps ;
	char *ps_key = STR2CSTR(key) ;
	char *ps_value = STR2CSTR(value) ;
    Data_Get_Struct(self, PSDoc, ps) ;
	PS_set_parameter(ps, ps_key, ps_value) ;
	
	return self ;
}

static VALUE
psdoc_begin_page (VALUE self, VALUE width, VALUE height)
{
	PSDoc *ps ;
	double ps_width, ps_height ;
	ps_width = NUM2DBL(width) ;
	ps_height = NUM2DBL(height) ;
    Data_Get_Struct(self, PSDoc, ps) ;
	PS_begin_page(ps, (float) ps_width, (float) ps_height) ;

	return self ;
}

static VALUE
psdoc_end_page (VALUE self)
{
	PSDoc *ps ;
	Data_Get_Struct(self, PSDoc, ps) ;
	PS_end_page(ps) ;
	
	return self ;
}

static VALUE
psdoc_close (VALUE self)
{
	PSDoc *ps ;
	Data_Get_Struct(self, PSDoc, ps) ;
	PS_close(ps) ;

	return self ;
}

void
Init_psruby ( )
{
	rb_mPsRuby = rb_define_module ("PsRuby");
	rb_cPsDoc = rb_define_class_under (rb_mPsRuby, "PsDoc", rb_cObject) ;
	rb_define_singleton_method(rb_cPsDoc, "new", psdoc_new, 1) ;
	rb_define_method (rb_cPsDoc, "initialize", psdoc_init, 1) ;
	rb_define_method (rb_cPsDoc, "document_filename", psdoc_document_filename, 0) ;
	rb_define_method (rb_cPsDoc, "set_info", psdoc_set_info, 2) ;
	rb_define_method (rb_cPsDoc, "set_parameter", psdoc_set_parameter, 2) ;
	rb_define_method (rb_cPsDoc, "begin_page", psdoc_begin_page, 2) ;
	rb_define_method (rb_cPsDoc, "end_page", psdoc_end_page, 0) ;
	rb_define_method (rb_cPsDoc, "close_", psdoc_close, 0) ;
}

