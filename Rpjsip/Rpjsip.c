// include the Ruby headers 
#include "ruby.h"

// include pjmanager header
#include <pjmanager.h>

//util
char * string(VALUE v);

int  integer(VALUE v);

VALUE hash_get(VALUE hash, char * key);

void * get_value( VALUE hash, char *key, void * (*c)(VALUE) );

VALUE execBlock(VALUE v);

// Defining a space for information and references about the module to be stored internally
VALUE Rpjsip = Qnil;

// Prototype for the initialization method - called by ruby
void Init_rpjsip();

// functions prototypes - must be prefixed by method_
VALUE method_pjsip_init(VALUE self, VALUE hash);

VALUE method_pjsip_destroy(VALUE self);

VALUE method_make_call(VALUE self, VALUE hash);

VALUE method_end_call(VALUE self, VALUE call_id);
	
// initializes the module
void Init_rpjsip() {
	Rpjsip = rb_define_module("Rpjsip");
	rb_define_method(Rpjsip, "pjsip_init", method_pjsip_init, 1);
	rb_define_method(Rpjsip, "pjsip_destroy", method_pjsip_destroy, 0);	
	rb_define_method(Rpjsip, "make_call", method_make_call, 1);	
	rb_define_method(Rpjsip, "end_call", method_end_call, 1 );
}

VALUE method_pjsip_init(VALUE self, VALUE hash)
{
	char * domain = get_value(hash, ":domain", string);
	char * user = get_value(hash, ":user", string);
	char * passwd = get_value(hash, ":passwd", string);
	char * proxy = get_value(hash, ":proxy", string);
	return INT2NUM(init(domain, user, passwd, proxy));
}

VALUE method_make_call(VALUE self, VALUE hash)
{
	//VALUE acc_id, volatile VALUE number, VALUE domain
	
	int  acc_id = get_value( hash, ":account_id", integer );
	char * number = get_value(hash, ":number", string);
	char * domain = get_value(hash, ":domain", string);
	VALUE v =  INT2NUM(call( acc_id, number, domain ));
	execBlock(self);
	return v;	
}

VALUE execBlock(VALUE v)
{
	if (rb_block_given_p())
	{
		return  rb_yield(v);
	}
	return Qnil;
}

VALUE method_end_call(VALUE self, VALUE call_id)
{
	endCall( NUM2INT(call_id) );
	return Qnil;
}

VALUE method_pjsip_destroy(VALUE self)
{
	destroy();
	return Qnil;
}

char * string(VALUE v)
{
	return (char*)rb_string_value_cstr(&v);
}

int  integer(VALUE v){
	return NUM2INT(v);
}

VALUE hash_get(VALUE hash, char * key)
{
	return rb_hash_aref(hash,rb_eval_string(key) );
}

void * get_value( VALUE hash, char *key, void * (*converter)(VALUE) )
{
	VALUE v = hash_get(hash, key);
	if(v  == Qnil){
		rb_raise( rb_eException, "Symbol %s not found.", key );
	}
	return converter(v);
	
}
