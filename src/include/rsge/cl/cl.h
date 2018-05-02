#ifndef __RSGE_CL_CL_H_
#define __RSGE_CL_CL_H_ 1

/** \file src/include/rsge/cl/cl.h
 * \brief OpenCL header
 * \author Spaceboy Ross
 */

#ifdef __cplusplus
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.hpp>
#endif
#else
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#endif

#endif