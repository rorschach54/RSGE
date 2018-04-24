#include <libxml/parser.h>
#include <rsge/gfx/model/xml.h>
#include <rsge/gfx/image.h>
#include <rsge/gfx/model.h>
#include <rsge/config.h>
#include <log.h>
#include <stdbool.h>
#include <string.h>

rsge_error_e rsge_model_xml_fromFile(rsge_shape_t* shape,char* path) {
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

	if(!!xmlStrcmp(node->name,(const xmlChar*)"model")) {
		log_error("XML document root node is not the model type tag");
		xmlFreeDoc(doc);
		return RSGE_ERROR_LIBXML;
	}

	GLenum mode;

	uint8_t shape_flags = 0;

	int verticeCount = 0;
	int materialCount = 0;
	xmlNodePtr cur = node->children;
	while(cur != NULL) {
		if(!xmlStrcmp(cur->name,(const xmlChar*)"enable")) {
			char* value = (char*)xmlGetProp(cur,"value");
			if(!value) {
				log_error("enable tag is missing the value property");
				return RSGE_ERROR_LIBXML;
			}

			if(!strcmp(value,"shadows")) shape_flags |= RSGE_SHAPE_FLAG_SHADOWS;
			else return RSGE_ERROR_LIBXML;
		}
		if(!xmlStrcmp(cur->name,(const xmlChar*)"vertices")) {
			xmlNodePtr verticeCur = cur->children;
			while(verticeCur != NULL) {
				if(verticeCur->type == XML_ELEMENT_NODE) verticeCount++;
				verticeCur = verticeCur->next;
			}
		}
		if(!xmlStrcmp(cur->name,(const xmlChar*)"materials")) {
			xmlNodePtr materialCur = cur->children;
			while(materialCur != NULL) {
				if(materialCur->type == XML_ELEMENT_NODE) materialCount++;
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

	cur = node->children;
	while(cur != NULL) {
		if(!xmlStrcmp(cur->name,(const xmlChar*)"vertices")) {
			xmlNodePtr verticeCur = cur->children;
			int i = 0;
			while(verticeCur != NULL) {
				if(verticeCur->type == XML_ELEMENT_NODE) {
					rsge_shape_vert_t* vert = (rsge_shape_vert_t*)&vertices[i];
					memset(vert,0,sizeof(rsge_shape_vert_t));
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

					int pos_ystart = 0;
					for(int a = 0;a < strlen(position);a++) {
						if(position[a] == ',') {
							pos_ystart = a+1;
							break;
						}
					}

					if(pos_ystart == 0) {
						log_error("vertex tag has a malformed position");
						free(vertices);
						if(materials != NULL) free(materials);
						xmlFreeDoc(doc);
						return RSGE_ERROR_LIBXML;
					}

					int pos_zstart = 0;
					for(int a = pos_ystart;a < strlen(position);a++) {
						if(position[a] == ',') {
							pos_zstart = a+1;
							break;
						}
					}

					if(pos_zstart == 0) {
						log_error("vertex tag has a malformed position");
						free(vertices);
						if(materials != NULL) free(materials);
						xmlFreeDoc(doc);
						return RSGE_ERROR_LIBXML;
					}

					int pos_xend = pos_ystart-1;
					int pos_yend = pos_zstart-1;
					int pos_zend = strlen(position);

					char* pos_xstr = malloc(pos_xend);
					if(!pos_xstr) {
						free(vertices);
						if(materials != NULL) free(materials);
						xmlFreeDoc(doc);
						return RSGE_ERROR_MALLOC;
					}
					memset(pos_xstr,0,pos_xend);

					char* pos_ystr = malloc(pos_yend-pos_ystart);
					if(!pos_xstr) {
						free(pos_xstr);
						free(vertices);
						if(materials != NULL) free(materials);
						xmlFreeDoc(doc);
						return RSGE_ERROR_MALLOC;
					}
					memset(pos_ystr,0,pos_yend-pos_ystart);

					char* pos_zstr = malloc(pos_zend-pos_zstart);
					if(!pos_xstr) {
						free(pos_xstr);
						free(pos_ystr);
						free(vertices);
						if(materials != NULL) free(materials);
						xmlFreeDoc(doc);
						return RSGE_ERROR_MALLOC;
					}
					memset(pos_zstr,0,pos_zend-pos_zstart);

					strncpy(pos_xstr,position,pos_xend);
					strncpy(pos_ystr,position+pos_ystart,pos_yend-pos_ystart);
					strncpy(pos_zstr,position+pos_zstart,pos_zend-pos_zstart);

					pos_xstr[pos_xend] = 0;
					pos_ystr[pos_yend-pos_ystart] = 0;
					pos_zstr[pos_zend-pos_zstart] = 0;

					vert->pos[0] = strtof(pos_xstr,NULL);
					vert->pos[1] = strtof(pos_ystr,NULL);
					vert->pos[2] = strtof(pos_zstr,NULL);

					free(pos_xstr);
					free(pos_ystr);
					free(pos_zstr);

					char* color = (char*)xmlGetProp(verticeCur,"color");
					if(color != NULL) {
						vert->flags |= RSGE_SHAPE_VERT_FLAG_COLOR;

						int color_gstart = 0;
						for(int a = 0;a < strlen(color);a++) {
							if(color[a] == ',') {
								color_gstart = a+1;
								break;
							}
						}
						if(color_gstart == 0) {
							log_error("vertex tag has a malformed color");
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}

						int color_bstart = 0;
						for(int a = color_gstart;a < strlen(color);a++) {
							if(color[a] == ',') {
								color_bstart = a+1;
								break;
							}
						}
						if(color_bstart == 0) {
							log_error("vertex tag has a malformed color");
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}

						int color_astart = 0;
						for(int a = color_bstart;a < strlen(color);a++) {
							if(color[a] == ',') {
								color_astart = a+1;
								break;
							}
						}
						if(color_bstart == 0) {
							log_error("vertex tag has a malformed color");
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}

						int color_rend = color_gstart-1;
						int color_gend = color_bstart-1;
						int color_bend = color_astart-1;
						int color_aend = strlen(color);

						char* color_rstr = malloc(color_rend);
						if(!color_rstr) {
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(color_rstr,0,color_rend);

						char* color_gstr = malloc(color_gend-color_gstart);
						if(!color_gstr) {
							free(color_rstr);
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(color_gstr,0,color_gend-color_gstart);

						char* color_bstr = malloc(color_bend-color_bstart);
						if(!color_bstr) {
							free(color_rstr);
							free(color_gstr);
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(color_bstr,0,color_bend-color_bstart);

						char* color_astr = malloc(color_aend-color_astart);
						if(!color_astr) {
							free(color_rstr);
							free(color_gstr);
							free(color_bstr);
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(color_astr,0,color_aend-color_astart);

						strncpy(color_rstr,color,color_rend);
						strncpy(color_gstr,color+color_gstart,color_gend-color_gstart);
						strncpy(color_bstr,color+color_bstart,color_bend-color_bstart);
						strncpy(color_astr,color+color_astart,color_aend-color_astart);
	
						color_rstr[color_rend] = 0;
						color_gstr[color_gend-color_gstart] = 0;
						color_bstr[color_bend-color_bstart] = 0;
						color_astr[color_aend-color_astart] = 0;
	
						vert->color[0] = strtof(color_rstr,NULL);
						vert->color[1] = strtof(color_gstr,NULL);
						vert->color[2] = strtof(color_bstr,NULL);
						vert->color[3] = strtof(color_astr,NULL);

						free(color_rstr);
						free(color_gstr);
						free(color_bstr);
						free(color_astr);
					}

					char* color2nd = (char*)xmlGetProp(verticeCur,"color2nd");
					if(color2nd != NULL) {
						vert->flags |= RSGE_SHAPE_VERT_FLAG_2NDCOLOR;

						int color_gstart = 0;
						for(int a = 0;a < strlen(color2nd);a++) {
							if(color[a] == ',') {
								color_gstart = a+1;
								break;
							}
						}
						if(color_gstart == 0) {
							log_error("vertex tag has a malformed color");
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}

						int color_bstart = 0;
						for(int a = color_gstart;a < strlen(color2nd);a++) {
							if(color[a] == ',') {
								color_bstart = a+1;
								break;
							}
						}
						if(color_bstart == 0) {
							log_error("vertex tag has a malformed color");
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}

						int color_astart = 0;
						for(int a = color_bstart;a < strlen(color2nd);a++) {
							if(color[a] == ',') {
								color_astart = a+1;
								break;
							}
						}
						if(color_bstart == 0) {
							log_error("vertex tag has a malformed color");
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}

						int color_rend = color_gstart-1;
						int color_gend = color_bstart-1;
						int color_bend = color_astart-1;
						int color_aend = strlen(color2nd);

						char* color_rstr = malloc(color_rend);
						if(!color_rstr) {
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(color_rstr,0,color_rend);

						char* color_gstr = malloc(color_gend-color_gstart);
						if(!color_gstr) {
							free(color_rstr);
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(color_gstr,0,color_gend-color_gstart);

						char* color_bstr = malloc(color_bend-color_bstart);
						if(!color_bstr) {
							free(color_rstr);
							free(color_gstr);
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(color_bstr,0,color_bend-color_bstart);

						char* color_astr = malloc(color_aend-color_astart);
						if(!color_astr) {
							free(color_rstr);
							free(color_gstr);
							free(color_bstr);
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(color_astr,0,color_aend-color_astart);

						strncpy(color_rstr,color2nd,color_rend);
						strncpy(color_gstr,color2nd+color_gstart,color_gend-color_gstart);
						strncpy(color_bstr,color2nd+color_bstart,color_bend-color_bstart);
						strncpy(color_astr,color2nd+color_astart,color_aend-color_astart);
	
						color_rstr[color_rend] = 0;
						color_gstr[color_gend-color_gstart] = 0;
						color_bstr[color_bend-color_bstart] = 0;
						color_astr[color_aend-color_astart] = 0;
	
						vert->color2nd[0] = strtof(color_rstr,NULL);
						vert->color2nd[1] = strtof(color_gstr,NULL);
						vert->color2nd[2] = strtof(color_bstr,NULL);
						vert->color2nd[3] = strtof(color_astr,NULL);

						free(color_rstr);
						free(color_gstr);
						free(color_bstr);
						free(color_astr);
					}

					char* fogCoord = (char*)xmlGetProp(verticeCur,"fogCoord");
					if(fogCoord != NULL) {
						vert->flags |= RSGE_SHAPE_VERT_FLAG_FOG;
						vert->fogCoord = strtof(fogCoord,NULL);
					}

					char* textCoord = (char*)xmlGetProp(verticeCur,"textCoord");
					if(position != NULL) {
						vert->flags |= RSGE_SHAPE_VERT_FLAG_TEXTURE;
						int pos_ystart = 0;
						for(int a = 0;a < strlen(position);a++) {
							if(position[a] == ',') {
								pos_ystart = a+1;
								break;
							}
						}
	
						if(pos_ystart == 0) {
							log_error("vertex tag has a malformed position");
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}
	
						int pos_zstart = 0;
						for(int a = pos_ystart;a < strlen(position);a++) {
							if(position[a] == ',') {
								pos_zstart = a+1;
								break;
							}
						}
	
						if(pos_zstart == 0) {
							log_error("vertex tag has a malformed position");
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}
	
						int pos_xend = pos_ystart-1;
						int pos_yend = pos_zstart-1;
						int pos_zend = strlen(position);
	
						char* pos_xstr = malloc(pos_xend);
						if(!pos_xstr) {
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(pos_xstr,0,pos_xend);
	
						char* pos_ystr = malloc(pos_yend-pos_ystart);
						if(!pos_xstr) {
							free(pos_xstr);
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(pos_ystr,0,pos_yend-pos_ystart);
	
						char* pos_zstr = malloc(pos_zend-pos_zstart);
						if(!pos_xstr) {
							free(pos_xstr);
							free(pos_ystr);
							free(vertices);
							if(materials != NULL) free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(pos_zstr,0,pos_zend-pos_zstart);
	
						strncpy(pos_xstr,position,pos_xend);
						strncpy(pos_ystr,position+pos_ystart,pos_yend-pos_ystart);
						strncpy(pos_zstr,position+pos_zstart,pos_zend-pos_zstart);
	
						pos_xstr[pos_xend] = 0;
						pos_ystr[pos_yend-pos_ystart] = 0;
						pos_zstr[pos_zend-pos_zstart] = 0;
	
						vert->textCoord[0] = strtof(pos_xstr,NULL);
						vert->textCoord[1] = strtof(pos_ystr,NULL);
						vert->textCoord[2] = strtof(pos_zstr,NULL);
	
						free(pos_xstr);
						free(pos_ystr);
						free(pos_zstr);
					}

					log_debug("Vertice %d (Pos: %f, %f, %f)",i,vert->pos[0],vert->pos[1],vert->pos[2]);
					i++;
				}
				verticeCur = verticeCur->next;
			}
		}
		if(!xmlStrcmp(cur->name,(const xmlChar*)"materials")) {
			xmlNodePtr materialCur = cur->children;
			int i = 0;
			while(materialCur != NULL) {
				if(materialCur->type == XML_ELEMENT_NODE) {
					rsge_shape_mat_t* material = (rsge_shape_mat_t*)&materials[i];
					memset(material,0,sizeof(rsge_shape_mat_t));

					if(!!xmlStrcmp(materialCur->name,(const xmlChar*)"material")) {
						log_error("Invalid tag %s, expecting material tag",(char*)materialCur->name);
						free(vertices);
						free(materials);
						xmlFreeDoc(doc);
						return RSGE_ERROR_LIBXML;
					}

					char* face = (char*)xmlGetProp(materialCur,"face");
					if(!face) {
						log_error("material tag is missing face property");
						free(vertices);
						free(materials);
						xmlFreeDoc(doc);
						return RSGE_ERROR_LIBXML;
					}

					char* type = (char*)xmlGetProp(materialCur,"type");
					if(!type) {
						log_error("material tag is missing type property");
						free(vertices);
						free(materials);
						xmlFreeDoc(doc);
						return RSGE_ERROR_LIBXML;
					}

					if(!strcmp(face,"GL_FRONT")) material->face = GL_FRONT;
					else if(!strcmp(face,"GL_BACK")) material->face = GL_BACK;
					else if(!strcmp(face,"GL_FRONT_AND_BACK")) material->face = GL_FRONT_AND_BACK;
					else return RSGE_ERROR_LIBXML;

					if(!strcmp(type,"GL_AMBIENT")) material->type = GL_AMBIENT;
					else if(!strcmp(type,"GL_DIFFUSE")) material->type = GL_DIFFUSE;
					else if(!strcmp(type,"GL_SPECULAR")) material->type = GL_SPECULAR;
					else if(!strcmp(type,"GL_EMISSION")) material->type = GL_EMISSION;
					else if(!strcmp(type,"GL_SHININESS")) material->type = GL_SHININESS;
					else if(!strcmp(type,"GL_AMBIENT_AND_DIFFUSE")) material->type = GL_AMBIENT_AND_DIFFUSE;
					else if(!strcmp(type,"GL_COLOR_INDEXES")) material->type = GL_COLOR_INDEXES;
					else return RSGE_ERROR_LIBXML;

					char* value = (char*)xmlGetProp(materialCur,"value");
					if(value != NULL) {
						material->values = malloc(1);
						if(!material->values) {
							log_error("Failed to allocate 1 byte of memory.");
							free(vertices);
							free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}

						material->values[0] = atoi(value);
					} else {
						int valueCount = 0;
						xmlNodePtr materialValueCur = materialCur->children;
						while(materialValueCur != NULL) {
							if(materialValueCur->type == XML_ELEMENT_NODE) valueCount++;
							materialValueCur = materialValueCur->next;
						}

						material->values = malloc(valueCount);
						if(!material->values) {
							log_error("Failed to allocate 1 byte of memory.");
							free(vertices);
							free(materials);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}

						int a = 0;
						materialValueCur = materialCur->children;
						while(materialValueCur != NULL) {
							if(materialValueCur->type == XML_ELEMENT_NODE) {
								if(!!xmlStrcmp(materialValueCur->name,(const xmlChar*)"value")) {
									log_error("Invalid tag %s, expecting value tag",(char*)materialValueCur->name);
									free(vertices);
									free(materials);
									xmlFreeDoc(doc);
									return RSGE_ERROR_LIBXML;
								}
								char* value = (char*)xmlGetProp(materialValueCur,"value");
								if(!value) {
									log_error("material value element is missing value property");
									free(vertices);
									free(materials);
									xmlFreeDoc(doc);
									return RSGE_ERROR_LIBXML;
								}
								material->values[a++] = atoi(value);
							}
							materialValueCur = materialValueCur->next;
						}
					}
					i++;
				}
				materialCur = materialCur->next;
			}
		}
		cur = cur->next;
	}

	err = rsge_shape_create(shape,mode,vertices,verticeCount,materials,materialCount,shape_flags);
	if(err != RSGE_ERROR_NONE) {
		xmlFreeDoc(doc);
		return err;
	}

	node = xmlDocGetRootElement(doc);

	cur = node->children;
	while(cur != NULL) {
		if(!xmlStrcmp(cur->name,(const xmlChar*)"texture")) {
			char* mode = (char*)xmlGetProp(cur,"mode");
			if(!mode) {
				log_error("texture tag is missing the mode property");
				rsge_shape_destroy(shape);
				xmlFreeDoc(doc);
				return RSGE_ERROR_LIBXML;
			}

			if(!strcmp(mode,"GL_TEXTURE_1D")) shape->textureMode = GL_TEXTURE_1D;
			else if(!strcmp(mode,"GL_TEXTURE_2D")) shape->textureMode = GL_TEXTURE_2D;
			else if(!strcmp(mode,"GL_TEXTURE_3D")) shape->textureMode = GL_TEXTURE_3D;
			else if(!strcmp(mode,"GL_TEXTURE_1D_ARRAY")) shape->textureMode = GL_TEXTURE_1D_ARRAY;
			else if(!strcmp(mode,"GL_TEXTURE_2D_ARRAY")) shape->textureMode = GL_TEXTURE_2D_ARRAY;
			else if(!strcmp(mode,"GL_TEXTURE_RECTANGLE")) shape->textureMode = GL_TEXTURE_RECTANGLE;
			else if(!strcmp(mode,"GL_TEXTURE_CUBE_MAP")) shape->textureMode = GL_TEXTURE_CUBE_MAP;
			else if(!strcmp(mode,"GL_TEXTURE_CUBE_MAP_ARRAY")) shape->textureMode = GL_TEXTURE_CUBE_MAP_ARRAY;
			else if(!strcmp(mode,"GL_TEXTURE_BUFFER")) shape->textureMode = GL_TEXTURE_BUFFER;
			else if(!strcmp(mode,"GL_TEXTURE_2D_MULTISAMPLE")) shape->textureMode = GL_TEXTURE_2D_MULTISAMPLE;
			else if(!strcmp(mode,"GL_TEXTURE_2D_MULTISAMPLE_ARRAY")) shape->textureMode = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
			else return RSGE_ERROR_LIBXML;

			size_t width;
			size_t height;
			size_t bpp;
			unsigned char* buffer;

			char* path = (char*)xmlGetProp(cur,"path");
			if(path) {
				rsge_surface_t surface;
				err = rsge_image_fromFile(&surface,path);
				if(err != RSGE_ERROR_NONE) {
					rsge_shape_destroy(shape);
					xmlFreeDoc(doc);
					return err;
				}

				width = surface.width;
				height = surface.height;
				bpp = surface.bpp;
				buffer = malloc(((width*height)*bpp)*sizeof(unsigned char));
				if(!buffer) {
					xmlFreeDoc(doc);
					rsge_surface_destroy(&surface);
					rsge_shape_destroy(shape);
					return RSGE_ERROR_MALLOC;
				}

				for(size_t y = 0;y < height;y++) {
					for(size_t x = 0;x < width;x++) {
						size_t off = bpp*(x+y*width);
						for(size_t i = 0;i < bpp;i++) buffer[off+i] = surface.buffer[off+i];
					}
				}
				rsge_surface_destroy(&surface);
			} else {
				// TODO: load texture a different way.
				return RSGE_ERROR_LIBXML;
			}

			glGenTextures(1,&shape->texture);
			glBindTexture(shape->textureMode,shape->texture);

			xmlNodePtr textureCur = cur->children;
			while(textureCur != NULL) {
				if(!xmlStrcmp(textureCur->name,(const xmlChar*)"param")) {
					GLenum pname;

					char* pnameStr = (char*)xmlGetProp(textureCur,"pname");
					if(!pnameStr) {
						log_debug("param tag is missing pname property");
						rsge_shape_destroy(shape);
						free(buffer);
						xmlFreeDoc(doc);
						return RSGE_ERROR_LIBXML;
					}

					if(!strcmp(pnameStr,"GL_DEPTH_STENCIL_TEXTURE_MODE")) pname = GL_DEPTH_STENCIL_TEXTURE_MODE;
					else if(!strcmp(pnameStr,"GL_TEXTURE_BASE_LEVEL")) pname = GL_TEXTURE_BASE_LEVEL;
					else if(!strcmp(pnameStr,"GL_TEXTURE_COMPARE_FUNC")) pname = GL_TEXTURE_COMPARE_FUNC;
					else if(!strcmp(pnameStr,"GL_TEXTURE_COMPARE_MODE")) pname = GL_TEXTURE_COMPARE_MODE;
					else if(!strcmp(pnameStr,"GL_TEXTURE_LOD_BIAS")) pname = GL_TEXTURE_LOD_BIAS;
					else if(!strcmp(pnameStr,"GL_TEXTURE_MIN_FILTER")) pname = GL_TEXTURE_MIN_FILTER;
					else if(!strcmp(pnameStr,"GL_TEXTURE_MAG_FILTER")) pname = GL_TEXTURE_MAG_FILTER;
					else if(!strcmp(pnameStr,"GL_TEXTURE_MIN_LOD")) pname = GL_TEXTURE_MIN_LOD;
					else if(!strcmp(pnameStr,"GL_TEXTURE_MAX_LOD")) pname = GL_TEXTURE_MAX_LOD;
					else if(!strcmp(pnameStr,"GL_TEXTURE_MAX_LEVEL")) pname = GL_TEXTURE_MAX_LEVEL;
					else if(!strcmp(pnameStr,"GL_TEXTURE_SWIZZLE_R")) pname = GL_TEXTURE_SWIZZLE_R;
					else if(!strcmp(pnameStr,"GL_TEXTURE_SWIZZLE_G")) pname = GL_TEXTURE_SWIZZLE_G;
					else if(!strcmp(pnameStr,"GL_TEXTURE_SWIZZLE_B")) pname = GL_TEXTURE_SWIZZLE_B;
					else if(!strcmp(pnameStr,"GL_TEXTURE_SWIZZLE_A")) pname = GL_TEXTURE_SWIZZLE_A;
					else if(!strcmp(pnameStr,"GL_TEXTURE_WRAP_S")) pname = GL_TEXTURE_WRAP_S;
					else if(!strcmp(pnameStr,"GL_TEXTURE_WRAP_T")) pname = GL_TEXTURE_WRAP_T;
					else if(!strcmp(pnameStr,"GL_TEXTURE_WRAP_R")) pname = GL_TEXTURE_WRAP_R;
					else if(!strcmp(pnameStr,"GL_TEXTURE_BORDER_COLOR")) pname = GL_TEXTURE_BORDER_COLOR;
					else if(!strcmp(pnameStr,"GL_TEXTURE_SWIZZLE_RGBA")) pname = GL_TEXTURE_SWIZZLE_RGBA;
					else return RSGE_ERROR_LIBXML;

					if(!strcmp(pnameStr,"GL_TEXTURE_BORDER_COLOR") || !strcmp(pnameStr,"GL_TEXTURE_SWIZZLE_RGBA")) {
						char* color = (char*)xmlGetProp(textureCur,"value");
						if(!color) {
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}
						int color_gstart = 0;
						for(int a = 0;a < strlen(color);a++) {
							if(color[a] == ',') {
								color_gstart = a+1;
								break;
							}
						}
						if(color_gstart == 0) {
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}
						int color_bstart = 0;
						for(int a = color_gstart;a < strlen(color);a++) {
							if(color[a] == ',') {
								color_bstart = a+1;
								break;
							}
						}
						if(color_bstart == 0) {
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}

						int color_astart = 0;
						for(int a = color_bstart;a < strlen(color);a++) {
							if(color[a] == ',') {
								color_astart = a+1;
								break;
							}
						}
						if(color_bstart == 0) {
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}

						int color_rend = color_gstart-1;
						int color_gend = color_bstart-1;
						int color_bend = color_astart-1;
						int color_aend = strlen(color);

						char* color_rstr = malloc(color_rend);
						if(!color_rstr) {
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(color_rstr,0,color_rend);

						char* color_gstr = malloc(color_gend-color_gstart);
						if(!color_gstr) {
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(color_gstr,0,color_gend-color_gstart);

						char* color_bstr = malloc(color_bend-color_bstart);
						if(!color_bstr) {
							free(color_rstr);
							free(color_gstr);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(color_bstr,0,color_bend-color_bstart);

						char* color_astr = malloc(color_aend-color_astart);
						if(!color_astr) {
							free(color_rstr);
							free(color_gstr);
							free(color_bstr);
							xmlFreeDoc(doc);
							return RSGE_ERROR_MALLOC;
						}
						memset(color_astr,0,color_aend-color_astart);

						strncpy(color_rstr,color,color_rend);
						strncpy(color_gstr,color+color_gstart,color_gend-color_gstart);
						strncpy(color_bstr,color+color_bstart,color_bend-color_bstart);
						strncpy(color_astr,color+color_astart,color_aend-color_astart);

						color_rstr[color_rend] = 0;
						color_gstr[color_gend-color_gstart] = 0;
						color_bstr[color_bend-color_bstart] = 0;
						color_astr[color_aend-color_astart] = 0;
		
						float color_value[4];
						color_value[0] = strtof(color_rstr,NULL);
						color_value[1] = strtof(color_gstr,NULL);
						color_value[2] = strtof(color_bstr,NULL);
						color_value[3] = strtof(color_astr,NULL);

						free(color_rstr);
						free(color_gstr);
						free(color_bstr);
						free(color_astr);

						glTextureParameterfv(shape->textureMode,pname,(const GLfloat*)color_value);
					} else if(!strcmp(pnameStr,"GL_TEXTURE_MIN_FILTER")) {
						GLint param;

						char* paramStr = (char*)xmlGetProp(textureCur,"value");
						if(!paramStr) {
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}

						if(!strcmp(paramStr,"GL_NEAREST")) param = GL_NEAREST;
						else if(!strcmp(paramStr,"GL_LINEAR")) param = GL_LINEAR;
						else if(!strcmp(paramStr,"GL_NEAREST_MIPMAP_NEAREST")) param = GL_NEAREST_MIPMAP_NEAREST;
						else if(!strcmp(paramStr,"GL_LINEAR_MIPMAP_NEAREST")) param = GL_LINEAR_MIPMAP_NEAREST;
						else if(!strcmp(paramStr,"GL_NEAREST_MIPMAP_LINEAR")) param = GL_NEAREST_MIPMAP_LINEAR;
						else if(!strcmp(paramStr,"GL_LINEAR_MIPMAP_LINEAR")) param = GL_LINEAR_MIPMAP_LINEAR;
						else return RSGE_ERROR_LIBXML;

						glTexParameteri(shape->textureMode,pname,param);
					} else if(!strcmp(pnameStr,"GL_TEXTURE_MAG_FILTER")) {
						GLint param;

						char* paramStr = (char*)xmlGetProp(textureCur,"value");
						if(!paramStr) {
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}

						if(!strcmp(paramStr,"GL_NEAREST")) param = GL_NEAREST;
						else if(!strcmp(paramStr,"GL_LINEAR")) param = GL_LINEAR;
						else return RSGE_ERROR_LIBXML;

						glTexParameteri(shape->textureMode,pname,param);
					} else if(!strcmp(pnameStr,"GL_TEXTURE_SWIZZLE_R") || !strcmp(pnameStr,"GL_TEXTURE_SWIZZLE_G") || !strcmp(pnameStr,"GL_TEXTURE_SWIZZLE_B") || !strcmp(pnameStr,"GL_TEXTURE_SWIZZLE_A")) {
						GLfloat param;

						char* paramStr = (char*)xmlGetProp(textureCur,"value");
						if(!paramStr) {
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}

						if(!strcmp(paramStr,"GL_RED")) param = GL_RED;
						else if(!strcmp(paramStr,"GL_GREEN")) param = GL_GREEN;
						else if(!strcmp(paramStr,"GL_BLUE")) param = GL_BLUE;
						else if(!strcmp(paramStr,"GL_ALPHA")) param = GL_ALPHA;
						else if(!strcmp(paramStr,"GL_ZERO")) param = GL_ZERO;
						else if(!strcmp(paramStr,"GL_ONE")) param = GL_ONE;
						else param = strtof(paramStr,NULL);

						glTexParameterf(shape->textureMode,pname,param);
					} else if(!strcmp(pnameStr,"GL_TEXTURE_COMPARE_FUNC")) {
						GLint param;

						char* paramStr = (char*)xmlGetProp(textureCur,"value");
						if(!paramStr) {
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}

						if(!strcmp(paramStr,"GL_LEQUAL")) param = GL_LEQUAL;
						else if(!strcmp(paramStr,"GL_GEQUAL")) param = GL_GEQUAL;
						else if(!strcmp(paramStr,"GL_LESS")) param = GL_LESS;
						else if(!strcmp(paramStr,"GL_GREATER")) param = GL_GREATER;
						else if(!strcmp(paramStr,"GL_EQUAL")) param = GL_EQUAL;
						else if(!strcmp(paramStr,"GL_NOTEQUAL")) param = GL_NOTEQUAL;
						else if(!strcmp(paramStr,"GL_ALWAYS")) param = GL_ALWAYS;
						else if(!strcmp(paramStr,"GL_NEVER")) param = GL_NEVER;
						else return RSGE_ERROR_LIBXML;

						glTexParameteri(shape->textureMode,pname,param);
					} else if(!strcmp(pnameStr,"GL_TEXTURE_COMPARE_MODE")) {
						GLint param;

						char* paramStr = (char*)xmlGetProp(textureCur,"value");
						if(!paramStr) {
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}

						if(!strcmp(paramStr,"GL_COMPARE_REF_TO_TEXTURE")) param = GL_COMPARE_REF_TO_TEXTURE;
						else if(!strcmp(paramStr,"GL_NONE")) param = GL_NONE;
						else return RSGE_ERROR_LIBXML;

						glTexParameteri(shape->textureMode,pname,param);
					} else if(!strcmp(pnameStr,"GL_TEXTURE_WRAP_S") || !strcmp(pnameStr,"GL_TEXTURE_WRAP_T") || !strcmp(pnameStr,"GL_TEXTURE_WRAP_R")) {
						GLfloat param;

						char* paramStr = (char*)xmlGetProp(textureCur,"value");
						if(!paramStr) {
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}

						if(!strcmp(paramStr,"GL_CLAMP_TO_EDGE")) param = GL_CLAMP_TO_EDGE;
						else if(!strcmp(paramStr,"GL_CLAMP_TO_BORDER")) param = GL_CLAMP_TO_BORDER;
						else if(!strcmp(paramStr,"GL_MIRRORED_REPEAT")) param = GL_MIRRORED_REPEAT;
						else if(!strcmp(paramStr,"GL_REPEAT")) param = GL_REPEAT;
						else if(!strcmp(paramStr,"GL_MIRROR_CLAMP_TO_EDGE")) param = GL_MIRROR_CLAMP_TO_EDGE;
						else return RSGE_ERROR_LIBXML;

						glTexParameterf(shape->textureMode,pname,param);
					} else {
						char* paramStr = (char*)xmlGetProp(textureCur,"value");
						if(!paramStr) {
							xmlFreeDoc(doc);
							return RSGE_ERROR_LIBXML;
						}
						glTexParameterf(shape->textureMode,pname,strtof(paramStr,NULL));
					}
				}
				if(!xmlStrcmp(textureCur->name,(const xmlChar*)"env")) {
					GLenum target;
					GLenum pname;
					GLfloat param;

					char* targetStr = (char*)xmlGetProp(textureCur,"target");
					if(!targetStr) {
						log_debug("env tag is missing target property");
						rsge_shape_destroy(shape);
						free(buffer);
						xmlFreeDoc(doc);
						return RSGE_ERROR_LIBXML;
					}

					if(!strcmp(targetStr,"GL_TEXTURE_ENV")) target = GL_TEXTURE_ENV;
					else if(!strcmp(targetStr,"GL_TEXTURE_FILTER_CONTROL")) target = GL_TEXTURE_FILTER_CONTROL;
					else if(!strcmp(targetStr,"GL_POINT_SPRITE")) target = GL_POINT_SPRITE;
					else return RSGE_ERROR_LIBXML;

					char* pnameStr = (char*)xmlGetProp(textureCur,"pname");
					if(!pnameStr) {
						log_debug("texEnv tag is missing pname property");
						rsge_shape_destroy(shape);
						free(buffer);
						xmlFreeDoc(doc);
						return RSGE_ERROR_LIBXML;
					}

					if(!strcmp(pnameStr,"GL_TEXTURE_ENV_MODE")) pname = GL_TEXTURE_ENV_MODE;
					else if(!strcmp(pnameStr,"GL_TEXTURE_LOD_BIAS")) pname = GL_TEXTURE_LOD_BIAS;
					else if(!strcmp(pnameStr,"GL_COMBINE_RGB")) pname = GL_COMBINE_RGB;
					else if(!strcmp(pnameStr,"GL_COMBINE_ALPHA")) pname = GL_COMBINE_ALPHA;
					else if(!strcmp(pnameStr,"GL_SRC0_RGB")) pname = GL_SRC0_RGB;
					else if(!strcmp(pnameStr,"GL_SRC1_RGB")) pname = GL_SRC1_RGB;
					else if(!strcmp(pnameStr,"GL_SRC2_RGB")) pname = GL_SRC2_RGB;
					else if(!strcmp(pnameStr,"GL_SRC0_ALPHA")) pname = GL_SRC0_ALPHA;
					else if(!strcmp(pnameStr,"GL_SRC1_ALPHA")) pname = GL_SRC1_ALPHA;
					else if(!strcmp(pnameStr,"GL_SRC2_ALPHA")) pname = GL_SRC2_ALPHA;
					else if(!strcmp(pnameStr,"GL_OPERAND0_RGB")) pname = GL_OPERAND0_RGB;
					else if(!strcmp(pnameStr,"GL_OPERAND1_RGB")) pname = GL_OPERAND1_RGB;
					else if(!strcmp(pnameStr,"GL_OPERAND2_RGB")) pname = GL_OPERAND2_RGB;
					else if(!strcmp(pnameStr,"GL_OPERAND0_ALPHA")) pname = GL_OPERAND0_ALPHA;
					else if(!strcmp(pnameStr,"GL_OPERAND1_ALPHA")) pname = GL_OPERAND1_ALPHA;
					else if(!strcmp(pnameStr,"GL_OPERAND2_ALPHA")) pname = GL_OPERAND2_ALPHA;
					else if(!strcmp(pnameStr,"GL_RGB_SCALE")) pname = GL_RGB_SCALE;
					else if(!strcmp(pnameStr,"GL_ALPHA_SCALE")) pname = GL_ALPHA_SCALE;
					else if(!strcmp(pnameStr,"GL_COORD_REPLACE")) pname = GL_COORD_REPLACE;
					else return RSGE_ERROR_LIBXML;

					char* paramStr = (char*)xmlGetProp(textureCur,"param");
					if(!paramStr) {
						log_debug("texEnv tag is missing param property");
						rsge_shape_destroy(shape);
						free(buffer);
						xmlFreeDoc(doc);
						return RSGE_ERROR_LIBXML;
					}

					if(!strcmp(paramStr,"GL_ADD")) param = GL_ADD;
					else if(!strcmp(paramStr,"GL_ADD_SIGNED")) param = GL_ADD_SIGNED;
					else if(!strcmp(paramStr,"GL_INTERPOLATE")) param = GL_INTERPOLATE;
					else if(!strcmp(paramStr,"GL_MODULATE")) param = GL_MODULATE;
					else if(!strcmp(paramStr,"GL_DECAL")) param = GL_DECAL;
					else if(!strcmp(paramStr,"GL_BLEND")) param = GL_BLEND;
					else if(!strcmp(paramStr,"GL_REPLACE")) param = GL_REPLACE;
					else if(!strcmp(paramStr,"GL_SUBTRACT")) param = GL_SUBTRACT;
					else if(!strcmp(paramStr,"GL_COMBINE")) param = GL_COMBINE;
					else if(!strcmp(paramStr,"GL_TEXTURE")) param = GL_TEXTURE;
					else if(!strcmp(paramStr,"GL_CONSTANT")) param = GL_CONSTANT;
					else if(!strcmp(paramStr,"GL_PRIMARY_COLOR")) param = GL_PRIMARY_COLOR;
					else if(!strcmp(paramStr,"GL_PREVIOUS")) param = GL_PREVIOUS;
					else if(!strcmp(paramStr,"GL_SRC_COLOR")) param = GL_SRC_COLOR;
					else if(!strcmp(paramStr,"GL_ONE_MINUS_SRC_COLOR")) param = GL_ONE_MINUS_SRC_COLOR;
					else if(!strcmp(paramStr,"GL_SRC_ALPHA")) param = GL_SRC_ALPHA;
					else if(!strcmp(paramStr,"GL_ONE_MINUS_SRC_ALPHA")) param = GL_ONE_MINUS_SRC_ALPHA;
					else {
						if(!strcmp(pnameStr,"GL_TEXTURE_LOD_BIAS")) param = strtof(paramStr,NULL);
						else if(!strcmp(pnameStr,"GL_RGB_SCALE")) param = strtof(paramStr,NULL);
						else if(!strcmp(pnameStr,"GL_ALPHA_SCALE")) param = strtof(paramStr,NULL);
						else if(!strcmp(pnameStr,"GL_COORD_REPLACE")) {
							if(!strcmp(paramStr,"true")) param = true;
							else if(!strcmp(paramStr,"false")) param = false;
							else return RSGE_ERROR_LIBXML;
						} else return RSGE_ERROR_LIBXML;
					}

					glTexEnvf(target,pname,param);
				}
				textureCur = textureCur->next;
			}

			if(bpp == 4) glTexImage2D(shape->textureMode,0,3,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
			else if(bpp == 3) glTexImage2D(shape->textureMode,0,3,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,buffer);
			else return RSGE_ERROR_INVALID_BPP;
		}
		cur = cur->next;
	}

	xmlFreeDoc(doc);
	return RSGE_ERROR_NONE;
}