#include <arpa/inet.h>
#include <netinet/in.h>
#include <rsge/net/http.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <list.h>
#include <log.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
	int sockfd;
	int packetCount;
} rsge_net_http_client_impl_t;

extern rsge_error_e rsge_net_http_client_parseheaders(rsge_net_http_client_t* client,char* header);

rsge_error_e rsge_net_http_client_create(rsge_net_http_client_t* client,rsge_net_http_client_cfg_t* cfg) {
	if(cfg == NULL) {
		rsge_net_http_client_cfg_t tmp_cfg;
		rsge_error_e err = rsge_net_http_clientcfg_fromFile(&tmp_cfg,"rsge@net/http/client/config.xml");
		if(err != RSGE_ERROR_NONE) return err;
		return rsge_net_http_client_create(client,&tmp_cfg);
	}
	memset(client,0,sizeof(rsge_net_http_client_t));

	rsge_net_http_client_impl_t* impl = malloc(sizeof(rsge_net_http_client_impl_t));
	if(!impl) return RSGE_ERROR_MALLOC;
	client->cfg = cfg;
	client->impl = (void*)impl;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_http_client_connect(rsge_net_http_client_t* client,char* url,int port,char* method) {
	client->headerCount = 0;
	rsge_error_e err;
	rsge_net_http_client_impl_t* impl = (rsge_net_http_client_impl_t*)client->impl;
	impl->packetCount = 0;

	char* temp;

	if(port == 0) {
		if((temp = strstr(url,"http://")) != NULL) {
			url += 7;
			port = 80;
		} else if((temp = strstr(url,"https://")) != NULL) {
			url += 8;
			port = 443;
		}
	}

	client->host = url;
	
	struct hostent* ghbn = gethostbyname(url);
	if(ghbn != NULL) {
		int found = 0;
		struct in_addr inaddr;
		for(int i = 0;i < ghbn->h_length;i++) {
			client->ip = inet_ntoa(*(struct in_addr*)ghbn->h_addr_list[i]);
			if(client->ip == NULL) continue;
			log_debug("IP: %s",client->ip);
			found = 1;
			break;
		}
		if(!found) return RSGE_ERROR_OS;
	}

	client->port = port;

	char* ptr;
	char* getrequest;
	char buffer[1024];
	struct sockaddr_in addr;
	
	if(inet_pton(AF_INET,url,&(addr.sin_addr)) != 0) {
		char reqtmp[1];
		client->path = "/";
	} else {
		client->path = "/";
		char reqtmp[1];
		if((ptr = strstr(url,"/")) != NULL) {
			client->path = malloc(strlen(ptr));
			if(!client->path) return RSGE_ERROR_MALLOC;
			strcpy(client->path,ptr);
			char* host = strtok(url,"/");
		}
	}

	impl->sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(impl->sockfd < 0) {
		log_error("Failed to create a socket");
		return RSGE_ERROR_OS;
	}

	log_debug("Linux socket fd: %d",impl->sockfd);

	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(client->ip);
	addr.sin_port = htons(client->port);

	log_debug("Connecting to %s",url);
	if(connect(impl->sockfd,(struct sockaddr*)&addr,sizeof(addr)) < 0) {
		log_error("Socket failed to connect to %s (%s)",url,client->ip);
		return RSGE_ERROR_OS;
	}

	err = rsge_net_http_client_exec(client,method);
	if(err != RSGE_ERROR_NONE) {
		close(impl->sockfd);
		return err;
	}

	log_debug("Receiving status");
	memset(&buffer,0,sizeof(buffer));
	int header_off = 0;
	int ret = recv(impl->sockfd,buffer,1024,0);
	if(ret < 0) {
		close(impl->sockfd);
		return RSGE_ERROR_OS;
	} else {
		if((temp = strstr(buffer,"HTTP/1.0 200 OK")) != NULL || (temp = strstr(buffer,"HTTP/1.0 301 Moved Permanently")) || (temp = strstr(buffer,"HTTP/1.1 200 OK")) != NULL || (temp = strstr(buffer,"HTTP/1.1 301 Moved Permanently"))) {
		} else {
			log_error("Bad status: %s",buffer);
			close(impl->sockfd);
			return RSGE_ERROR_OS; // TODO: use RSGE_ERROR_HTTP_BAD_STATUS
		}
	}

	log_debug("Loading headers");
	err = rsge_net_http_client_parseheaders(client,buffer);
	if(err != RSGE_ERROR_NONE) {
		close(impl->sockfd);
		return err;
	}
	rsge_net_http_header_t* header_ContentLength = &client->headers[1];
	int header_ContentLength_v = atoi(header_ContentLength->value);
	log_debug("Loaded headers");

	log_debug("Downloading %d bytes",header_ContentLength_v);

	client->content = malloc(header_ContentLength_v);
	if(!client->content) {
		log_error("Failed to allocate %d bytes of memory",header_ContentLength_v);
		return RSGE_ERROR_MALLOC;
	}
	memset(client->content,0,header_ContentLength_v);

	err = rsge_net_http_client_exec(client,method);
	if(err != RSGE_ERROR_NONE) {
		free(client->content);
		close(impl->sockfd);
		return err;
	}

	ret = recv(impl->sockfd,client->content,header_ContentLength_v,0);
	if(ret < 0) {
		free(client->content);
		close(impl->sockfd);
		return RSGE_ERROR_OS;
	}
	
	log_debug("Downloaded %d bytes",strlen(client->content));
	if(strlen(client->content) != header_ContentLength_v) {
		log_error("Failed to download %d bytes, only downloaded %d/%d bytes",strlen(client->content),strlen(client->content),header_ContentLength_v);
		free(client->content);
		close(impl->sockfd);
		return RSGE_ERROR_OS;
	}
	
	for(int i = 0;i < strlen(client->content);i++) {
		if(client->content[i] == '<') break;
		client->content++;
	}
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_http_client_exec(rsge_net_http_client_t* client,char* method) {
	rsge_net_http_client_impl_t* impl = (rsge_net_http_client_impl_t*)client->impl;
	log_debug("Linux socket fd: %d",impl->sockfd);
	char request[256];
	memset(request,0,sizeof(request));
	sprintf(request,"%s %s HTTP/1.1\nHost: %s\nConnection: keep-alive\n\n",method,client->path,client->host);
	log_debug("Sending request: %s",request);
	if(impl->packetCount == 0) write(impl->sockfd,request,strlen(request));
	else send(impl->sockfd,request,strlen(request),0);
	impl->packetCount++;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_http_client_disconnect(rsge_net_http_client_t* client) {
	rsge_net_http_client_impl_t* impl = (rsge_net_http_client_impl_t*)client->impl;
	free(client->headers);
	free(client->content);
	close(impl->sockfd);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_http_client_destroy(rsge_net_http_client_t* client) {
	rsge_net_http_client_impl_t* impl = (rsge_net_http_client_impl_t*)client->impl;
	free(impl);
	memset(client,0,sizeof(rsge_net_http_client_t));
	return RSGE_ERROR_NONE;
}