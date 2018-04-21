ifeq ($(CONFIG_USE_FREETYPE),y)
LIBRARY_OBJS += $(LIBRARY_BIN)/gfx/font/freetype.o
endif
ifeq ($(CONFIG_USE_LIBPNG),y)
LIBRARY_OBJS += $(LIBRARY_BIN)/gfx/image/libpng.o
endif
LIBRARY_OBJS += $(LIBRARY_BIN)/gfx/model/xml.o
LIBRARY_OBJS += $(LIBRARY_BIN)/gfx/camera.o $(LIBRARY_BIN)/gfx/font.o $(LIBRARY_BIN)/gfx/image.o $(LIBRARY_BIN)/gfx/lighting.o $(LIBRARY_BIN)/gfx/model.o $(LIBRARY_BIN)/gfx/shader.o $(LIBRARY_BIN)/gfx/shape.o $(LIBRARY_BIN)/gfx/surface.o
LIBRARY_OBJS += $(LIBRARY_BIN)/platform/$(CONFIG_PLATFORM)/main.o $(LIBRARY_BIN)/platform/$(CONFIG_PLATFORM)/settings.o
LIBRARY_OBJS += $(LIBRARY_BIN)/snd/init.o
LIBRARY_OBJS += $(LIBRARY_BIN)/ui/xml/widget.o
LIBRARY_OBJS += $(LIBRARY_BIN)/ui/widgets/button.o $(LIBRARY_BIN)/ui/widgets/label.o
LIBRARY_OBJS += $(LIBRARY_BIN)/ui/surface.o $(LIBRARY_BIN)/ui/widget.o
LIBRARY_OBJS += $(LIBRARY_BIN)/assets.o $(LIBRARY_BIN)/settings.o $(LIBRARY_BIN)/ui.o $(LIBRARY_BIN)/version.o

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

$(LIBRARY_BIN)/%.o: $(LIBRARY_SRC)/%.c $(INCLUDE_SRC)
	@echo "Compiling $<"
	@mkdir -p $(@D)
	@$(CC) -o $@ $< $(LIBRARY_CFLAGS)

include $(BUILD_SRC)/target/library/platform/$(CONFIG_PLATFORM)/Makefile
