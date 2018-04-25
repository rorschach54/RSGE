#include <rsge/gfx/gl.h>
#include <rsge/net/http.h>
#include <rsge/assets.h>
#include <rsge/game.h>
#include <math.h>
#include <log.h>

rsge_net_http_client_t client;

rsge_error_e rsge_game_init() {
	rsge_net_http_client_cfg_t cfg;
	rsge_error_e err;
	err = rsge_net_http_client_create(&client,NULL);
	if(err != RSGE_ERROR_NONE) return err;
	
	err = rsge_net_http_client_connect(&client,"spaceboyross.tk",80,RSGE_NET_HTTP_METHOD_GET);
	if(err != RSGE_ERROR_NONE) return err;
	
	log_info("%s",client.content);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_uninit() {
	rsge_error_e err = rsge_net_http_client_disconnect(&client);
	if(err != RSGE_ERROR_NONE) return err;
	
	err = rsge_net_http_client_destroy(&client);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_update(double time,int fps) {
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo) {
	gameinfo->name = "RSGE Example - Networking";
	gameinfo->id = "rsge.example.net";
	return RSGE_ERROR_NONE;
}