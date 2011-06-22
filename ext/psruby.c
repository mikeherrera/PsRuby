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
	VALUE ps_document = Data_Wrap_Struct (rb_cPsDoc, 0, free, ps) ;
	argv[0] = filename ;
	rb_obj_call_init (ps_document, 1, argv) ;

	return ps_document ;
}

static VALUE 
psdoc_init (VALUE self, VALUE filename )
{
	PSDoc *ps ;
	char *ps_filename = STR2CSTR(filename)  ;
	Data_Get_Struct (self, PSDoc, ps) ;	
	PS_open_file (ps, ps_filename) ;
	rb_iv_set (self, "@document_filename", filename) ;

	return self ;
}

static VALUE 
psdoc_document_filename (VALUE self)
{
	return rb_iv_get (self, "@document_filename") ;
}

static VALUE 
psdoc_set_info (VALUE self, VALUE key, VALUE value)
{
	PSDoc *ps ;
	char *ps_key = STR2CSTR (key) ;
	char *ps_value = STR2CSTR (value) ;
	Data_Get_Struct (self, PSDoc, ps) ;
	PS_set_info (ps, ps_key, ps_value) ;
	
	return self ;
}

static VALUE
psdoc_set_parameter (VALUE self, VALUE key, VALUE value)
{
	PSDoc *ps ;
	char *ps_key = STR2CSTR (key) ;
	char *ps_value = STR2CSTR (value) ;
    Data_Get_Struct (self, PSDoc, ps) ;
	PS_set_parameter (ps, ps_key, ps_value) ;
	
	return self ;
}

static VALUE
psdoc_begin_page (VALUE self, VALUE width, VALUE height)
{
	PSDoc *ps ;
	double ps_width, ps_height ;
	ps_width = NUM2DBL (width) ;
	ps_height = NUM2DBL (height) ;
    Data_Get_Struct (self, PSDoc, ps) ;
	PS_begin_page (ps, (float) ps_width, (float) ps_height) ;

	return self ;
}

static VALUE
psdoc_end_page (VALUE self)
{
	PSDoc *ps ;
	Data_Get_Struct (self, PSDoc, ps) ;
	PS_end_page (ps) ;
	
	return self ;
}

static VALUE
psdoc_close (VALUE self)
{
	PSDoc *ps ;
	Data_Get_Struct (self, PSDoc, ps) ;
	PS_close (ps) ;

	return self ;
}

static VALUE
psdoc_open_image_file (VALUE self, VALUE filename)
{
	PSDoc *ps ;
	Data_Get_Struct (self, PSDoc, ps) ;
	char *ps_filename = STR2CSTR (filename) ;
	int imageid = PS_open_image_file (ps, "eps", ps_filename, NULL, (int) 0) ;

	return INT2NUM (imageid) ;
}

static VALUE
psdoc_stroke (VALUE self)
{
	PSDoc *ps ;
	Data_Get_Struct (self, PSDoc, ps) ;
	PS_stroke (ps) ;

	return self ;
}

static VALUE
psdoc_fill (VALUE self)
{
	PSDoc *ps ;
	Data_Get_Struct (self, PSDoc, ps) ;
	PS_fill (ps) ;

	return self ;
}

static VALUE
psdoc_place_image (VALUE self, VALUE image_id, VALUE x, VALUE y, VALUE scale)
{
	PSDoc *ps ;
	int ps_imageid = NUM2INT (image_id) ;
	double ps_x, ps_y, ps_scale ;
	ps_x = NUM2DBL (x) ;
	ps_y = NUM2DBL (y) ;
	ps_scale = NUM2DBL (scale) ;
	PS_place_image (ps, ps_imageid, (float) ps_x, (float) ps_y, (float) ps_scale) ;

	return self ; 
}

static VALUE
psdoc_get_value (VALUE self, VALUE name, VALUE modifier)
{
	PSDoc *ps ;
	Data_Get_Struct (self, PSDoc, ps) ;
	double ps_value, ps_modifier ;
	char *ps_name = STR2CSTR (name) ;
	ps_modifier = NUM2INT (modifier) ;
	ps_value = (double) PS_get_value (ps, ps_name, (float) ps_modifier) ;

	return rb_float_new (ps_value) ;
}

static VALUE
psdoc_find_font (VALUE self, VALUE font_name, VALUE embed)
{
	PSDoc *ps ;
	Data_Get_Struct (self, PSDoc, ps) ;
	char *ps_font_name = STR2CSTR (font_name) ;
	int ps_font ;
	int ps_embed = NUM2INT (embed) ;
	ps_font = PS_findfont (ps, ps_font_name, 'builtin', ps_embed) ;

	return INT2NUM (ps_font) ;
}

void
Init_PsRuby ( )
{
	rb_mPsRuby = rb_define_module ("PsRuby");
	rb_cPsDoc = rb_define_class_under (rb_mPsRuby, "PsDoc", rb_cObject) ;
	rb_define_singleton_method(rb_cPsDoc, "new", psdoc_new, 1) ;
	rb_define_method (rb_cPsDoc, "initialize", psdoc_init, 1) ;
	rb_define_method (rb_cPsDoc, "document_filename", psdoc_document_filename, 0) ;
	rb_define_method (rb_cPsDoc, "set_info", psdoc_set_info, 2) ;
	rb_define_method (rb_cPsDoc, "set_parameter", psdoc_set_parameter, 2) ;
	rb_define_method (rb_cPsDoc, "begin_page", psdoc_begin_page, 2) ;
	rb_define_method (rb_cPsDoc, "open_image_file", psdoc_open_image_file, 1) ;
	rb_define_method (rb_cPsDoc, "place_image", psdoc_place_image, 4) ;
	rb_define_method (rb_cPsDoc, "stroke", psdoc_stroke, 0) ;
	rb_define_method (rb_cPsDoc, "fill", psdoc_fill, 0) ;
	rb_define_method (rb_cPsDoc, "get_value", psdoc_get_value, 2) ;
	rb_define_method (rb_cPsDoc, "find_font", psdoc_find_font, 2) ;
	rb_define_method (rb_cPsDoc, "end_page", psdoc_end_page, 0) ;
	rb_define_method (rb_cPsDoc, "close", psdoc_close, 0) ;

}
