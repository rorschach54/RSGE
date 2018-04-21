#ifndef __RSGE_GFX_MODEL_H_
#define __RSGE_GFX_MODEL_H_ 1

#include <rsge/gfx/shape.h>
#include <rsge/assets.h>

/**
 * \fn rsge_error_e rsge_model_fromFile(rsge_shape_t* shape,rsge_asset_t* asset)
 * \brief Loads a model from a file.
 * \param shape The pointer to the shape.
 * \param asset The asset to use.
 * \return An error code.
 */
rsge_error_e rsge_model_fromFile(rsge_shape_t* shape,rsge_asset_t* asset);

#endif
