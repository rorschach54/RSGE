GLAD_EXTS=GL_ARB_shading_language_420pack,GL_EXT_framebuffer_object,GL_EXT_framebuffer_blit,GL_EXT_framebuffer_multisample,GL_EXT_packed_depth_stencil,GL_EXT_texture_filter_anisotropic,GL_ARB_vertex_array_object,GL_ARB_get_program_binary,GL_ARB_vertex_attrib_binding

$(GLAD_PATH):
	@echo "Setting up GLAD"
	@glad --api "gl=4.2" --generator=c --extensions=$(GLAD_EXTS) --out-path=$(GLAD_PATH)

build-glad: $(GLAD_PATH)
