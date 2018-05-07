#ifndef __RSGE_GFX_MODEL_OBJ_H_
#define __RSGE_GFX_MODEL_OBJ_H_ 1

/** \file src/include/rsge/gfx/model/obj.h
 * \brief Object (.obj) model
 * \author Spaceboy Ross
 */

#include <rsge/gfx/model.h>
#include <rsge/assets.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
rsge_error_e rsge_model_obj_fromFile(list_t** objects,char* path);
#endif
#endif
