BIN=./bin
DEPS=./deps
SRC=./src

LIBRARY_BIN=$(BIN)/library

GLAD_PATH=$(DEPS)/glad
GLAD_BIN=$(GLAD_PATH)/bin
GLAD_INCLUDE=$(GLAD_PATH)/include
GLAD_SRC=$(GLAD_PATH)/src

BUILD_SRC=$(SRC)/build
CONFIG_SRC=$(SRC)/config
INCLUDE_SRC=$(SRC)/include
LIBRARY_SRC=$(SRC)/library

-include $(CONFIG_SRC)/Makefile

all: build

clean:
	@rm -rf $(BIN) $(CONFIG_SRC)/Makefile $(INCLUDE_SRC)/rsge/config.h $(GLAD_BIN)

include $(BUILD_SRC)/Makefile
