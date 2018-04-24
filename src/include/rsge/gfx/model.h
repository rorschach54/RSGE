#ifndef __RSGE_GFX_MODEL_H_
#define __RSGE_GFX_MODEL_H_ 1

#include <rsge/gfx/shape.h>
#include <rsge/assets.h>

/**
 * \fn rsge_error_e rsge_model_fromFile(rsge_shape_t* shape,char* path)
 * \brief Loads a model from a file.
 * \param shape The pointer to the shape.
 * \param path The path to the asset to use.
 * \return An error code.
 */
rsge_error_e rsge_model_fromFile(rsge_shape_t* shape,char* path);

#endif
