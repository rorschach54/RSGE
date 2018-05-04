#include <rsge/gfx/objects/texture.h>
#include <list.h>

static int rsge_obj_texture_next = 0;
list_t* rsge_obj_textures;

static int rsge_obj_texture_match(int id,rsge_obj_texture_t* texture) {
    return id == texture->id;
}

rsge_error_e rsge_obj_texture_create(rsge_obj_texture_t* texture) {
    memset(texture,0,sizeof(rsge_obj_texture_t));
    texture->id = rsge_obj_texture_next++;
    if(rsge_obj_textures == NULL) rsge_obj_textures = list_new();
    rsge_obj_textures->match = (int (*)(void*,void*))rsge_obj_texture_match;
    list_rpush(rsge_obj_textures,list_node_new(texture));
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_obj_texture_getbyid(int id,rsge_obj_texture_t** texture) {
    list_node_t* node = list_at(rsge_obj_textures,id);
    if(node == NULL) return RSGE_ERROR_INVALID_ID;
    *texture = node->val;
    return RSGE_ERROR_NONE;
}