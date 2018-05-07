#include <rsge/gfx/image/bmp.h>
#include <rsge/assets.h>
#include <log.h>
#include <stdlib.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
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
    
    i = (b[3] << 24) | (b[2] << 16) | (b[1] << 8) | b[0];
    *val = i;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_image_bmp_readshort(rsge_image_bmp_t* bmp,unsigned short* val) {
    unsigned char b[2];
    unsigned short s;
    
    if(bmp->off+sizeof(b) >= bmp->datasz) return RSGE_ERROR_INVALID_BMP;
    memcpy(b,bmp->data+bmp->off,sizeof(b));
    bmp->off += sizeof(b);
    
    s = (b[1] << 8) | b[0];
    *val = s;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_image_bmp_fromFile(rsge_obj_texture_t* texture,char* path) {
    rsge_image_bmp_t bmp;
	rsge_error_e err = rsge_asset_read(path,&bmp.data,&bmp.datasz);
	if(err != RSGE_ERROR_NONE) return err;
	
	/* Start file header decoding */
	if(bmp.data[0] != 'B' && bmp.data[1] != 'M') return RSGE_ERROR_INVALID_BMP;
	
	bmp.off = 10;
	unsigned int pixelDataOffset;
	err = rsge_image_bmp_readint(&bmp,&pixelDataOffset);
	if(err != RSGE_ERROR_NONE) return err;
	/* File header decoding finished */
	
	/* Start image header decoding */
    bmp.off = 18;
	
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
	
	bmp.off += 32;
	unsigned int colormapCount;
	err = rsge_image_bmp_readint(&bmp,&colormapCount);
	if(err != RSGE_ERROR_NONE) return err;
	
	bmp.off += 4;
	/* Image header decoding finished */
	
	int bpp = 3;
	bmp.off = pixelDataOffset;
	
	err = rsge_obj_texture_create(texture);
	if(err != RSGE_ERROR_NONE) return err;
	texture->width = res_w;
	texture->height = res_h;
	texture->pixels = malloc(((res_w+4-(res_w % 4))*res_h));
	if(!texture->pixels) {
		log_error("Failed to allocate %d bytes of memory",((res_w+4-(res_w % 4))*res_h));
		return RSGE_ERROR_MALLOC;
	}
	char* cursor = bmp.data+bmp.off;
	/* Load the pixel data */
	for(size_t y = res_h-1;y >= 0;y--) {
		for(size_t x = 0;x < res_w;x++) {
			rsge_color_rgba_t pixel;
			pixel.red = (float)((unsigned char)(*(cursor++)))/255;
			pixel.green = (float)((unsigned char)(*(cursor++)))/255;
			pixel.blue = (float)((unsigned char)(*(cursor++)))/255;
			pixel.alpha = 1.0f;
			texture->pixels[y*res_w+x] = pixel;
		}
		cursor += (4-((res_w*3) % 4)) % 4;
	}
	return RSGE_ERROR_NONE;
}
#endif
