#include <rsge/gfx/model/xml.h>
#include <rsge/gfx/model.h>
#include <rsge/config.h>
#include <string.h>

rsge_error_e rsge_model_fromFile(rsge_shape_t* shape,rsge_asset_t* asset) {
	rsge_error_e err;

	err = rsge_model_xml_fromFile(shape,asset);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}