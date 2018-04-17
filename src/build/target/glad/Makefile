$(GLAD_PATH):
	@echo "Setting up GLAD"
	@glad --api "gl=4.6" --generator=c --extensions=GL_EXT_framebuffer_multisample,GL_EXT_texture_filter_anisotropic,GL_ARB_vertex_array_object,GL_ARB_get_program_binary,GL_ARB_vertex_attrib_binding --out-path=$(GLAD_PATH)

build-glad: $(GLAD_PATH)
