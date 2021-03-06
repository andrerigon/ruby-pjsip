#include <pjsua-lib/pjsua.h>
#define THIS_FILE	"APP"


#include <stdlib.h>

#include "pjmanager.h"


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

int init(  char *domain, char * user, char * passwd, char *proxy)
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
		if (status != PJ_SUCCESS)
		{
			error_exit("Error creating transport", status) ;
		}
    }

    /* Initialization is done, now start pjsua */
    status = pjsua_start();
    if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);

    /* Register to SIP server by creating SIP account. */
    {
	pjsua_acc_config cfg;

	pjsua_acc_config_default(&cfg);	
	
	char reg_uri[5 + strlen(domain)];
	char id[6+strlen(user)+strlen(passwd)];
	char proxy_uri[22+strlen(proxy)];

	sprintf( reg_uri, "sip:%s", domain);
	sprintf( id, "sip:%s@%s", user, domain);
	sprintf( proxy_uri, "sip:%s;lr;transport=UDP", proxy);
	
	cfg.id = pj_str(id);
    cfg.reg_uri = pj_str(reg_uri);
    cfg.cred_count = 1;
    cfg.cred_info[0].scheme = pj_str((char*)"digest");
    cfg.cred_info[0].realm = pj_str(domain);
    cfg.cred_info[0].username = pj_str(user);
    cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    cfg.cred_info[0].data = pj_str(passwd);
    cfg.proxy_cnt = 1;

    cfg.proxy[0] = pj_str(proxy_uri);

	status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
	if (status != PJ_SUCCESS) error_exit("Error adding account", status);
    }
	return acc_id;
}

int call( int acc_id, char* number, char* domain )
{
		char str_uri[13+strlen(number)+strlen(domain)];

		sprintf( str_uri, "sip:%s@%s.com.br", number, domain);

		pj_str_t uri = pj_str(str_uri);
		pjsua_call_id call_id;
		
	    pj_status_t	status = pjsua_call_make_call(acc_id, &uri, 0, NULL, NULL, &call_id);

		if (status != PJ_SUCCESS) 
		{ 
			error_exit("Error making call", status); 
		}
		return call_id;
}

void destroy()
{
	pjsua_destroy();
}

void endCall(int call_id)
{
	pjsua_call_id id = call_id;
	pjsua_call_hangup(id, 0, NULL, NULL);	
}
