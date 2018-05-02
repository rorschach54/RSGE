#ifndef __RSGE_CL_INIT_H_
#define __RSGE_CL_INIT_H_ 1

/** \file src/include/rsge/cl/init.h
 * \brief OpenCL initialization.
 * \author Spaceboy Ross
 */

#include <rsge/error.h>

/**
 * \fn rsge_error_e rsge_cl_init()
 * \brief Initializes OpenCL. This is automatically done.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_cl_init();

/**
 * \fn rsge_error_e rsge_cl_deinit()
 * \brief Deinitializes OpenCL. This is automatically done.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_cl_deinit();

#endif