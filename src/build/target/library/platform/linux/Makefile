LIBRARY_OBJS += $(GLAD_BIN)/glad.o

ifeq ($(CONFIG_LINKMODE),shared)
$(BIN)/librsge.so: $(LIBRARY_OBJS) $(INCLUDE_SRC)
	@echo "Linking librsge.so"
	@$(LD) -o $@ $(LIBRARY_OBJS) $(LIBRARY_LDFLAGS)
else
ifeq ($(CONFIG_LINKMODE),static)
$(BIN)/librsge.a: $(LIBRARY_OBJS) $(INCLUDE_SRC)
	@echo "Linking librsge.a"
	@$(AR) rc -o $@ $(LIBRARY_OBJS)
endif
endif

ifeq ($(CONFIG_LINKMODE),shared)
build-library: $(BIN)/librsge.so
else
ifeq ($(CONFIG_LINKMODE),static)
build-library: $(BIN)/librsge.a
endif
endif
