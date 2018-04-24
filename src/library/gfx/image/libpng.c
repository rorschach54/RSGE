#include <rsge/gfx/image/libpng.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
	char* data;
	size_t size;
	uint32_t off;
} rsge_asset_libpng_t;

void rsge_image_libpng_read(png_structp png_ptr,png_bytep outBytes,png_size_t byteCountToRead) {
	png_voidp io_ptr = png_get_io_ptr(png_ptr);
	if(io_ptr == NULL) return;
	rsge_asset_libpng_t* libpng_asset = (rsge_asset_libpng_t*)io_ptr;
	for(size_t i = 0;i < byteCountToRead;i++) {
		if(libpng_asset->off > libpng_asset->size) {
			outBytes[i] = libpng_asset->data[libpng_asset->off+i];
			libpng_asset->off++;
		} else {
			outBytes[i] = 0;
		}
	}
}

rsge_error_e rsge_image_libpng_fromFile(rsge_surface_t* surface,char* path) {
	rsge_asset_libpng_t libpng_asset;
	rsge_error_e err = rsge_asset_read(path,&libpng_asset.data,&libpng_asset.size);
	if(err != RSGE_ERROR_NONE) return err;
	libpng_asset.off = 0;

	char header[8];
	for(int i = 0;i < 8;i++) header[i] = libpng_asset.data[i];
	if(png_sig_cmp(header,0,8)) {
		return RSGE_ERROR_LIBPNG;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
	if(!png_ptr) {
		return RSGE_ERROR_LIBPNG;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr) {
		return RSGE_ERROR_LIBPNG;
	}

	if(setjmp(png_jmpbuf(png_ptr))) {
		return RSGE_ERROR_LIBPNG;
	}
	png_set_read_fn(png_ptr,&libpng_asset,rsge_image_libpng_read);
	png_set_sig_bytes(png_ptr,8);
	png_read_info(png_ptr,info_ptr);

	int width = png_get_image_width(png_ptr,info_ptr);
	int height = png_get_image_height(png_ptr,info_ptr);
	png_byte color_type = png_get_color_type(png_ptr,info_ptr);
	png_byte bit_depth = png_get_bit_depth(png_ptr,info_ptr);
	int number_of_passes = png_set_interlace_handling(png_ptr);

	png_read_update_info(png_ptr,info_ptr);

	if(setjmp(png_jmpbuf(png_ptr))) {
		return RSGE_ERROR_LIBPNG;
	}

	png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep)*height);
	if(!row_pointers) {
		return RSGE_ERROR_MALLOC;
	}

	for(int y = 0;y < height;y++) {
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr,info_ptr));
		if(!row_pointers[y]) {
			free(row_pointers);
			return RSGE_ERROR_MALLOC;
		}
	}

	png_read_image(png_ptr,row_pointers);

	size_t bpp = 0;
	if(png_get_color_type(png_ptr,info_ptr) == PNG_COLOR_TYPE_RGB) bpp = 3;
	else if(png_get_color_type(png_ptr,info_ptr) == PNG_COLOR_TYPE_RGBA) bpp = 4;
	else {
		free(row_pointers);
		return RSGE_ERROR_INVALID_BPP;
	}
	err = rsge_surface_create(surface,width,height,bpp,0);
	if(err != RSGE_ERROR_NONE) {
		free(row_pointers);
		return err;
	}

	for(int y = 0;y < height;y++) {
		png_byte* row = row_pointers[y];
		for(int x = 0;x < width;x++) {
			png_byte* ptr = &(row[x*bpp]);
			size_t off = surface->bpp*(x+y*surface->width);
			for(size_t i = 0;i < bpp;i++) {
				surface->buffer[off+i] = ptr[i];
			}
		}
	}

	free(row_pointers);
	return RSGE_ERROR_NONE;
}