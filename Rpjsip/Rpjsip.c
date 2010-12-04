// Include the Ruby headers and goodies
#include "ruby.h"
#include <pjsua-lib/pjsua.h>
#define THIS_FILE	"APP"

#define SIP_DOMAIN	"azzu.com.br"
#define SIP_USER	"12013283"
#define SIP_PASSWD	"12345"


/* Callback called by the library upon receiving incoming call */
static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
			     pjsip_rx_data *rdata)
{
    pjsua_call_info ci;

    PJ_UNUSED_ARG(acc_id);
    PJ_UNUSED_ARG(rdata);

    pjsua_call_get_info(call_id, &ci);

    PJ_LOG(3,(THIS_FILE, "Incoming call from %.*s!!",
			 (int)ci.remote_info.slen,
			 ci.remote_info.ptr));

    /* Automatically answer incoming calls with 200/OK */
    pjsua_call_answer(call_id, 200, NULL, NULL);
}

/* Callback called by the library when call's state has changed */
static void on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
    pjsua_call_info ci;

    PJ_UNUSED_ARG(e);

    pjsua_call_get_info(call_id, &ci);
    PJ_LOG(3,(THIS_FILE, "Call %d state=%.*s", call_id,
			 (int)ci.state_text.slen,
			 ci.state_text.ptr));
}

/* Callback called by the library when call's media state has changed */
static void on_call_media_state(pjsua_call_id call_id)
{
    pjsua_call_info ci;

    pjsua_call_get_info(call_id, &ci);

    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
	// When media is active, connect call to sound device.
	pjsua_conf_connect(ci.conf_slot, 0);
	pjsua_conf_connect(0, ci.conf_slot);
    }
}

/* Display error and exit application */
static void error_exit(const char *title, pj_status_t status)
{
    pjsua_perror(THIS_FILE, title, status);
    pjsua_destroy();
    exit(1);
}

/*
 * main()
 *
 * argv[1] may contain URL to call.
 */
int go()
{
    pjsua_acc_id acc_id;
    pj_status_t status;

    /* Create pjsua first! */
    status = pjsua_create();
    if (status != PJ_SUCCESS) error_exit("Error in pjsua_create()", status);


    /* Init pjsua */
    {
	pjsua_config cfg;
	pjsua_logging_config log_cfg;

	pjsua_config_default(&cfg);
	cfg.cb.on_incoming_call = &on_incoming_call;
	cfg.cb.on_call_media_state = &on_call_media_state;
	cfg.cb.on_call_state = &on_call_state;

	pjsua_logging_config_default(&log_cfg);
	log_cfg.console_level = 4;

	status = pjsua_init(&cfg, &log_cfg, NULL);
	if (status != PJ_SUCCESS) error_exit("Error in pjsua_init()", status);
    }
	pjsua_media_config media_cfg;
	 pjsua_media_config_default(&media_cfg);

    /* Add UDP transport. */
    {
	pjsua_transport_config cfg;

	pjsua_transport_config_default(&cfg);
	cfg.port = 55060;
	status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, NULL);
	if (status != PJ_SUCCESS) error_exit("Error creating transport", status);
    }

    /* Initialization is done, now start pjsua */
    status = pjsua_start();
    if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);

    /* Register to SIP server by creating SIP account. */
    {
	pjsua_acc_config cfg;

	pjsua_acc_config_default(&cfg);	
	cfg.id = pj_str("sip:12013283@azzu");
    cfg.reg_uri = pj_str("sip:azzu");
    cfg.cred_count = 1;
    cfg.cred_info[0].scheme = pj_str("digest");
    cfg.cred_info[0].realm = pj_str("azzu");
    cfg.cred_info[0].username = pj_str("12013283");
    cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    cfg.cred_info[0].data = pj_str("12345");
    cfg.proxy_cnt = 1;

    cfg.proxy[0] = pj_str("sip:proxy.azzu.com.br;lr;transport=UDP");

	status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
	if (status != PJ_SUCCESS) error_exit("Error adding account", status);
    }

    
	pj_str_t uri = pj_str("sip:3496739006@azzu.com.br");
	status = pjsua_call_make_call(acc_id, &uri, 0, NULL, NULL, NULL);
	if (status != PJ_SUCCESS) error_exit("Error making call", status);
   

    /* Wait until user press "q" to quit. */
    for (;;) {
	char option[10];

	puts("Press 'h' to hangup all calls, 'q' to quit");
	if (fgets(option, sizeof(option), stdin) == NULL) {
	    puts("EOF while reading stdin, will quit now..");
	    break;
	}

	if (option[0] == 'q')
	    break;

	if (option[0] == 'h')
	    pjsua_call_hangup_all();
    }

    /* Destroy pjsua */
    pjsua_destroy();

    return 0;
}


// Defining a space for information and references about the module to be stored internally
VALUE Rpjsip = Qnil;

// Prototype for the initialization method - Ruby calls this, not you
void Init_rpjsip();

// Prototype for our method 'test1' - methods are prefixed by 'method_' here
VALUE method_test1(VALUE self);

VALUE method_add(VALUE self, VALUE a, VALUE b);

VALUE method_pjsip(VALUE self);
	
// The initialization method for this module
void Init_rpjsip() {
	Rpjsip = rb_define_module("Rpjsip");
	rb_define_method(Rpjsip, "test1", method_test1, 0);	
	rb_define_method(Rpjsip, "add", method_add, 2);
	rb_define_method(Rpjsip, "pjsip_init", method_pjsip, 0);	
}

// Our 'test1' method.. it simply returns a value of '10' for now.
VALUE method_test1(VALUE self) {
	rb_eval_string("puts 'rá'");
	int *x = malloc(sizeof(int));
	*x = 25;
	return INT2NUM(*x);
}

VALUE method_add(VALUE self, VALUE a, VALUE b){
	return INT2NUM(rb_num2dbl(a) + rb_num2dbl(b));
}

VALUE method_pjsip(VALUE self){
	go();}