#ifndef __RSGE_NET_HTTPS_H_
#define __RSGE_NET_HTTPS_H_ 1

/** \file src/include/rsge/net/https.h
 * \brief HTTPS stuff.
 * \author Spaceboy Ross
 */

#include <curl/curl.h>
#include <rsge/net/http.h>
#include <rsge/error.h>
#include <list.h>
#include <stdarg.h>

/** \struct rsge_net_https_client_t src/include/rsge/net/https.h rsge/net/https.h
 * \brief HTTPS client
 */
typedef struct {
	/**
	 * \brief The instance of an HTTP client.
	 */
	rsge_net_http_client_t client;
} rsge_net_https_client_t;

/**
 * \fn rsge_error_e rsge_net_https_client_create(rsge_net_https_client_t* client)
 * \brief Creates an HTTPS Client.
 * \param[out] client The pointer to store the client.
 * \return An error code.
 */
rsge_error_e rsge_net_https_client_create(rsge_net_https_client_t* client);

/**
 * \fn rsge_error_e rsge_net_https_client_connect(rsge_net_https_client_t* client,char* url,rsge_net_http_method_e method)
 * \brief Connects to an HTTPS server.
 * \param[out] client The pointer to a client.
 * \param[in] url The URL of the server.
 * \param[in] method The HTTPS method to use.
 * \return An error code.
 */
rsge_error_e rsge_net_https_client_connect(rsge_net_https_client_t* client,char* url,rsge_net_http_method_e method);

/**
 * \fn rsge_error_e rsge_net_https_client_disconnect(rsge_net_https_client_t* client)
 * \brief Disconnects from an HTTPS server.
 * \param[out] client The pointer to a client.
 * \return An error code.
 */
rsge_error_e rsge_net_https_client_disconnect(rsge_net_https_client_t* client);

/**
 * \fn rsge_error_e rsge_net_https_client_destroy(rsge_net_https_client_t* client)
 * \brief Destroyes the HTTPS client.
 * \param[out] client The pointer to a client.
 * \return An error code.
 */
rsge_error_e rsge_net_https_client_destroy(rsge_net_https_client_t* client);

#endif
