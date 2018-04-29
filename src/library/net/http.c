#include <libxml/parser.h>
#include <rsge/net/http.h>
#include <rsge/assets.h>
#include <log.h>
#include <string.h>

static size_t rsge_net_http_client_write(void* buffer,size_t size,size_t nmemb,void* userdata) {
	rsge_net_http_client_t* client = (rsge_net_http_client_t*)userdata;
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

static size_t rsge_net_http_client_headerswrite(void* buffer,size_t size,size_t nmemb,void* userdata) {
	rsge_net_http_client_t* client = (rsge_net_http_client_t*)userdata;
	size_t totalSize = size*nmemb;
	if(totalSize > 2) {
		if(!strncmp((char*)buffer,"HTTP",4)) {
			char* statusFull = strstr((char*)buffer," ");
			statusFull++;

			char* statusStr = strstr(statusFull," ");
			statusStr++;

			char* statusCodeStr = statusFull;
			statusCodeStr[strlen(statusFull)-strlen(statusStr)] = 0;
			client->status = atoi(statusCodeStr);
		} else {
			while(((char*)buffer)[totalSize-1] == '\r' || ((char*)buffer)[totalSize-1] == '\n') totalSize--;
			char* header = malloc(totalSize);
			if(!header) return 0;
			memset(header,0,totalSize);
			memcpy(header,buffer,totalSize);
			header[totalSize] = 0;
			list_node_t* node = list_node_new(header);
			if(!node) {
				free(header);
				return 0;
			}
			list_rpush(client->tmpHeaders,node);
		}
	}
	return size*nmemb;
}

rsge_error_e rsge_net_http_client_create(rsge_net_http_client_t* client) {
	memset(client,0,sizeof(rsge_net_http_client_t));

	/* Setup headers */
	client->defaultHeaderCount = 1;
	client->defaultHeaders = malloc(sizeof(rsge_net_http_header_t)*client->defaultHeaderCount);
	if(!client->defaultHeaders) {
		log_error("Failed to allocate %d bytes",sizeof(rsge_net_http_header_t)*client->defaultHeaderCount);
		return RSGE_ERROR_MALLOC;
	}

	client->defaultHeaders[0].name = "User-agent";
	client->defaultHeaders[0].value = "RSGE";

	/* Setup CURL */
	client->curl = curl_easy_init();
	if(!client->curl) {
		free(client->defaultHeaders);
		return RSGE_ERROR_CURL;
	}
	curl_easy_setopt(client->curl,CURLOPT_FOLLOWLOCATION,1L);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_http_client_connect(rsge_net_http_client_t* client,char* url,rsge_net_http_method_e method) {
	client->content = NULL;
	client->contentSize = 0;

	client->headers = NULL;
	client->headerCount = 0;
	client->tmpHeaders = list_new();
	if(!client->tmpHeaders) return RSGE_ERROR_MALLOC;

	curl_easy_setopt(client->curl,CURLOPT_WRITEFUNCTION,rsge_net_http_client_write);
	curl_easy_setopt(client->curl,CURLOPT_WRITEDATA,client);

	curl_easy_setopt(client->curl,CURLOPT_HEADERFUNCTION,rsge_net_http_client_headerswrite);
	curl_easy_setopt(client->curl,CURLOPT_HEADERDATA,client);

	if(method == RSGE_HTTP_METHOD_GET) {
		curl_easy_setopt(client->curl,CURLOPT_HTTPGET,1L);
	} else if(method == RSGE_HTTP_METHOD_POST) {
		curl_easy_setopt(client->curl,CURLOPT_POST,1L);
	} else if(method == RSGE_HTTP_METHOD_PUT) {
		curl_easy_setopt(client->curl,CURLOPT_PUT,1L);
	} else {
		log_error("Invalid or unsupported HTTP method: %d",method);
		return RSGE_ERROR_INVALID_HTTP_METHOD;
	}
	curl_easy_setopt(client->curl,CURLOPT_URL,url);

	struct curl_slist* headers = NULL;
	char header_tmp[1];
	for(size_t i = 0;i < client->defaultHeaderCount;i++) {
		size_t header_sz = sprintf(header_tmp,"%s: %s",client->defaultHeaders[i].name,client->defaultHeaders[i].value);
		char* header = malloc(header_sz);
		if(!header) {
			log_error("Failed to allocate %d bytes",header_sz);
			return RSGE_ERROR_MALLOC;
		}
		memset(header,0,header_sz);
		sprintf(header,"%s: %s",client->defaultHeaders[i].name,client->defaultHeaders[i].value);
		headers = curl_slist_append(headers,header);
		free(header);
	}

	curl_easy_setopt(client->curl,CURLOPT_HTTPHEADER,headers);

	CURLcode res = curl_easy_perform(client->curl);
	if(res != CURLE_OK) {
		log_error("curl_easy_perform() failed: %s",curl_easy_strerror(res));
		curl_slist_free_all(headers);
		return RSGE_ERROR_CURL;
	}

	client->headerCount = 0;

	list_iterator_t* headers_it = list_iterator_new(client->tmpHeaders,LIST_HEAD);
	if(!headers_it) {
		curl_slist_free_all(headers);
		list_destroy(client->tmpHeaders);
		return RSGE_ERROR_MALLOC;
	}

	list_node_t* header_node;
	while((header_node = list_iterator_next(headers_it))) {
		char* headerStr = (char*)header_node->val;
		char* value = strstr(headerStr,": ");
		if(!value) value = strstr(headerStr,":");
		else value++;
		if(!value) continue;
		value++;
		client->headerCount++;
	}
	list_iterator_destroy(headers_it);

	client->headers = malloc(sizeof(rsge_net_http_header_t)*client->headerCount);
	if(!client->headers) {
		curl_slist_free_all(headers);
		list_destroy(client->tmpHeaders);
		return RSGE_ERROR_MALLOC;
	}

	headers_it = list_iterator_new(client->tmpHeaders,LIST_HEAD);
	if(!headers_it) {
		free(client->headers);
		curl_slist_free_all(headers);
		list_destroy(client->tmpHeaders);
		return RSGE_ERROR_MALLOC;
	}

	size_t headerIndex = 0;
	while((header_node = list_iterator_next(headers_it))) {
		char* headerStr = (char*)header_node->val;
		int nameOff = 1;
		char* value = strstr(headerStr,": ");
		if(!value) value = strstr(headerStr,":");
		else nameOff++;
		if(!value) continue;
		value += nameOff;

		rsge_net_http_header_t* header = (rsge_net_http_header_t*)&client->headers[headerIndex++];
		memset(header,0,sizeof(rsge_net_http_header_t));

		header->value = value;

		header->name = headerStr;
		header->name[strlen(headerStr)-strlen(value)-nameOff] = 0;
	}
	list_iterator_destroy(headers_it);

	curl_slist_free_all(headers);
	list_destroy(client->tmpHeaders);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_http_client_disconnect(rsge_net_http_client_t* client) {
	client->status = 0;
	client->contentSize = 0;
	client->headerCount = 0;
	free(client->content);
	free(client->headers);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_http_client_destroy(rsge_net_http_client_t* client) {
	curl_easy_cleanup(client->curl);
	free(client->defaultHeaders);
	memset(client,0,sizeof(rsge_net_http_client_t));
	return RSGE_ERROR_NONE;
}
