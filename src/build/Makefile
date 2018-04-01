CFLAGS=-I $(INCLUDE_SRC) -I $(GLAD_INCLUDE) -I $(LOGC_SRC)
LIBRARY_CFLAGS=-c $(CFLAGS) -std=gnu99

build: build-glad build-library

-include $(BUILD_SRC)/platform/$(CONFIG_PLATFORM)/Makefile
include $(BUILD_SRC)/target/config/Makefile
include $(BUILD_SRC)/target/glad/Makefile
include $(BUILD_SRC)/target/library/Makefile
