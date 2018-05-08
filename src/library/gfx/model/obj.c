#include <rsge/gfx/model/obj.h>
#include <rsge/gfx/objects/texture.h>
#include <rsge/gfx/colors.h>
#include <rsge/gfx/mesh.h>
#include <rsge/assets.h>
#include <log.h>
#include <linmath.h>
#include <string.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
typedef struct {
    char* data;
    size_t datasz;
    size_t off;
} rsge_model_obj_t;

rsge_error_e rsge_model_obj_fromFile(list_t** objects,char* path) {
    if(objects == NULL) {
        *objects = list_new();
        if(!objects) return RSGE_ERROR_MALLOC;
    }
    
    list_t* verts = list_new();
    if(!verts) return RSGE_ERROR_MALLOC;
    list_t* normals = list_new();
    if(!normals) return RSGE_ERROR_MALLOC;
    list_t* uvs = list_new();
    if(!uvs) return RSGE_ERROR_MALLOC;
    list_t* materials = list_new();
    if(!materials) return RSGE_ERROR_MALLOC;
    list_t* vertexIndices = list_new();
    if(!vertexIndices) return RSGE_ERROR_MALLOC;
    list_t* uvIndices = list_new();
    if(!uvIndices) return RSGE_ERROR_MALLOC;
    list_t* normalIndices = list_new();
    if(!normalIndices) return RSGE_ERROR_MALLOC;
    
    char* currentObjectName = "";
    size_t baseDirectorySize = strlen(path);
    while(baseDirectorySize > 0 && path[baseDirectorySize--] == '/') break;
    while(path[baseDirectorySize--] == '/');
    char* baseDirectory = malloc(baseDirectorySize);
    if(!baseDirectory) return RSGE_ERROR_MALLOC;
    strncpy(baseDirectory,path,baseDirectorySize);
    
    char* data;
    size_t datasz;
    rsge_error_e err = rsge_asset_read(path,&data,&datasz);
    if(err != RSGE_ERROR_NONE) return err;
    
    while(true) {
        char lineHeader[128];
        int res = sscanf(data,"%s",lineHeader);
        if(res == EOF) break;
        
        if(!strcmp(lineHeader,"v")) {
            vec3 pos;
            sscanf(data,"%f %f %f\n",&pos[0],&pos[1],&pos[2]);
            list_rpush(verts,list_node_new(pos));
        } else if(!strcmp(lineHeader,"vt")) {
            vec2 pos;
            sscanf(data,"%f %f\n",&pos[0],&pos[1]);
            list_rpush(uvs,list_node_new(pos));
        } else if(!strcmp(lineHeader,"vn")) {
            vec3 pos;
            sscanf(data,"%f %f %f\n",&pos[0],&pos[1],&pos[2]);
            list_rpush(normals,list_node_new(pos));
        } else if(!strcmp(lineHeader,"f")) {
            char* vertex1;
            char* vertex2;
            char* vertex3;
            unsigned int vertexIndex[3];
            unsigned int uvIndex[3];
            unsigned int normalIndex[3];
            int matches = sscanf(data,"%d/%d/%d %d/%d/%d %d/%d/%d\n",&vertexIndex[0],&uvIndex[0],&normalIndex[0],&vertexIndex[1],&uvIndex[1],&normalIndex[1],&vertexIndex[2],&uvIndex[2],&normalIndex[2]);
            if(matches != 9) return RSGE_ERROR_INVALID_TYPE;
            list_rpush(vertexIndices,list_node_new(&vertexIndex[0]));
            list_rpush(vertexIndices,list_node_new(&vertexIndex[1]));
            list_rpush(vertexIndices,list_node_new(&vertexIndex[2]));
            list_rpush(uvIndices,list_node_new(&uvIndex[0]));
            list_rpush(uvIndices,list_node_new(&uvIndex[1]));
            list_rpush(uvIndices,list_node_new(&uvIndex[2]));
            list_rpush(normalIndices,list_node_new(&normalIndex[0]));
            list_rpush(normalIndices,list_node_new(&normalIndex[1]));
            list_rpush(normalIndices,list_node_new(&normalIndex[2]));
        }
    }
    rsge_object_t* obj = malloc(sizeof(rsge_object_t));
    if(!obj) return RSGE_ERROR_MALLOC;
    
    obj->meshes = list_new();
    if(!obj->meshes) return RSGE_ERROR_MALLOC;
    
    rsge_mesh_t* mesh = malloc(sizeof(rsge_mesh_t));
    if(!mesh) return RSGE_ERROR_MALLOC;
    
    mesh->vertices = list_new();
    if(!mesh->vertices) return RSGE_ERROR_MALLOC;
    
    for(int i = 0;i < vertexIndices->len;i++) {
        unsigned int vertexIndex = *((unsigned int*)list_at(vertexIndices,i)->val);
        float* vertexFA = (float*)(list_at(verts,vertexIndex-1)->val);
        vec3 vertex = { vertexFA[0], vertexFA[1], vertexFA[2] };
        
        unsigned int uvIndex = *((unsigned int*)list_at(uvIndices,i)->val);
        float* uvFA = (float*)(list_at(uvs,uvIndex-1)->val);
        vec2 uv = { uvFA[0], uvFA[1] };
        
        unsigned int normalIndex = *((unsigned int*)list_at(normalIndices,i)->val);
        float* normalFA = (float*)(list_at(normals,normalIndex-1)->val);
        vec3 normal = { normalFA[0], normalFA[1], normalFA[2] };
        
        list_rpush(mesh->vertices,list_node_new(vertex));
        list_rpush(mesh->normals,list_node_new(normal));
    }
    
    list_rpush(obj->meshes,list_node_new(mesh));
    list_rpush(*objects,list_node_new(obj));
    return RSGE_ERROR_NONE;
}
#endif