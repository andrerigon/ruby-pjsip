// include the Ruby headers 
#include "ruby.h"

// include pjmanager header
#include <pjmanager.h>

char * string(VALUE v);

// Defining a space for information and references about the module to be stored internally
VALUE Rpjsip = Qnil;

// Prototype for the initialization method - called by ruby
void Init_rpjsip();

// functions prototypes - must be prefixed by method_
VALUE method_pjsip_init(VALUE self, VALUE proxy, VALUE domain, VALUE user, VALUE passwd);

VALUE method_pjsip_destroy(VALUE self);

VALUE method_call(VALUE self, VALUE acc_id, VALUE number, VALUE domain);
	
// initializes the module
void Init_rpjsip() {
	Rpjsip = rb_define_module("Rpjsip");
	rb_define_method(Rpjsip, "pjsip_init", method_pjsip_init, 4);
	rb_define_method(Rpjsip, "pjsip_destroy", method_pjsip_destroy, 0);	
	rb_define_method(Rpjsip, "call", method_call, 3);	
}

VALUE method_pjsip_init(VALUE self, VALUE proxy, VALUE domain, VALUE user, VALUE passwd){
	return INT2NUM(init(string(domain), string(user), string(passwd), string(proxy)));
}

VALUE method_call(VALUE self, VALUE acc_id, volatile VALUE number, VALUE domain){
	call( rb_num2long(acc_id), string(number), string(domain)  );
	return Qnil;
}

VALUE method_pjsip_destroy(VALUE self){
	destroy();
	return Qnil;
}

char * string(VALUE v){
	return (char*)rb_string_value_cstr(&v);
}
