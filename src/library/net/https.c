#include <libxml/parser.h>
#include <rsge/net/https.h>
#include <rsge/assets.h>
#include <log.h>
#include <string.h>

rsge_error_e rsge_net_https_client_create(rsge_net_https_client_t* client) {
	memset(client,0,sizeof(rsge_net_https_client_t));
	rsge_error_e err = rsge_net_http_client_create(&client->client);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_https_client_connect(rsge_net_https_client_t* client,char* url,rsge_net_http_method_e method) {
	rsge_error_e err = rsge_net_http_client_connect(&client->client,url,method);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_https_client_disconnect(rsge_net_https_client_t* client) {
	rsge_error_e err = rsge_net_http_client_disconnect(&client->client);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_https_client_destroy(rsge_net_https_client_t* client) {
	rsge_error_e err = rsge_net_http_client_destroy(&client->client);
	if(err != RSGE_ERROR_NONE) return err;
	memset(client,0,sizeof(rsge_net_https_client_t));
	return RSGE_ERROR_NONE;
}
