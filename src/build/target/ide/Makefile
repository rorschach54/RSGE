IDE_OBJS=$(IDE_BIN)/main.o

$(IDE_BIN)/%.o: $(IDE_SRC)/%.c $(INCLUDE_SRC)
	@echo "Compiling $<"
	@mkdir -p $(@D)
	@$(CC) -o $@ $< $(IDE_CFLAGS)

include $(BUILD_SRC)/target/ide/os/$(CONFIG_OS)/Makefile
include $(BUILD_SRC)/target/ide/platform/$(CONFIG_PLATFORM)/Makefile