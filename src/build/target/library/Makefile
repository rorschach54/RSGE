LIBRARY_OBJS += $(LIBRARY_BIN)/cl/context.o $(LIBRARY_BIN)/cl/init.o $(LIBRARY_BIN)/cl/program.o
ifeq ($(CONFIG_USE_FREETYPE),y)
LIBRARY_OBJS += $(LIBRARY_BIN)/gfx/font/freetype.o
endif
LIBRARY_OBJS += $(LIBRARY_BIN)/gfx/image/bmp.o
ifeq ($(CONFIG_USE_LIBPNG),y)
LIBRARY_OBJS += $(LIBRARY_BIN)/gfx/image/libpng.o
endif
ifeq ($(CONFIG_USE_ASSIMP),y)
LIBRARY_OBJS += $(LIBRARY_BIN)/gfx/model/assimp.o
endif
LIBRARY_OBJS += $(LIBRARY_BIN)/gfx/model/obj.o
LIBRARY_OBJS += $(LIBRARY_BIN)/gfx/objects/fb.o $(LIBRARY_BIN)/gfx/objects/texbuff.o \
$(LIBRARY_BIN)/gfx/objects/texture.o $(LIBRARY_BIN)/gfx/objects/unifbuff.o \
$(LIBRARY_BIN)/gfx/objects/vert.o
LIBRARY_OBJS += $(LIBRARY_BIN)/gfx/uniforms/camera.o $(LIBRARY_BIN)/gfx/uniforms/material.o \
$(LIBRARY_BIN)/gfx/uniforms/object.o $(LIBRARY_BIN)/gfx/uniforms/point-light.o \
$(LIBRARY_BIN)/gfx/uniforms/render.o $(LIBRARY_BIN)/gfx/uniforms/scene.o
LIBRARY_OBJS += $(LIBRARY_BIN)/gfx/camera.o $(LIBRARY_BIN)/gfx/elglr.o $(LIBRARY_BIN)/gfx/font.o \
$(LIBRARY_BIN)/gfx/image.o $(LIBRARY_BIN)/gfx/lighting.o $(LIBRARY_BIN)/gfx/model.o \
$(LIBRARY_BIN)/gfx/screenshot.o $(LIBRARY_BIN)/gfx/shader.o $(LIBRARY_BIN)/gfx/shape.o \
$(LIBRARY_BIN)/gfx/surface.o
LIBRARY_OBJS += $(LIBRARY_BIN)/net/ftp.o $(LIBRARY_BIN)/net/http.o $(LIBRARY_BIN)/net/https.o
LIBRARY_OBJS += $(LIBRARY_BIN)/os/$(CONFIG_OS)/callbacks.o $(LIBRARY_BIN)/os/$(CONFIG_OS)/input.o \
$(LIBRARY_BIN)/os/$(CONFIG_OS)/main.o $(LIBRARY_BIN)/os/$(CONFIG_OS)/settings.o \
$(LIBRARY_BIN)/os/$(CONFIG_OS)/window.o
LIBRARY_OBJS += $(LIBRARY_BIN)/physics/collision/box.o $(LIBRARY_BIN)/physics/collision/object.o \
$(LIBRARY_BIN)/physics/collision/shape.o $(LIBRARY_BIN)/physics/collision/sphere.o
LIBRARY_OBJS += $(LIBRARY_BIN)/physics/init.o $(LIBRARY_BIN)/physics/motion-state.o $(LIBRARY_BIN)/physics/raycasting.o \
$(LIBRARY_BIN)/physics/rigid-body.o $(LIBRARY_BIN)/physics/transform.o $(LIBRARY_BIN)/physics/world.o
LIBRARY_OBJS += $(LIBRARY_BIN)/snd/buffer.o $(LIBRARY_BIN)/snd/init.o $(LIBRARY_BIN)/snd/source.o
LIBRARY_OBJS += $(LIBRARY_BIN)/ui/widgets/button.o
LIBRARY_OBJS += $(LIBRARY_BIN)/assets.o $(LIBRARY_BIN)/input.o $(LIBRARY_BIN)/settings.o \
$(LIBRARY_BIN)/ui.o $(LIBRARY_BIN)/version.o
LIBRARY_OBJS += $(LIBRARY_BIN)/assets/main.o

LIBRARY_OBJS += $(LIST_BIN)/list.o $(LIST_BIN)/list_iterator.o $(LIST_BIN)/list_node.o
LIBRARY_OBJS += $(LOGC_BIN)/log.o

$(LIBRARY_BIN)/assets/%.c: $(LIBRARY_SRC)/assets/%.assets.json
	@echo "Generating assets $<"
	@mkdir -p $(@D)
	@node $(TOOLS_SRC)/mkassets.js $< $(INCLUDE_SRC)/rsge/assets/ rsge/assets/ $@

$(LIBRARY_BIN)/assets/%.o: $(LIBRARY_SRC)/assets/%.assets.json
	@echo "Generating assets $<"
	@mkdir -p $(@D)
	@node $(TOOLS_SRC)/mkassets.js $< $(INCLUDE_SRC)/rsge/assets/ rsge/assets/ $@.c
	@echo "Compiling $@.c"
	@mkdir -p $(@D)
	@$(CC) -o $@ $@.c $(LIBRARY_CFLAGS) -w

$(GLAD_BIN)/%.o: $(GLAD_SRC)/%.c
	@echo "Compiling $<"
	@mkdir -p $(@D)
	@$(CC) -o $@ $< $(LIBRARY_CFLAGS)

$(LIST_BIN)/%.o: $(LIST_SRC)/%.c
	@echo "Compiling $<"
	@mkdir -p $(@D)
	@$(CC) -o $@ $< $(LIBRARY_CFLAGS)

$(LOGC_BIN)/%.o: $(LOGC_SRC)/%.c
	@echo "Compiling $<"
	@mkdir -p $(@D)
	@$(CC) -o $@ $< $(LIBRARY_CFLAGS) -DLOG_USE_COLOR

$(LIBRARY_BIN)/%.o: $(LIBRARY_SRC)/%.cc $(INCLUDE_SRC)
	@echo "Compiling $<"
	@mkdir -p $(@D)
	@$(CXX) -o $@ $< $(LIBRARY_CXXFLAGS)

$(LIBRARY_BIN)/%.o: $(LIBRARY_SRC)/%.c $(INCLUDE_SRC)
	@echo "Compiling $<"
	@mkdir -p $(@D)
	@$(CC) -o $@ $< $(LIBRARY_CFLAGS)

include $(BUILD_SRC)/target/library/os/$(CONFIG_OS)/Makefile
include $(BUILD_SRC)/target/library/platform/$(CONFIG_PLATFORM)/Makefile
