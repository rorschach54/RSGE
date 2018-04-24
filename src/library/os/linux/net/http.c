#include <arpa/inet.h>
#include <netinet/in.h>
#include <rsge/net/http.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <list.h>
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

	if(port == 0) {
		if((temp = strstr(url,"http://")) != NULL) {
			url += 7;
			port = 80;
		} else if((temp = strstr(url,"https://")) != NULL) {
			url += 8;
			port = 443;
		}
	}

	char* ptr;
	char* host;
	char* temp;
	char getrequest[1024];
	char path[1000];
	char buffer[1024];
	struct sockaddr_in addr;
	if(inet_pton(AF_INET,url,&(addr.sin_addr)) != 0) {
		sprintf(getrequest,"GET / HTTP/1.0\nHOST: %s\n\n",url);
	} else {
		if((ptr = strstr(url,"/")) == NULL) {
			sprintf(getrequest,"%s / HTTP/1.0\nHOST: %s\n\n",mode,url); 
		} else {
			strcpy(path,ptr);
			host = strtok(url,"/");
			sprintf(getrequest,"%s %s HTTP/1.0\nHOST: %s\n\n",mode,path,url);
		}
	}

	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0) return RSGE_ERROR_OS;

	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(url);
	addr.sin_port = htons(port);

	if(connect(sockfd,(struct sockaddr*)&addr,sizeof(addr)) < 0) return RSGE_ERROR_OS;

	write(sockfd,getrequest,strlen(getrequest));

	memset(&buffer,0,sizeof(buffer));
	int ret = recv(sockfd,buffer,1024,0);
	if(ret < 0) {
		close(sockfd);
		return RSGE_ERROR_OS;
	} else {
		if((temp = strstr(buffer,"HTTP/1.0 200 OK")) != NULL) {
			send(sockfd,"OK",strlen("OK"),0);
		} else {
			close(sockfd);
			return RSGE_ERROR_OS;
		}
	}

	memset(&buffer,0,sizeof(buffer));
	ret = recv(sockfd,buffer,1024,0);
	if(ret < 0) {
		close(sockfd);
		return RSGE_ERROR_OS;
	} else {
		rsge_error_e err = rsge_net_http_client_parseheaders(client,buffer);
		if(err != RSGE_ERROR_NONE) {
			close(sockfd);
			return err;
		}
		send(sockfd,"OK",strlen("OK"),0);
	}

	list_t* tmp_buffs = list_new();
	if(!tmp_buffs) {
		close(sockfd);
		return RSGE_ERROR_MALLOC;
	}

	memset(&buffer,0,sizeof(buffer));
	while(recv(sockfd,buffer,1024,0) > 0) {
		list_node_t* node = list_node_new(buffer);
		if(!node) {
			close(sockfd);
			list_destroy(tmp_buffs);
			return RSGE_ERROR_MALLOC;
		}
		list_rpush(tmp_buffs,node);
		memset(&buffer,0,sizeof(buffer));
	}

	int buff_sz = 0;
	list_node_t* node;
	list_iterator_t* it = list_iterator_new(tmp_buffs,LIST_HEAD);
	if(it == NULL) {
		close(sockfd);
		list_destroy(tmp_buffs);
		return RSGE_ERROR_MALLOC;
	}
	while((node = list_iterator_next(it))) {
		char* tmp_buff = (char*)node->val;
		buff_sz += strlen(tmp_buff);
	}
	list_iterator_destroy(it);

	client->content = malloc(sizeof(char)*buff_sz);
	if(!client->content) {
		close(sockfd);
		list_destroy(tmp_buffs);
		return RSGE_ERROR_MALLOC;
	}

	int off = 0;
	it = list_iterator_new(tmp_buffs,LIST_HEAD);
	if(it == NULL) {
		close(sockfd);
		list_destroy(tmp_buffs);
		return RSGE_ERROR_MALLOC;
	}
	while((node = list_iterator_next(it))) {
		char* tmp_buff = (char*)node->val;
		memcpy(client->content+off,tmp_buff,strlen(tmp_buff));
		off += strlen(tmp_buff);
	}
	list_iterator_destroy(it);

	close(sockfd);
	list_destroy(tmp_buffs);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_http_client_disconnect(rsge_net_http_client_t* client) {
	free(client->headers);
	free(client->content);
	return RSGE_ERROR_NONE;
}