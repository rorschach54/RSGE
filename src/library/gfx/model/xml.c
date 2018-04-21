#include <libxml/parser.h>
#include <rsge/gfx/model/xml.h>
#include <rsge/gfx/model.h>
#include <rsge/config.h>
#include <log.h>
#include <string.h>

rsge_error_e rsge_model_xml_fromFile(rsge_shape_t* shape,rsge_asset_t* asset) {
	xmlDocPtr doc = xmlReadMemory((char*)asset->data,asset->size,asset->name,NULL,0);
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

	if(!!xmlStrcmp(node->name,(const xmlChar*)"model")) {
		log_error("XML document root node is not the model type tag");
		xmlFreeDoc(doc);
		return RSGE_ERROR_LIBXML;
	}

	GLenum mode;

	int verticeCount = 0;
	int materialCount = 0;
	xmlNodePtr cur = node->xmlChildrenNode;
	while(cur != NULL) {
		if(!xmlStrcmp(cur->name,(const xmlChar*)"vertices")) {
			xmlNodePtr verticeCur = cur->xmlChildrenNode;
			while(verticeCur != NULL) {
				verticeCount++;
				verticeCur = verticeCur->next;
			}
		}
		if(!xmlStrcmp(cur->name,(const xmlChar*)"materials")) {
			xmlNodePtr materialCur = cur->xmlChildrenNode;
			while(materialCur != NULL) {
				materialCount++;
				materialCur = materialCur->next;
			}
		}
		if(!xmlStrcmp(cur->name,(const xmlChar*)"mode")) {
			char* value = (char*)xmlGetProp(cur,"value");
			if(!value) {
				log_error("mode tag is missing the value property");
				return RSGE_ERROR_LIBXML;
			}
			if(!strcmp(value,"GL_POINTS")) mode = GL_POINTS;
			else if(!strcmp(value,"GL_LINES")) mode = GL_LINES;
			else if(!strcmp(value,"GL_LINE_STRIP")) mode = GL_LINE_STRIP;
			else if(!strcmp(value,"GL_LINE_LOOP")) mode = GL_LINE_LOOP;
			else if(!strcmp(value,"GL_TRIANGLES")) mode = GL_TRIANGLES;
			else if(!strcmp(value,"GL_TRIANGLE_STRIP")) mode = GL_TRIANGLE_STRIP;
			else if(!strcmp(value,"GL_TRIANGLE_FAN")) mode = GL_TRIANGLE_FAN;
			else if(!strcmp(value,"GL_QUADS")) mode = GL_QUADS;
			else if(!strcmp(value,"GL_QUAD_STRIP")) mode = GL_QUAD_STRIP;
			else if(!strcmp(value,"GL_POLYGON")) mode = GL_POLYGON;
			else {
				log_error("mode tag has an invalid value, %s",value);
				return RSGE_ERROR_LIBXML;
			}
		}
		cur = cur->next;
	}

	rsge_shape_vert_t* vertices = malloc(sizeof(rsge_shape_vert_t)*verticeCount);
	if(vertices == NULL) {
		log_error("Failed to allocate %d bytes of memory.",sizeof(rsge_shape_vert_t)*verticeCount);
		xmlFreeDoc(doc);
		return RSGE_ERROR_MALLOC;
	}

	rsge_shape_mat_t* materials = malloc(sizeof(rsge_shape_mat_t)*materialCount);
	if(materials == NULL && materialCount > 0) {
		log_error("Failed to allocate %d bytes of memory.",sizeof(rsge_shape_mat_t)*materialCount);
		xmlFreeDoc(doc);
		free(vertices);
		return RSGE_ERROR_MALLOC;
	}

	node = xmlDocGetRootElement(doc);

	cur = node->xmlChildrenNode;
	while(cur != NULL) {
		if(!xmlStrcmp(cur->name,(const xmlChar*)"vertices")) {
			xmlNodePtr verticeCur = cur->xmlChildrenNode;
			for(int i = 0;i < verticeCount,verticeCur != NULL;i++) {
				rsge_shape_vert_t* vert = (rsge_shape_vert_t*)&vertices[i];

				if(!!xmlStrcmp(verticeCur->name,(const xmlChar*)"vertice")) {
					log_error("Invalid tag %s, expecting vertice tag",(char*)verticeCur->name);
					free(vertices);
					if(materials != NULL) free(materials);
					xmlFreeDoc(doc);
					return RSGE_ERROR_LIBXML;
				}

				char* position = (char*)xmlGetProp(verticeCur,"position");
				if(!position) {
					log_error("vertex tag is missing position property");
					free(vertices);
					if(materials != NULL) free(materials);
					xmlFreeDoc(doc);
					return RSGE_ERROR_LIBXML;
				}
				int xi;
				for(xi = 0;xi < strlen(position);xi++) {
					if(position[xi] == ',') break;
				}
				if(xi+1 == strlen(position)) {
					free(vertices);
					if(materials != NULL) free(materials);
					xmlFreeDoc(doc);
					return RSGE_ERROR_LIBXML;
				}
				int yi;
				for(yi = (xi+1);yi < strlen(position);yi++) {
					if(position[yi] == ',') break;
				}
				if(yi+1 == strlen(position)) {
					free(vertices);
					if(materials != NULL) free(materials);
					xmlFreeDoc(doc);
					return RSGE_ERROR_LIBXML;
				}

				int xsz = xi-1;
				int zsz = strlen(position)-((yi+1)+xi);
				int ysz = strlen(position)-zsz;

				printf("%d,%d,%d\n",xsz,ysz,zsz);
				verticeCur = verticeCur->next;
			}
		}
		if(!xmlStrcmp(cur->name,(const xmlChar*)"materials")) {
			xmlNodePtr materialCur = cur->xmlChildrenNode;
			for(int i = 0;i < materialCount,materialCur != NULL;i++) {
				rsge_shape_mat_t* material = (rsge_shape_mat_t*)&materials[i];
				materialCur = materialCur->next;
			}
		}
		cur = cur->next;
	}
	xmlFreeDoc(doc);
	return rsge_shape_create(shape,mode,vertices,verticeCount,materials,materialCount);
}