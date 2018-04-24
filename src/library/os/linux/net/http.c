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
} rsge_net_http_client_impl_t;

extern rsge_error_e rsge_net_http_client_parseheaders(rsge_net_http_client_t* client,char* header);

rsge_error_e rsge_net_http_client_create(rsge_net_http_client_t* client,rsge_net_http_client_cfg_t* cfg) {
	memset(client,0,sizeof(rsge_net_http_client_t));

	rsge_net_http_client_impl_t* impl = malloc(sizeof(rsge_net_http_client_impl_t));
	if(!impl) return RSGE_ERROR_MALLOC;

	if(cfg == NULL) {
		rsge_net_http_client_cfg_t tmp_cfg;
		rsge_error_e err = rsge_net_http_clientcfg_fromFile(&tmp_cfg,"rsge@net/http/client/config.xml");
		if(err != RSGE_ERROR_NONE) {
			free(impl);
			return err;
		}
		cfg = &tmp_cfg;
	}
	client->cfg = cfg;
	client->impl = (void*)impl;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_http_client_connect(rsge_net_http_client_t* client,char* url,int port,char* mode) {
	client->headerCount = 0;
	rsge_net_http_client_impl_t* impl = (rsge_net_http_client_impl_t*)client->impl;

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
	char* ip = url;
	
	struct hostent* ghbn = gethostbyname(ip);
	if(ghbn != NULL) {
		int found = 0;
		struct in_addr inaddr;
		for(int i = 0;i < ghbn->h_length;i++) {
			ip = inet_ntoa(*(struct in_addr*)ghbn->h_addr_list[i]);
			if(ip == NULL) continue;
			log_debug("IP: %s",ip);
			found = 1;
			break;
		}
		if(!found) return RSGE_ERROR_OS;
	}

	char* ptr;
	char* host;
	char* getrequest;
	char path[1000];
	char buffer[1024];
	struct sockaddr_in addr;
	
	int getrequestsz = 0;
	
	// TODO: fix it to work a lot better
	int headerstrsz = 0;
	for(int i = 0;i < client->cfg->headerCount;i++) {
		log_debug("Adding header: %s: %s",client->cfg->headers[i].name,client->cfg->headers[i].value);
		headerstrsz += strlen(client->cfg->headers[i].name)+strlen(": ")+strlen(client->cfg->headers[i].value)+strlen("\n");
	}
	char* headersstr = malloc(headerstrsz);
	if(!headersstr) return RSGE_ERROR_MALLOC;
	int headeroff = 0;
	for(int i = 0;i < client->cfg->headerCount;i++) {
		headeroff += sprintf(headersstr+headeroff,"%s: %s\n",client->cfg->headers[i].name,client->cfg->headers[i].value);
	}
	
	char* request_get;
	int request_getsz;
	
	if(inet_pton(AF_INET,url,&(addr.sin_addr)) != 0) {
		char reqtmp[1];
		strcpy(path,"/");
		getrequestsz = sprintf(reqtmp,"HEAD / HTTP/1.0\nHOST: %s\n%s\n\n",url,headersstr);
		getrequest = malloc(getrequestsz);
		if(!getrequest) {
			free(headersstr);
			return RSGE_ERROR_MALLOC;
		}
		sprintf(getrequest,"HEAD / HTTP/1.0\nHOST: %s\n%s\n\n",url,headersstr);
		request_getsz = sprintf(reqtmp,"HEAD / HTTP/1.0\nHOST: %s\n%s\n\n",url,headersstr);
	} else {
		strcpy(path,"/");
		char reqtmp[1];
		if((ptr = strstr(url,"/")) == NULL) {
			getrequestsz = sprintf(reqtmp,"HEAD / HTTP/1.0\nHOST: %s\n%s\n\n",url,headersstr);
			getrequest = malloc(getrequestsz);
			if(!getrequest) {
				free(headersstr);
				return RSGE_ERROR_MALLOC;
			}
			sprintf(getrequest,"HEAD / HTTP/1.0\nHOST: %s\n%s\n\n",url,headersstr); 
		} else {
			strcpy(path,ptr);
			host = strtok(url,"/");
			getrequestsz = sprintf(reqtmp,"HEAD %s HTTP/1.0\nHOST: %s\n%s\n\n",path,url,headersstr);
			getrequest = malloc(getrequestsz);
			if(!getrequest) {
				free(headersstr);
				return RSGE_ERROR_MALLOC;
			}
			sprintf(getrequest,"HEAD %s HTTP/1.0\nHOST: %s\n%s\n\n",path,url,headersstr);
		}
	}

	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0) {
		log_error("Failed to create a socket");
		return RSGE_ERROR_OS;
	}

	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);

	log_debug("Connecting to %s",url);
	if(connect(sockfd,(struct sockaddr*)&addr,sizeof(addr)) < 0) {
		log_error("Socket failed to connect to %s (%s)",url,ip);
		free(getrequest);
		free(headersstr);
		return RSGE_ERROR_OS;
	}

	log_debug("Sending request");
	write(sockfd,getrequest,strlen(getrequest));

	log_debug("Receiving status");
	memset(&buffer,0,sizeof(buffer));
	int header_off = 0;
	int ret = recv(sockfd,buffer,1024,0);
	if(ret < 0) {
		free(getrequest);
		free(headersstr);
		close(sockfd);
		return RSGE_ERROR_OS;
	} else {
		log_debug("Downloaded: %s",buffer);
		if((temp = strstr(buffer,"HTTP/1.0 200 OK")) != NULL || (temp = strstr(buffer,"HTTP/1.0 301 Moved Permanently")) || (temp = strstr(buffer,"HTTP/1.0 200 OK")) != NULL || (temp = strstr(buffer,"HTTP/1.0 301 Moved Permanently"))) {
			send(sockfd,"OK",strlen("OK"),0);
			header_off = strlen(temp);
		} else {
			log_error("Bad status: %s",buffer);
			close(sockfd);
			free(getrequest);
			free(headersstr);
			return RSGE_ERROR_OS;
		}
	}

	log_debug("Loading headers");
	//memset(&buffer,0,sizeof(buffer));
	//ret = recv(sockfd,buffer,1024,0);
	if(ret < 0) {
		close(sockfd);
		free(getrequest);
		free(headersstr);
		return RSGE_ERROR_OS;
	} else {
		rsge_error_e err = rsge_net_http_client_parseheaders(client,buffer+header_off);
		if(err != RSGE_ERROR_NONE) {
			free(getrequest);
			free(headersstr);
			close(sockfd);
			return err;
		}
		//send(sockfd,"OK",strlen("OK"),0);
	}
	log_debug("Loaded headers");
	rsge_net_http_header_t* header_ContentLength = &client->headers[1];

	log_debug("Downloading %d bytes (%s)",atoi(header_ContentLength->value),header_ContentLength->value);
	
	client->content = malloc(atoi(header_ContentLength->value)+237);
	if(!client->content) {
		log_error("Failed to allocate %d bytes of memory",atoi(header_ContentLength->value)+237);
		free(getrequest);
		free(headersstr);
		return RSGE_ERROR_MALLOC;
	}
	memset(client->content,0,atoi(header_ContentLength->value)+237);
	char tmpdownreq[1];
	int reqstrsz = sprintf(tmpdownreq,"%s %s\n",mode,path);
	char* reqstr = malloc(reqstrsz);
	if(!reqstrsz) {
		free(client->content);
		free(getrequest);
		free(headersstr);
		close(sockfd);
		return RSGE_ERROR_MALLOC;
	}
	sprintf(reqstr,"%s %s\n",mode,path);
	send(sockfd,reqstr,strlen(reqstr),0);
	ret = recv(sockfd,client->content,atoi(header_ContentLength->value)+237,0);
	if(ret < 0) {
		free(client->content);
		free(getrequest);
		free(headersstr);
		free(reqstr);
		close(sockfd);
		return RSGE_ERROR_OS;
	}
	log_debug("%s",client->content);
	client->content += 237;
	//send(sockfd,"OK",strlen("OK"),0);
	
	log_debug("Downloaded page");

	close(sockfd);
	free(getrequest);
	free(headersstr);
	free(reqstr);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_http_client_disconnect(rsge_net_http_client_t* client) {
	free(client->headers);
	free(client->content);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_http_client_destroy(rsge_net_http_client_t* client) {
	rsge_net_http_client_impl_t* impl = (rsge_net_http_client_impl_t*)client->impl;
	free(impl);
	memset(client,0,sizeof(rsge_net_http_client_t));
	return RSGE_ERROR_NONE;
}