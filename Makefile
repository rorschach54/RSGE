BIN=./bin
DEPS=./deps
SRC=./src

EXAMPLES_BIN=$(BIN)/examples
IDE_BIN=$(BIN)/ide
LIBRARY_BIN=$(BIN)/library

GLAD_PATH=$(DEPS)/glad
GLAD_BIN=$(GLAD_PATH)/bin
GLAD_INCLUDE=$(GLAD_PATH)/include
GLAD_SRC=$(GLAD_PATH)/src

LIST_PATH=$(DEPS)/list
LIST_BIN=$(LIST_PATH)/bin
LIST_INCLUDE=$(LIST_PATH)/src
LIST_SRC=$(LIST_PATH)/src

LOGC_PATH=$(DEPS)/log.c
LOGC_BIN=$(LOGC_PATH)/bin
LOGC_INCLUDE=$(LOGC_PATH)/include
LOGC_SRC=$(LOGC_PATH)/src

ASSETS_SRC=$(SRC)/assets
BUILD_SRC=$(SRC)/build
CONFIG_SRC=$(SRC)/config
EXAMPLES_SRC=$(SRC)/examples
IDE_SRC=$(SRC)/ide
INCLUDE_SRC=$(SRC)/include
LIBRARY_SRC=$(SRC)/library
TOOLS_SRC=$(SRC)/tools

-include $(CONFIG_SRC)/Makefile

all: build

clean:
	@rm -rf $(BIN) $(CONFIG_SRC)/Makefile $(INCLUDE_SRC)/rsge/config.h $(GLAD_PATH) $(LIST_BIN) $(LOGC_BIN)

doc:
	@doxygen $(CONFIG_SRC)/doxygen.cfg

include $(BUILD_SRC)/Makefile
