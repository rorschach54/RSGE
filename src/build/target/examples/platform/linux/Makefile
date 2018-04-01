ifeq ($(CONFIG_LINKMODE),shared)
LIB_OUT=$(BIN)/librsge.so
else
ifeq ($(CONFIG_LINKMODE),static)
LIB_OUT=$(BIN)/librsge.a
endif
endif

$(EXAMPLES_BIN)/%: $(EXAMPLES_SRC)/%.c $(LIB_OUT)
	@echo "Building example $@"
	@mkdir -p $(@D)
	@$(CC) -o $@ $< $(CFLAGS) -L$(BIN) $(LIB_OUT)
