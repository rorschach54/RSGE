#ifndef __RSGE_NET_HTTP_H_
#define __RSGE_NET_HTTP_H_ 1

#include <rsge/error.h>
#include <list.h>

#define RSGE_NET_HTTP_STATUS_OK 200

#define RSGE_NET_HTTP_MODE_GET "GET"
#define RSGE_NET_HTTP_MODE_PUT "PUT"

typedef struct {
	/**
	 * \brief Header name.
	 */
	char* name;

	/**
	 * \brief Header value.
	 */
	char* value;
} rsge_net_http_header_t;

typedef struct {
	/**
	 * \brief Client header count.
	 */
	int headerCount;

	/**
	 * \brief Client headers.
	 */
	rsge_net_http_header_t* headers;
} rsge_net_http_client_cfg_t;

typedef struct {
	/**
	 * \brief HTTP Client config.
	 */
	rsge_net_http_client_cfg_t* cfg;

	/**
	 * \brief HTTP Client implementation data.
	 */
	void* impl;

	/**
	 * \brief The content recieved from the connection.
	 */
	char* content;

	/**
	 * \brief The HTTP status recieved.
	 */
	int status;

	/**
	 * \brief The number of HTTP headers recieved.
	 */
	int headerCount;

	/**
	 * \brief The HTTP headers recieved.
	 */
	rsge_net_http_header_t* headers;
} rsge_net_http_client_t;

/**
 * \fn rsge_error_e rsge_net_http_clientcfg_fromFile(rsge_net_http_client_cfg_t* cfg,char* path)
 * \brief Loads client config data from an XML File.
 * \param cfg The pointer to store the configuration.
 * \param path The path to the asset.
 * \return An error code.
 */
rsge_error_e rsge_net_http_clientcfg_fromFile(rsge_net_http_client_cfg_t* cfg,char* path);

/**
 * \fn rsge_error_e rsge_net_http_client_create(rsge_net_http_client_t* client,rsge_net_http_client_cfg_t* cfg)
 * \brief Creates an HTTP Client.
 * \param client The pointer to store the client.
 * \param cfg The client's configuration. Set to NULL to use the default config.
 * \return An error code.
 */
rsge_error_e rsge_net_http_client_create(rsge_net_http_client_t* client,rsge_net_http_client_cfg_t* cfg);

/**
 * \fn rsge_error_e rsge_net_http_client_connect(rsge_net_http_client_t* client,char* url,int port,char* mode)
 * \brief Connects to an HTTP server.
 * \param client The pointer to a client.
 * \param url The URL of the server.
 * \param port The port of the server.
 * \param mode The HTTP mode to use.
 * \return An error code.
 */
rsge_error_e rsge_net_http_client_connect(rsge_net_http_client_t* client,char* url,int port,char* mode);

/**
 * \fn rsge_error_e rsge_net_http_client_disconnect(rsge_net_http_client_t* client)
 * \brief Disconnects from an HTTP server.
 * \param client The pointer to a client.
 * \param url The URL of the server.
 * \param port The port of the server.
 * \param mode The HTTP mode to use.
 * \return An error code.
 */
rsge_error_e rsge_net_http_client_disconnect(rsge_net_http_client_t* client);

#endif