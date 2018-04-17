CFLAGS=-g -I $(INCLUDE_SRC) -I $(GLAD_INCLUDE) -I $(LIST_INCLUDE) -std=gnu99
LIBRARY_CFLAGS=-c $(CFLAGS)

build: build-glad build-library build-examples

-include $(BUILD_SRC)/platform/$(CONFIG_PLATFORM)/Makefile
include $(BUILD_SRC)/target/config/Makefile
include $(BUILD_SRC)/target/examples/Makefile
include $(BUILD_SRC)/target/glad/Makefile
include $(BUILD_SRC)/target/library/Makefile