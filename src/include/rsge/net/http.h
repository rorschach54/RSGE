#ifndef __RSGE_NET_HTTP_H_
#define __RSGE_NET_HTTP_H_ 1

/** \file src/include/rsge/net/http.h
 * \brief HTTP stuff.
 * \author Spaceboy Ross
 */

#include <curl/curl.h>
#include <rsge/error.h>
#include <list.h>
#include <stdarg.h>

/** \enum rsge_net_http_method_e
 * \brief HTTP methods
 */
typedef enum {
	/**
	 * HTTP GET method
	 */
	RSGE_HTTP_METHOD_GET,
	/**
	 * HTTP POST method
	 */
	RSGE_HTTP_METHOD_POST,
	/**
	 * HTTP PUT method
	 */
	RSGE_HTTP_METHOD_PUT
} rsge_net_http_method_e;

/** \struct rsge_net_http_header_t src/include/rsge/net/http.h rsge/net/http.h
 * \brief HTTP header
 */
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

/** \struct rsge_net_http_cookie_t src/include/rsge/net/http.h rsge/net/http.h
 * \brief HTTP cookie
 */
typedef struct {
	/**
	 * \brief Cookie hostname
	 */
	char* hostname;

	/**
	 * \brief Include subdomains.
	 */
	int subdomains;

	/**
	 * \brief Cookie path.
	 */
	char* path;

	/**
	 * \brief Secure.
	 */
	int secure;

	/**
	 * \brief Cookie expires.
	 */
	char* expires;

	/**
	 * \brief Cookie name.
	 */
	char* name;

	/**
	 * \brief Cookie value.
	 */
	char* value;
} rsge_net_http_cookie_t;

/** \struct rsge_net_http_client_t src/include/rsge/net/http.h rsge/net/http.h
 * \brief HTTP client
 */
typedef struct {
	/**
	 * \brief The instance of libcurl created.
	 */
	CURL* curl;

	/**
	 * \brief The number of default headers.
	 */
	int defaultHeaderCount;

	/**
	 * \brief The default headers.
	 */
	rsge_net_http_header_t* defaultHeaders;

	/**
	 * \brief The number of headers received.
	 */
	int headerCount;

	/**
	 * \brief The headers received.
	 */
	rsge_net_http_header_t* headers;

	/**
	 * \brief The number of cookies set.
	 */
	int cookieCount;

	/**
	 * \brief The cookies set.
	 */
	rsge_net_http_cookie_t* cookies;

	/**
	 * \brief The content received from the connection.
	 */
	char* content;

	/**
	 * \brief The size of the content received from the connection.
	 */
	size_t contentSize;

	/**
	 * \brief The HTTP status received.
	 */
	int status;

	/**
	 * \brief Temporary variable to store the HTTP headers received.
	 */
	list_t* tmpHeaders;
} rsge_net_http_client_t;

/**
 * \fn rsge_error_e rsge_net_http_client_create(rsge_net_http_client_t* client)
 * \brief Creates an HTTP Client.
 * \param[out] client The pointer to store the client.
 * \return An error code.
 */
rsge_error_e rsge_net_http_client_create(rsge_net_http_client_t* client);

/**
 * \fn rsge_error_e rsge_net_http_client_connect(rsge_net_http_client_t* client,char* url,rsge_net_http_method_e method)
 * \brief Connects to an HTTP server.
 * \param[out] client The pointer to a client.
 * \param[in] url The URL of the server.
 * \param[in] method The HTTP method to use.
 * \return An error code.
 */
rsge_error_e rsge_net_http_client_connect(rsge_net_http_client_t* client,char* url,rsge_net_http_method_e method);

/**
 * \fn rsge_error_e rsge_net_http_client_disconnect(rsge_net_http_client_t* client)
 * \brief Disconnects from an HTTP server.
 * \param[out] client The pointer to a client.
 * \return An error code.
 */
rsge_error_e rsge_net_http_client_disconnect(rsge_net_http_client_t* client);

/**
 * \fn rsge_error_e rsge_net_http_client_destroy(rsge_net_http_client_t* client)
 * \brief Destroyes the HTTP client.
 * \param[out] client The pointer to a client.
 * \return An error code.
 */
rsge_error_e rsge_net_http_client_destroy(rsge_net_http_client_t* client);

#endif
