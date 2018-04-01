#ifndef __RSGE_ERROR_H_
#define __RSGE_ERROR_H_ 1

/**
 * \enum rsge_error_e
 * \brief An enum containing all error codes.
 */
typedef enum {
	/**
	 * No error.
	 */
	RSGE_ERROR_NONE = 0,
	/**
	 * malloc() failed.
	 */
	RSGE_ERROR_MALLOC,
	/**
	 * Invalid bits per pixel (bpp).
	 */
	RSGE_ERROR_INVALID_BPP,
	/**
	 * Freetype library error.
	 */
	RSGE_ERROR_FREETYPE,
	/**
	 * Invalid font.
	 */
	RSGE_ERROR_INVALID_FONT,
	/**
	 * Shader error.
	 */
	RSGE_ERROR_SHADER,
	/**
	 * libpng library error.
	 */
	RSGE_ERROR_LIBPNG,
	/**
	 * File error.
	 */
	RSGE_ERROR_FILE,
	/**
	 * Invalid file.
	 */
	RSGE_ERROR_INVALID_FILE,
	/**
	 * OpenAL error.
	 */
	RSGE_ERROR_OPENAL
} rsge_error_e;

#endif
