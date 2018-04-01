$(GLAD_PATH):
	@echo "Setting up GLAD"
	@glad --generator=c --extensions=GL_EXT_framebuffer_multisample,GL_EXT_texture_filter_anisotropic --out-path=$(GLAD_PATH)

build-glad: $(GLAD_PATH)
