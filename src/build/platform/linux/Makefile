AR=ar
CC=gcc
LD=gcc
PKG_CONFIG=pkg-config

LIBRARY_LIBS=glfw3 libconfig libxml-2.0

ifeq ($(CONFIG_USE_FREETYPE),y)
LIBRARY_LIBS += freetype2
endif

ifeq ($(CONFIG_USE_LIBPNG),y)
LIBRARY_LIBS += libpng
endif

ifeq ($(CONFIG_AUDIO_BACKEND),libaudio)
LIBRARY_LDFLAGS += -laudio
else
ifeq ($(CONFIG_AUDIO_BACKEND),alut)
LIBRARY_LDFLAGS += -lalut
else
$(error "Invalid audio backend, $(CONFIG_AUDIO_BACKEND), for OpenAL")
endif
endif

LIBRARY_CFLAGS += $(shell $(PKG_CONFIG) --cflags $(LIBRARY_LIBS))
LIBRARY_LDFLAGS += $(shell $(PKG_CONFIG) --libs $(LIBRARY_LIBS)) -ldl -lm -lopenal -lGLU

ifeq ($(CONFIG_LINKMODE),shared)
LIBRARY_CFLAGS += -fpic
LIBRARY_LDFLAGS += -shared
endif
