AR=ar
CC=gcc
CXX=g++
LD=g++
PKG_CONFIG=pkg-config

LIBRARY_LIBS=glfw3 libconfig libxml-2.0 bullet

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
endif
endif

IDE_LIBS=libconfig gtk+-3.0

IDE_CFLAGS=$(CFLAGS) -c $(shell $(PKG_CONFIG) --cflags $(IDE_LIBS))
IDE_LDFLAGS=$(shell $(PKG_CONFIG) --libs $(IDE_LIBS))

EXAMPLES_CFLAGS=$(CFLAGS) $(shell $(PKG_CONFIG) --cflags $(LIBRARY_LIBS)) $(shell curl-config --cflags)
EXAMPLES_CXXFLAGS=$(CFLAGS) $(shell $(PKG_CONFIG) --cflags $(LIBRARY_LIBS)) $(shell curl-config --cflags)

LIBRARY_CFLAGS += $(shell $(PKG_CONFIG) --cflags $(LIBRARY_LIBS)) $(shell curl-config --cflags)
LIBRARY_LDFLAGS += $(shell $(PKG_CONFIG) --libs $(LIBRARY_LIBS)) $(shell curl-config --libs) -ldl -lm -lopenal -lGLU -lOpenCL

ifeq ($(CONFIG_LINKMODE),shared)
LIBRARY_CFLAGS += -fpic
LIBRARY_CXXFLAGS += -fpic
LIBRARY_LDFLAGS += -shared
else
EXAMPLE_CFLAGS += -DGLEW_STATIC $(shell $(PKG_CONFIG) --cflags --static glew) 
LIBRARY_CFLAGS += -fpic $(shell $(PKG_CONFIG) --cflags --static glew) -DGLEW_STATIC
LIBRARY_CXXFLAGS += -fpic $(shell $(PKG_CONFIG) --cflags --static glew) -DGLEW_STATIC
LIBRARY_LDFLAGS += -shared $(shell $(PKG_CONFIG) --libs --static glew)
endif

LIBRARY_CXXFLAGS=$(LIBRARY_CFLAGS) -fPIC -std=gnu++11

IDE_CFLAGS += -std=gnu99
EXAMPLES_CFLAGS += -std=gnu99
LIBRARY_CFLAGS += -std=gnu99
