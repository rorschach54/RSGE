AR=ar
CC=gcc
LD=gcc
PKG_CONFIG=pkg-config

LIBRARY_LIBS=glfw3

ifeq ($(CONFIG_USE_FREETYPE),y)
LIBRARY_LIBS += freetype2
endif

LIBRARY_CFLAGS += $(shell $(PKG_CONFIG) --cflags $(LIBRARY_LIBS))
LIBRARY_LDFLAGS += $(shell $(PKG_CONFIG) --libs $(LIBRARY_LIBS)) -ldl -lm

ifeq ($(CONFIG_LINKMODE),shared)
LIBRARY_CFLAGS += -fpic
LIBRARY_LDFLAGS += -shared
endif
