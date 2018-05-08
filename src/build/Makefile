CFLAGS=-g -I $(INCLUDE_SRC) -I $(LIST_INCLUDE) -I $(LOGC_INCLUDE) -DGLEW_STATIC
LIBRARY_CFLAGS=-c $(CFLAGS)

build: build-library build-examples build-ide

-include $(BUILD_SRC)/os/$(CONFIG_OS)/Makefile
-include $(BUILD_SRC)/platform/$(CONFIG_PLATFORM)/Makefile
include $(BUILD_SRC)/target/config/Makefile
include $(BUILD_SRC)/target/examples/Makefile
include $(BUILD_SRC)/target/ide/Makefile
include $(BUILD_SRC)/target/library/Makefile
