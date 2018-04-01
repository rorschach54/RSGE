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
	RSGE_ERROR_SHADER
} rsge_error_e;

#endif
