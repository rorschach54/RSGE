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

static rsge_error_e rsge_model_obj_splitStr(char* a_str,const char a_delim,char*** result,size_t* count) {
    *count = 0;
    char* tmp = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    /* Count how many elements will be extracted. */
    while(*tmp) {
        if(a_delim == *tmp) {
            *count++;
            last_comma = tmp;
        }
        tmp++;
    }
    /* Add space for trailing token. */
    *count += last_comma < (a_str+strlen(a_str)-1);
    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    *count++;
    *result = malloc(sizeof(char*)*(*count));
    if(!result) return RSGE_ERROR_MALLOC;
    size_t idx  = 0;
    char* token = strtok(a_str,delim);
    while(token) {
        if(!(idx < *count)) return RSGE_ERROR_MALLOC;
        *(*result+idx++) = strdup(token);
        token = strtok(0,delim);
    }
    if(!(idx == *count-1)) return RSGE_ERROR_MALLOC;
    *(*result+idx) = 0;
    return RSGE_ERROR_NONE;
}

static rsge_error_e rsge_model_obj_readVec(char* line,vec3* vec) {
    while(line[0] == ' ') line++;
    char** split;
    size_t split_count;
    rsge_error_e err = rsge_model_obj_splitStr(line,' ',&split,&split_count);
    if(err != RSGE_ERROR_NONE) return err;
    *vec[0] = atof(split[0]);
    *vec[1] = atof(split[1]);
    *vec[2] = 0.0f;
    if(split_count > 3) *vec[2] = atof(split[2]);
    return RSGE_ERROR_NONE;
}

static rsge_error_e rsge_model_obj_readColor(char* line,rsge_color_rgba_t* color) {
    while(line[0] == ' ') line++;
    char** split;
    size_t split_count;
    rsge_error_e err = rsge_model_obj_splitStr(line,' ',&split,&split_count);
    if(err != RSGE_ERROR_NONE) return err;
    color->red = atof(split[0]);
    color->green = atof(split[1]);
    color->blue = atof(split[2]);
    color->alpha = 1.0f;
    if(split_count > 3) color->alpha = atof(split[3]);
    return RSGE_ERROR_NONE;
}

static rsge_error_e rsge_model_obj_readFloat(char* line,float* val) {
    while(line[0] == ' ') line++;
    char** split;
    size_t split_count;
    rsge_error_e err = rsge_model_obj_splitStr(line,' ',&split,&split_count);
    if(err != RSGE_ERROR_NONE) return err;
    *val = atof(split[0]);
    return RSGE_ERROR_NONE;
}

static rsge_error_e rsge_model_obj_readMaterials(char* path,char* baseDir,list_t** materials) {
    return RSGE_ERROR_NONE;
}

static rsge_error_e rsge_model_obj_readTexture(char* line,char* baseDir,rsge_obj_texture_t* texture) {
    while(line[0] == ' ') line++;
    char** split;
    size_t split_count;
    rsge_error_e err = rsge_model_obj_splitStr(line,' ',&split,&split_count);
    if(err != RSGE_ERROR_NONE) return err;
    int arg = 1;
    char* filename = split[arg++];
    // TODO: load texture
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_model_obj_fromFile(list_t** objects,char* path) {
    *objects = list_new();
    if(!objects) return RSGE_ERROR_MALLOC;
    
    list_t* asdf = list_new();
    if(!asdf) return RSGE_ERROR_MALLOC;
    char* line = "";
    list_t* verts = list_new();
    if(!verts) return RSGE_ERROR_MALLOC;
    list_t* normals = list_new();
    if(!normals) return RSGE_ERROR_MALLOC;
    list_t* uvs = list_new();
    if(!uvs) return RSGE_ERROR_MALLOC;
    list_t* materials = list_new();
    if(!materials) return RSGE_ERROR_MALLOC;
    
    char* currentObjectName = "";
    size_t baseDirectorySize = strlen(path);
    while(baseDirectorySize > 0 && path[baseDirectorySize--] == '/') break;
    while(path[baseDirectorySize--] == '/');
    char* baseDirectory = malloc(baseDirectorySize);
    if(!baseDirectory) return RSGE_ERROR_MALLOC;
    strncpy(baseDirectory,path,baseDirectorySize);
    
    rsge_material_t currentMaterial;
    char* data;
    size_t datasz;
    rsge_error_e err = rsge_asset_read(path,&data,&datasz);
    if(err != RSGE_ERROR_NONE) return err;
    
    line = data;
    while(line) {
        char* nextLine = strchr(line,'\n');
        if(nextLine) *nextLine = '\0';
        
        if(strlen(line) > 1) {
            char** split;
            size_t split_count;
            err = rsge_model_obj_splitStr(line,' ',&split,&split_count);
            if(err != RSGE_ERROR_NONE) return err;
            char* keyword = split[0];
            
            if(!!strcmp(keyword,"#")) {
                if(!strcmp(keyword,"mtllib")) {
                    char** split;
                    size_t split_count;
                    err = rsge_model_obj_splitStr(line,' ',&split,&split_count);
                    if(err != RSGE_ERROR_NONE) return err;
                    size_t matPathSize = strlen(baseDirectory);
                    if(split[0][0] == '/') split[0]++;
                    matPathSize += strlen(split[0])+1;
                    char* matPath = malloc(matPathSize);
                    if(!matPath) return RSGE_ERROR_MALLOC;
                    strcpy(matPath,baseDirectory);
                    strcat(matPath,"/");
                    strcat(matPath,split[0]);
                    err = rsge_model_obj_readMaterials(matPath,baseDirectory,&materials);
                    if(err != RSGE_ERROR_NONE) return err;
                } else if(!strcmp(keyword,"o")) {
                    char** split;
                    size_t split_count;
                    err = rsge_model_obj_splitStr(line,' ',&split,&split_count);
                    if(err != RSGE_ERROR_NONE) return err;
                    currentObjectName = split[0];
                } else if(!strcmp(keyword,"v")) {
                    vec3 pos;
                    err = rsge_model_obj_readVec(line,&pos);
                    if(err != RSGE_ERROR_NONE) return err;
                    list_rpush(verts,list_node_new(pos));
                } else if(!strcmp(keyword,"vn")) {
                    vec3 pos;
                    err = rsge_model_obj_readVec(line,&pos);
                    if(err != RSGE_ERROR_NONE) return err;
                    list_rpush(normals,list_node_new(pos));
                } else if(!strcmp(keyword,"vt")) {
                    vec3 pos;
                    err = rsge_model_obj_readVec(line,&pos);
                    if(err != RSGE_ERROR_NONE) return err;
                    list_rpush(uvs,list_node_new(pos));
                } else if(!strcmp(keyword,"f")) {
                } else if(!strcmp(keyword,"usemtl")) {
                }
            }
        }
        
        if(nextLine) *nextLine = '\n';
        line = nextLine ? (nextLine+1) : NULL;
    }
    return RSGE_ERROR_NONE;
}
#endif