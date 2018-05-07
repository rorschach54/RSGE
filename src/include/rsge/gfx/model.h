#ifndef __RSGE_GFX_MODEL_H_
#define __RSGE_GFX_MODEL_H_ 1

/** \file src/include/rsge/gfx/model.h
 * \brief Model stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/object.h>
#include <rsge/assets.h>
#include <list.h>

/**
 * \fn rsge_error_e rsge_model_fromFile(list_t** objects,char* path)
 * \brief Loads a model from a file.
 * \param[out] objects A list of the objects.
 * \param[in] path The path to the asset to use.
 * \return An error code.
 */
rsge_error_e rsge_model_fromFile(list_t** objects,char* path);

#endif
