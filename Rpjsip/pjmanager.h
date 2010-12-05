int init(char *domain, char *user, char *passwd, char *proxy);

void destroy();

int call( long acc_id, char* number, char* domain );

void endCall(int call_id);