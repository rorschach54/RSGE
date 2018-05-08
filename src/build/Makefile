CFLAGS=-g -I $(INCLUDE_SRC) -I $(GLAD_INCLUDE) -I $(LIST_INCLUDE) -I $(LOGC_INCLUDE)
LIBRARY_CFLAGS=-c $(CFLAGS)

build: build-glad build-library build-examples build-ide

-include $(BUILD_SRC)/os/$(CONFIG_OS)/Makefile
-include $(BUILD_SRC)/platform/$(CONFIG_PLATFORM)/Makefile
include $(BUILD_SRC)/target/config/Makefile
include $(BUILD_SRC)/target/examples/Makefile
include $(BUILD_SRC)/target/glad/Makefile
include $(BUILD_SRC)/target/ide/Makefile
include $(BUILD_SRC)/target/library/Makefile