#include <rsge/gfx/image/bmp.h>
#include <rsge/assets.h>

typedef struct {
    char* data;
    size_t datasz;
    size_t off;
} rsge_image_bmp_t;

rsge_error_e rsge_image_bmp_readint(rsge_image_bmp_t* bmp,unsigned int* val) {
    unsigned char b[4]; 
    unsigned int i;
    
    if(bmp->off+sizeof(b) >= bmp->datasz) return RSGE_ERROR_INVALID_BMP;
    memcpy(b,bmp->data+bmp->off,sizeof(b));
    bmp->off += sizeof(b);
    
    i = (b[3]<<24) | (b[2]<<16) | (b[1]<<8) | b[0];
    *val = i;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_image_bmp_readshort(rsge_image_bmp_t* bmp,unsigned short* val) {
    unsigned char b[2];
    unsigned short s;
    
    if(bmp->off+sizeof(b) >= bmp->datasz) return RSGE_ERROR_INVALID_BMP;
    memcpy(b,bmp->data+bmp->off,sizeof(b));
    bmp->off += sizeof(b);
    
    s = (b[1]<<8) | b[0];
    *val = s;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_image_bmp_fromFile(rsge_surface_t* surface,char* path) {
    rsge_image_bmp_t bmp;
    bmp.off = 18;
	rsge_error_e err = rsge_asset_read(path,&bmp.data,&bmp.datasz);
	if(err != RSGE_ERROR_NONE) return err;
	
	unsigned int res_w;
	err = rsge_image_bmp_readint(&bmp,&res_w);
	if(err != RSGE_ERROR_NONE) return err;
	
	unsigned int res_h;
	err = rsge_image_bmp_readint(&bmp,&res_h);
	if(err != RSGE_ERROR_NONE) return err;
	
	unsigned short planes;
	err = rsge_image_bmp_readshort(&bmp,&planes);
	if(err != RSGE_ERROR_NONE) return err;
	
	if(planes != 1) return RSGE_ERROR_INVALID_BMP;
	
	unsigned short bpp_s;
	err = rsge_image_bmp_readshort(&bmp,&bpp_s);
	if(err != RSGE_ERROR_NONE) return err;
	
	int bpp = bpp_s/8;
	bmp.off = 24;
	
	err = rsge_surface_create(surface,res_w,res_h,bpp,0);
	if(err != RSGE_ERROR_NONE) return err;
	memcpy(surface->buffer,bmp.data+bmp.off,(res_w*res_h)*bpp);
	return RSGE_ERROR_NONE;
}