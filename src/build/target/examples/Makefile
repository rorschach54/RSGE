EXAMPLES=$(EXAMPLES_BIN)/model $(EXAMPLES_BIN)/net \
$(EXAMPLES_BIN)/physics $(EXAMPLES_BIN)/simple

include $(BUILD_SRC)/target/examples/os/$(CONFIG_OS)/Makefile
include $(BUILD_SRC)/target/examples/platform/$(CONFIG_PLATFORM)/Makefile

build-examples: $(EXAMPLES)
