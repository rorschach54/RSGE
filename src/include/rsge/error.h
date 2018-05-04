#ifndef __RSGE_ERROR_H_
#define __RSGE_ERROR_H_ 1

/** \file src/include/rsge/error.h
 * \brief The error codes and error handling functions.
 * \author Spaceboy Ross
 */

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
	RSGE_ERROR_OPENAL,
	/**
	 * libconfig error.
	 */
	RSGE_ERROR_LIBCONFIG,
	/**
	 * No asset.
	 */
	RSGE_ERROR_NOASSET,
	/**
	 * OpenGL error.
	 */
	RSGE_ERROR_OPENGL,
	/**
	 * libxml2 error.
	 */
	RSGE_ERROR_LIBXML,
	/**
	 * From XML: UI widget is not valid. Registering a widget: Widget is already registered.
	 */
	RSGE_ERROR_INVALID_WIDGET,
	/**
	 * Invalid widget style name.
	 */
	RSGE_ERROR_INVALID_STYLE,
	/**
	 * Invalid asset location.
	 */
	RSGE_ERROR_INVALID_ASSET_LOC,
	/**
	 * Invalid input device.
	 */
	RSGE_ERROR_INVALID_DEVICE,
	/**
	 * An OS error.
	 */
	RSGE_ERROR_OS,
	/**
	 * Invalid bmp file.
	 */
	RSGE_ERROR_INVALID_BMP,
	/**
	 * Network error.
	 */
	RSGE_ERROR_NET,
	/**
	 * Invalid HTTP method
	 */
	RSGE_ERROR_INVALID_HTTP_METHOD,
	/**
	 * libcurl error
	 */
	RSGE_ERROR_CURL,
	/**
	 * Invalid flag
	 */
	RSGE_ERROR_INVALID_FLAG,
	/**
	 * Bullet physics
	 */
	RSGE_ERROR_BULLET_PHYSICS,
	/**
	 * OpenCL
	 */
	RSGE_ERROR_OPENCL,
	/**
	 * Invalid platform for OpenCL.
	 */
	RSGE_ERROR_CL_INVALID_PLATFORM,
	/**
	 * Invalid type.
	 */
	RSGE_ERROR_INVALID_TYPE
} rsge_error_e;

#endif
