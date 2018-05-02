#ifndef __RSGE_CL_PROGRAM_H_
#define __RSGE_CL_PROGRAM_H_ 1

/** \file src/include/rsge/cl/program.h
 * \brief OpenCL program.
 * \author Spaceboy Ross
 */

#include <rsge/cl/cl.h>
#include <rsge/error.h>

/** \struct rsge_cl_prog_t src/include/rsge/cl/program.h rsge/cl/program.h
 * \brief OpenCL program.
 */
typedef struct {
    /**
     * \brief The OpenCL program.
     */
    cl_program prog;
} rsge_cl_prog_t;

#endif