#include <rsge/gfx/image/libpng.h>
#include <stdio.h>
#include <stdlib.h>

rsge_error_e rsge_image_libpng_fromFile(rsge_surface_t* surface,char* path) {
	FILE* fp = fopen(path,"rb");
	if(!fp) return RSGE_ERROR_FILE;

	char header[8];
	fread(header,1,8,fp);
	if(png_sig_cmp(header,0,8)) {
		fclose(fp);
		return RSGE_ERROR_LIBPNG;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
	if(!png_ptr) {
		fclose(fp);
		return RSGE_ERROR_LIBPNG;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr) {
		fclose(fp);
		return RSGE_ERROR_LIBPNG;
	}

	if(setjmp(png_jmpbuf(png_ptr))) {
		fclose(fp);
		return RSGE_ERROR_LIBPNG;
	}

	png_init_io(png_ptr,fp);
	png_set_sig_bytes(png_ptr,8);
	png_read_info(png_ptr,info_ptr);

	int width = png_get_image_width(png_ptr,info_ptr);
	int height = png_get_image_height(png_ptr,info_ptr);
	png_byte color_type = png_get_color_type(png_ptr,info_ptr);
	png_byte bit_depth = png_get_bit_depth(png_ptr,info_ptr);
	int number_of_passes = png_set_interlace_handling(png_ptr);

	png_read_update_info(png_ptr,info_ptr);

	if(setjmp(png_jmpbuf(png_ptr))) {
		fclose(fp);
		return RSGE_ERROR_LIBPNG;
	}

	png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep)*height);
	if(!row_pointers) {
		fclose(fp);
		return RSGE_ERROR_MALLOC;
	}

	for(int y = 0;y < height;y++) {
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr,info_ptr));
		if(!row_pointers[y]) {
			free(row_pointers);
			fclose(fp);
			return RSGE_ERROR_MALLOC;
		}
	}

	png_read_image(png_ptr,row_pointers);
	fclose(fp);

	size_t bpp = 0;
	if(png_get_color_type(png_ptr,info_ptr) == PNG_COLOR_TYPE_RGB) bpp = 3;
	else if(png_get_color_type(png_ptr,info_ptr) == PNG_COLOR_TYPE_RGBA) bpp = 4;
	else {
		free(row_pointers);
		return RSGE_ERROR_INVALID_BPP;
	}
	rsge_error_e err = rsge_surface_create(surface,width,height,bpp);
	if(err != RSGE_ERROR_NONE) {
		free(row_pointers);
		return err;
	}

	for(int y = 0;y < height;y++) {
		png_byte* row = row_pointers[y];
		for(int x = 0;x < width;x++) {
			png_byte* ptr = &(row[x*bpp]);
			size_t off = surface->bpp*((x+y)*surface->width);
			for(size_t i = 0;i < bpp;i++) surface->buffer[off+i] = ptr[i];
		}
	}

	free(row_pointers);
	return RSGE_ERROR_NONE;
}
