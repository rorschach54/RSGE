ifeq ($(CONFIG_USE_FREETYPE),y)
LIBRARY_OBJS += $(LIBRARY_BIN)/gfx/font/freetype.o
endif
LIBRARY_OBJS += $(LIBRARY_BIN)/gfx/font.o $(LIBRARY_BIN)/gfx/shader.o $(LIBRARY_BIN)/gfx/shape.o $(LIBRARY_BIN)/gfx/surface.o
LIBRARY_OBJS += $(LIBRARY_BIN)/platform/$(CONFIG_PLATFORM)/main.o

$(GLAD_BIN)/%.o: $(GLAD_SRC)/%.c
	@echo "Compiling $<"
	@mkdir -p $(@D)
	@$(CC) -o $@ $< $(LIBRARY_CFLAGS)

$(LIBRARY_BIN)/%.o: $(LIBRARY_SRC)/%.c $(INCLUDE_SRC)
	@echo "Compiling $<"
	@mkdir -p $(@D)
	@$(CC) -o $@ $< $(LIBRARY_CFLAGS)

include $(BUILD_SRC)/target/library/platform/$(CONFIG_PLATFORM)/Makefile
