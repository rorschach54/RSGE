#include <libxml/parser.h>
#include <rsge/net/ftp.h>
#include <rsge/assets.h>
#include <log.h>
#include <string.h>

static size_t rsge_net_ftp_client_write(void* buffer,size_t size,size_t nmemb,void* userdata) {
	rsge_net_ftp_client_t* client = (rsge_net_ftp_client_t*)userdata;
	if(client->content == NULL) {
		client->content = malloc(client->contentSize+(size*nmemb)+1);
		if(!client->content) {
			log_error("Failed to allocate %d bytes of memory",(size*nmemb)+1);
			return 0;
		}
	} else {
		client->content = realloc(client->content,client->contentSize+(size*nmemb)+1);
		if(!client->content) {
			log_error("Failed to reallocate %d bytes of memory",client->contentSize+(size*nmemb)+1);
			return 0;
		}
	}
	memcpy(&(client->content[client->contentSize]),buffer,size*nmemb);
	client->contentSize += size*nmemb;
	client->content[client->contentSize] = 0;
	return size*nmemb;
}

static size_t rsge_net_ftp_client_read(void* buffer,size_t size,size_t nmemb,void* userdata) {
	rsge_net_ftp_client_t* client = (rsge_net_ftp_client_t*)userdata;
	size_t max = size*nmemb;
	if(max < 1) return 0;
	if(client->contentSize) {
		size_t copylen = max;
		if(copylen > client->contentSize) copylen = client->contentSize;
		memcpy(buffer,client->content+client->contentOffset,copylen);
		client->contentOffset += copylen;
		client->contentSize -= copylen;
		return copylen;
	}
	return  0;
}

rsge_error_e rsge_net_ftp_client_create(rsge_net_ftp_client_t* client) {
	memset(client,0,sizeof(rsge_net_ftp_client_t));

	/* Setup CURL */
	client->curl = curl_easy_init();
	if(!client->curl) {
		return RSGE_ERROR_CURL;
	}
	curl_easy_setopt(client->curl,CURLOPT_FILETIME,1L);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_ftp_client_login(rsge_net_ftp_client_t* client,char* url,char* username,char* password) {
	curl_easy_setopt(client->curl,CURLOPT_USERNAME,username);
	curl_easy_setopt(client->curl,CURLOPT_PASSWORD,password);
	return rsge_net_ftp_client_connect(client,url);
}

rsge_error_e rsge_net_ftp_client_connect(rsge_net_ftp_client_t* client,char* url) {
	if(strlen(url)-1 > 0 && url[strlen(url)-1] == '/') url[strlen(url)-1] = 0;
	client->url = url;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_ftp_client_upload(rsge_net_ftp_client_t* client,char* path) {
	client->contentOffset = 0;
	if(strlen(path) > 0 && path[0] == '/') path++;

	char url_tmp[1];
	size_t url_sz = sprintf(url_tmp,"%s/%s",client->url,path);
	char* url = malloc(url_sz);
	if(!url) {
		log_error("Failed to allocate %d bytes of memory",url_sz);
		return RSGE_ERROR_MALLOC;
	}
	memset(url,0,url_sz);
	sprintf(url,"%s/%s",client->url,path);
	url[url_sz] = 0;

	curl_easy_setopt(client->curl,CURLOPT_UPLOAD,1L);
	curl_easy_setopt(client->curl,CURLOPT_READFUNCTION,rsge_net_ftp_client_read);
	curl_easy_setopt(client->curl,CURLOPT_READDATA,client);
	curl_easy_setopt(client->curl,CURLOPT_URL,url);
	curl_easy_setopt(client->curl,CURLOPT_INFILESIZE_LARGE,(curl_off_t)client->contentSize);

	CURLcode res = curl_easy_perform(client->curl);
	if(res != CURLE_OK) {
		log_error("curl_easy_perform() failed: %s",curl_easy_strerror(res));
		free(url);
		return RSGE_ERROR_CURL;
	}
	client->contentSize = strlen(client->content);
	free(url);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_ftp_client_download(rsge_net_ftp_client_t* client,char* path) {
	client->content = NULL;
	client->contentSize = 0;
	if(strlen(path) > 0 && path[0] == '/') path++;

	char url_tmp[1];
	size_t url_sz = sprintf(url_tmp,"%s/%s",client->url,path);
	char* url = malloc(url_sz);
	if(!url) {
		log_error("Failed to allocate %d bytes of memory",url_sz);
		return RSGE_ERROR_MALLOC;
	}
	memset(url,0,url_sz);
	sprintf(url,"%s/%s",client->url,path);
	url[url_sz] = 0;

	curl_easy_setopt(client->curl,CURLOPT_UPLOAD,0L);
	curl_easy_setopt(client->curl,CURLOPT_WRITEFUNCTION,rsge_net_ftp_client_write);
	curl_easy_setopt(client->curl,CURLOPT_WRITEDATA,client);
	curl_easy_setopt(client->curl,CURLOPT_URL,url);

	CURLcode res = curl_easy_perform(client->curl);
	if(res != CURLE_OK) {
		log_error("curl_easy_perform() failed: %s",curl_easy_strerror(res));
		free(url);
		return RSGE_ERROR_CURL;
	}

	double info_filetime = -1;
	res = curl_easy_getinfo(client->curl,CURLINFO_FILETIME,&info_filetime);
	if(res == CURLE_OK && info_filetime >= 0) {
		time_t info_file_time = (time_t)info_filetime;
		client->fileTime = ctime(&info_file_time);
	} else {
		client->fileTime = "";
	}
	free(url);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_ftp_client_disconnect(rsge_net_ftp_client_t* client) {
	client->contentSize = 0;
	free(client->content);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_ftp_client_destroy(rsge_net_ftp_client_t* client) {
	curl_easy_cleanup(client->curl);
	memset(client,0,sizeof(rsge_net_ftp_client_t));
	return RSGE_ERROR_NONE;
}

static rsge_error_e rsge_net_ftp_client_asset_find(rsge_assets_t* assets,rsge_asset_t* file,char* key) {
	rsge_net_ftp_client_t* client = (rsge_net_ftp_client_t*)assets->impl;
	rsge_error_e err = rsge_net_ftp_client_download(client,key);
	if(err != RSGE_ERROR_NONE) return err;
	file->name = key;
	file->data = client->content;
	file->size = client->contentSize;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_ftp_client_createAssets(rsge_assets_t* assets,rsge_net_ftp_client_t* client) {
	memset(assets,0,sizeof(rsge_assets_t));
	assets->impl = (void*)client;
	assets->find = rsge_net_ftp_client_asset_find;
	return RSGE_ERROR_NONE;
}
