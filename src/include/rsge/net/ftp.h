#ifndef __RSGE_NET_FTP_H_
#define __RSGE_NET_FTP_H_ 1

/** \file src/include/rsge/net/ftp.h
 * \brief FTP stuff
 * \author Spaceboy Ross
 */

#include <curl/curl.h>
#include <rsge/assets.h>
#include <rsge/error.h>
#include <list.h>
#include <stdarg.h>

/** \struct rsge_net_ftp_client_t src/include/rsge/net/ftp.h rsge/net/ftp.h
 * \brief FTP client.
 */
typedef struct {
	/**
	 * \brief The instance of libcurl created.
	 */
	CURL* curl;

	/**
	 * \brief The content received from the connection. This is also used for uploading.
	 */
	char* content;

	/**
	 * \brief The size of the content received from the connection.
	 */
	size_t contentSize;

	/**
	 * \brief The current offset of where the content is currently being uploaded from.
	 */
	size_t contentOffset;

	/**
	 * \brief The remote time of the file.
	 */
	char* fileTime;

	/**
	 * \brief The base URL of the FTP server.
	 */
	char* url;
} rsge_net_ftp_client_t;

/**
 * \fn rsge_error_e rsge_net_ftp_client_create(rsge_net_ftp_client_t* client)
 * \brief Creates an FTP Client.
 * \param[out] client The pointer to store the client.
 * \return An error code.
 */
rsge_error_e rsge_net_ftp_client_create(rsge_net_ftp_client_t* client);

/**
 * \fn rsge_error_e rsge_net_ftp_client_login(rsge_net_ftp_client_t* client,char* url,char* username,char* password)
 * \brief Connects to an FTP server.
 * \param[out] client The pointer to a client.
 * \param[in] url The base URL of the FTP server.
 * \param[in] username The username to use.
 * \param[in] password The password to use.
 * \return An error code.
 */
rsge_error_e rsge_net_ftp_client_login(rsge_net_ftp_client_t* client,char* url,char* username,char* password);

/**
 * \fn rsge_error_e rsge_net_ftp_client_connect(rsge_net_http_client_t* client,char* url)
 * \brief Connects to an FTP server.
 * \param[out] client The pointer to a client.
 * \param[in] url The base URL of the FTP server.
 * \return An error code.
 */
rsge_error_e rsge_net_ftp_client_connect(rsge_net_ftp_client_t* client,char* url);

/**
 * \fn rsge_error_e rsge_net_ftp_client_upload(rsge_net_http_client_t* client,char* path)
 * \brief Uploads to an FTP server.
 * \param[out] client The pointer to a client.
 * \param[in] path The path to the file.
 * \return An error code.
 */
rsge_error_e rsge_net_ftp_client_upload(rsge_net_ftp_client_t* client,char* path);

/**
 * \fn rsge_error_e rsge_net_ftp_client_download(rsge_net_http_client_t* client,char* path)
 * \brief Downloads from an FTP server.
 * \param[out] client The pointer to a client.
 * \param[in] path The path to the file.
 * \return An error code.
 */
rsge_error_e rsge_net_ftp_client_download(rsge_net_ftp_client_t* client,char* path);

/**
 * \fn rsge_error_e rsge_net_ftp_client_disconnect(rsge_net_ftp_client_t* client)
 * \brief Disconnects from an FTP server.
 * \param[out] client The pointer to a client.
 * \return An error code.
 */
rsge_error_e rsge_net_ftp_client_disconnect(rsge_net_ftp_client_t* client);

/**
 * \fn rsge_error_e rsge_net_ftp_client_destroy(rsge_net_ftp_client_t* client)
 * \brief Destroyes the FTP client.
 * \param[out] client The pointer to a client.
 * \return An error code.
 */
rsge_error_e rsge_net_ftp_client_destroy(rsge_net_ftp_client_t* client);

/**
 * \fn rsge_error_e rsge_net_ftp_client_createAssets(rsge_assets_t* assets,rsge_net_ftp_client_t* client)
 * \brief Creates assets from an FTP client.
 * \param[out] assets The pointer to store the FTP assets.
 * \param[in] client The pointer to a client.
 * \return An error code.
 */
rsge_error_e rsge_net_ftp_client_createAssets(rsge_assets_t* assets,rsge_net_ftp_client_t* client);

#endif
