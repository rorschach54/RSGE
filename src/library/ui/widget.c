#include <rsge/ui/widget.h>
#include <string.h>

rsge_error_e rsge_ui_widget_loadStyles(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node) {
	list_node_t* no;
	xmlNodePtr cur = node->xmlChildrenNode;
	while(cur != NULL) {
		char* value_raw = (char*)xmlGetProp(cur,"value");
		if(!value_raw) return RSGE_ERROR_LIBXML;

		void* value = value_raw;

		if(!xmlStrcmp(cur->name,(const xmlChar*)"backgroundColor")
			|| !xmlStrcmp(cur->name,(const xmlChar*)"foregroundColor")
			|| !xmlStrcmp(cur->name,(const xmlChar*)"textColor")) {
			if(value_raw[0] == '#') {
				value_raw++;
				int color[4];
				color[0] = color[1] = color[2] = 0;
				color[3] = 0xFF;
				if(strlen(value_raw) == 2) color[0] = (int)strtol(value_raw,NULL,16);
				else if(strlen(value_raw) == 4) {
					char raw_red[2];
					char raw_green[2];

					memcpy(raw_red,value_raw,2);
					memcpy(raw_green,value_raw+2,2);

					color[0] = (int)strtol(raw_red,NULL,16);
					color[1] = (int)strtol(raw_green,NULL,16);
				} else if(strlen(value_raw) == 6) {
					char raw_red[2];
					char raw_green[2];
					char raw_blue[2];

					memcpy(raw_red,value_raw,2);
					memcpy(raw_green,value_raw+2,2);
					memcpy(raw_blue,value_raw+4,2);

					color[0] = (int)strtol(raw_red,NULL,16);
					color[1] = (int)strtol(raw_green,NULL,16);
					color[2] = (int)strtol(raw_blue,NULL,16);
				} else if(strlen(value_raw) == 8) {
					char raw_red[2];
					char raw_green[2];
					char raw_blue[2];
					char raw_alpha[2];

					memcpy(raw_red,value_raw,2);
					memcpy(raw_green,value_raw+2,2);
					memcpy(raw_blue,value_raw+4,2);
					memcpy(raw_alpha,value_raw+6,2);

					color[0] = (int)strtol(raw_red,NULL,16);
					color[1] = (int)strtol(raw_green,NULL,16);
					color[2] = (int)strtol(raw_blue,NULL,16);
					color[3] = (int)strtol(raw_alpha,NULL,16);
				}
				value = color;
			}
		}

		rsge_ui_widget_style_t style = {
			.name = (char*)cur->name,
			.value = value
		};

		void* tmp;
		rsge_error_e err = rsge_ui_widget_getstyle(widget,style.name,&tmp);
		if(err) {
			no = list_node_new(&style);
			if(!no) return RSGE_ERROR_MALLOC;
			list_rpush(widget->styles,no);
		} else {
			list_iterator_t* it = list_iterator_new(widget->styles,LIST_HEAD);
			if(it == NULL) return RSGE_ERROR_MALLOC;
			while((no = list_iterator_next(it))) {
				if(!strcmp(((rsge_ui_widget_style_t*)no->val)->name,style.name)) list_remove(widget->styles,no);
			}
			list_iterator_destroy(it);

			no = list_node_new(&style);
			if(!no) return RSGE_ERROR_MALLOC;
			list_rpush(widget->styles,no);
		}
		cur = cur->next;
	}
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_widget_loadEvents(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node) {
	list_node_t* no;
	xmlNodePtr cur = node->xmlChildrenNode;
	while(cur != NULL) {
		char* type = (char*)xmlGetProp(cur,"type");
		if(!type) return RSGE_ERROR_LIBXML;

		rsge_ui_widget_event_t event = {
			.type = type
		};

		xmlNodePtr event_cur = cur->xmlChildrenNode;
		while(event_cur != NULL) {
			if(!xmlStrcmp(event_cur->name,(const xmlChar*)"styles")) {
				event.stylesNode = event_cur;
			}
			event_cur = event_cur->next;
		}

		no = list_node_new(&event);
		if(!no) return RSGE_ERROR_MALLOC;
		list_rpush(widget->events,no);
		cur = cur->next;
	}
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_widget_create(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node) {
	memset(widget,0,sizeof(rsge_ui_widget_t));

	widget->baseXML.doc = doc;
	widget->baseXML.node = node;

	widget->styles = list_new();
	if(!widget->styles) return RSGE_ERROR_MALLOC;

	widget->events = list_new();
	if(!widget->events) {
		list_destroy(widget->styles);
		return RSGE_ERROR_MALLOC;
	}

	rsge_error_e err;
	xmlNodePtr cur = node->xmlChildrenNode;
	while(cur != NULL) {
		if(!xmlStrcmp(cur->name,(const xmlChar*)"styles")) {
			err = rsge_ui_widget_loadStyles(widget,ui,doc,cur);
			if(err != RSGE_ERROR_NONE) {
				list_destroy(widget->styles);
				list_destroy(widget->events);
				return err;
			}
		}
		if(!xmlStrcmp(cur->name,(const xmlChar*)"events")) {
			err = rsge_ui_widget_loadEvents(widget,ui,doc,cur);
			if(err != RSGE_ERROR_NONE) {
				list_destroy(widget->styles);
				list_destroy(widget->events);
				return err;
			}
		}
		cur = cur->next;
	}
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_widget_getstyle(rsge_ui_widget_t* widget,char* name,void** value) {
	list_node_t* node;
	list_iterator_t* it = list_iterator_new(widget->styles,LIST_HEAD);
	if(it == NULL) return RSGE_ERROR_MALLOC;
	while((node = list_iterator_next(it))) {
		rsge_ui_widget_style_t* style = (rsge_ui_widget_style_t*)node->val;
		if(!strcmp(style->name,name)) {
			*value = style->value;
			list_iterator_destroy(it);
			return RSGE_ERROR_NONE;
		}
	}
	list_iterator_destroy(it);
	return RSGE_ERROR_INVALID_STYLE;
}

rsge_error_e rsge_ui_widget_getevent(rsge_ui_widget_t* widget,char* name,rsge_ui_widget_event_t*** events,int* count) {
	*count = 0;
	list_node_t* node;
	list_iterator_t* it = list_iterator_new(widget->events,LIST_HEAD);
	if(it == NULL) return RSGE_ERROR_MALLOC;
	while((node = list_iterator_next(it))) {
		rsge_ui_widget_event_t* event = (rsge_ui_widget_event_t*)node->val;
		*count++;
	}
	list_iterator_destroy(it);
	*events = malloc(sizeof(rsge_ui_widget_event_t)*(*count));
	if(events == NULL) return RSGE_ERROR_MALLOC;

	it = list_iterator_new(widget->events,LIST_HEAD);
	if(it == NULL) {
		free(*events);
		return RSGE_ERROR_MALLOC;
	}
	int i = 0;
	while((node = list_iterator_next(it))) {
		rsge_ui_widget_event_t* event = (rsge_ui_widget_event_t*)node->val;
		*events[i++] = event;
	}
	list_iterator_destroy(it);
	return RSGE_ERROR_INVALID_STYLE;
}