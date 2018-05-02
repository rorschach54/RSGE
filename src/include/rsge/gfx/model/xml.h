#ifndef __RSGE_GFX_MODEL_XML_H_
#define __RSGE_GFX_MODEL_XML_H_ 1

/** \file src/include/rsge/gfx/model/xml.h
 * \brief XML model
 * \author Spaceboy Ross
 */

#include <rsge/gfx/shape.h>
#include <rsge/assets.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
rsge_error_e rsge_model_xml_fromFile(rsge_shape_t* shape,char* path);
#endif
#endif
