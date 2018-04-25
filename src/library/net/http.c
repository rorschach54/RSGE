#include <libxml/parser.h>
#include <rsge/net/http.h>
#include <rsge/assets.h>
#include <log.h>
#include <string.h>

rsge_error_e rsge_net_http_client_parseheaders(rsge_net_http_client_t* client,char* header) {
	char temp[100];
	char* line = strtok(header,"\n");
	char* key;
	char* value;
	int i = 0;
	client->headerCount = 2;
	client->headers = malloc(sizeof(rsge_net_http_header_t)*client->headerCount);
	if(!client->headers) {
		return RSGE_ERROR_MALLOC;
	}

	rsge_net_http_header_t* header_ContentType = &client->headers[0];
	memset(header_ContentType,0,sizeof(rsge_net_http_header_t));
	header_ContentType->name = "Content-Type";
	header_ContentType->value = "";
	
	rsge_net_http_header_t* header_ContentLength = &client->headers[1];
	memset(header_ContentLength,0,sizeof(rsge_net_http_header_t));
	header_ContentLength->name = "Content-Length";
	header_ContentLength->value = "";

	while(line != NULL) {
		if(strlen(line) == 1) break;
		strcpy(temp,line);
		log_debug("Parsing header line: %s",line);
		char* tmp;
		for(int a = 0;a < client->headerCount;a++) {
			if((tmp = strstr(line,client->headers[a].name)) != NULL) {
				tmp += strlen(client->headers[a].name)+strlen(": ");
				client->headers[a].value = tmp;
				break;
			}
		}
		line = strtok(NULL,"\n");
		i++;
	}
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_net_http_clientcfg_fromFile(rsge_net_http_client_cfg_t* cfg,char* path) {
	char* asset_data;
	size_t asset_size;
	rsge_error_e err = rsge_asset_read(path,&asset_data,&asset_size);
	if(err != RSGE_ERROR_NONE) {
		log_error("Failed to read asset");
		return err;
	}
	xmlDocPtr doc = xmlReadMemory(asset_data,asset_size,path,NULL,0);
	if(doc == NULL) {
		log_error("Cannot parse XML document");
		return RSGE_ERROR_LIBXML;
	}
	xmlNodePtr node = xmlDocGetRootElement(doc);
	if(node == NULL) {
		log_error("Cannot get root XML node");
		xmlFreeDoc(doc);
		return RSGE_ERROR_LIBXML;
	}

	if(!!xmlStrcmp(node->name,(const xmlChar*)"httpClientConfig")) {
		log_error("XML document root node is not the httpClientConfig tag");
		xmlFreeDoc(doc);
		return RSGE_ERROR_LIBXML;
	}

	xmlNodePtr cur = node->children;
	while(cur != NULL) {
		if(!xmlStrcmp(cur->name,(const xmlChar*)"headers")) {
			cfg->headerCount = 0;
			xmlNodePtr headerCur = cur->children;
			while(headerCur != NULL) {
				if(headerCur->type == XML_ELEMENT_NODE && !xmlStrcmp(headerCur->name,(const xmlChar*)"header")) {
					cfg->headerCount++;
				}
				headerCur = headerCur->next;
			}

			cfg->headers = malloc(sizeof(rsge_net_http_header_t)*cfg->headerCount);
			if(!cfg->headers) {
				log_error("Failed to allocate %d bytes of memory",sizeof(rsge_net_http_header_t)*cfg->headerCount);
				xmlFreeDoc(doc);
				return RSGE_ERROR_MALLOC;
			}

			int i = 0;
			headerCur = cur->children;
			while(headerCur != NULL) {
				if(headerCur->type == XML_ELEMENT_NODE && !xmlStrcmp(headerCur->name,(const xmlChar*)"header")) {
					rsge_net_http_header_t* header = &cfg->headers[i];
					memset(header,0,sizeof(rsge_net_http_header_t));

					header->name = (char*)xmlGetProp(headerCur,"name");
					if(!header->name) {
						xmlFreeDoc(doc);
						free(cfg->headers);
						return RSGE_ERROR_LIBXML;
					}

					header->value = (char*)xmlGetProp(headerCur,"value");
					if(!header->value) {
						xmlFreeDoc(doc);
						free(cfg->headers);
						return RSGE_ERROR_LIBXML;
					}

					i++;
				}
				headerCur = headerCur->next;
			}
		}
		cur = cur->next;
	}
	xmlFreeDoc(doc);
	return RSGE_ERROR_NONE;
}