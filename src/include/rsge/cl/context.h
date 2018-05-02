#ifndef __RSGE_CL_PROGRAM_H_
#define __RSGE_CL_PROGRAM_H_ 1

/** \file src/include/rsge/cl/context.h
 * \brief OpenCL context.
 * \author Spaceboy Ross
 */

#include <rsge/cl/cl.h>
#include <rsge/error.h>

/** \struct rsge_cl_ctx_t src/include/rsge/cl/ctx.h rsge/cl/ctx.h
 * \brief OpenCL context.
 */
typedef struct {
    /**
     * \brief The OpenCL context.
     */
    cl_context context;
} rsge_cl_ctx_t;

/**
 * \fn rsge_error_e rsge_cl_ctx_create(rsge_cl_ctx_t* ctx,int platformIndex)
 * \brief Creates a new OpenCL context.
 * \param[out] ctx The pointer that stores the OpenCL context.
 * \param[in] platformIndex The platform ID index to use.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_cl_ctx_create(rsge_cl_ctx_t* ctx,int platformIndex);

#endif