ifeq ($(CONFIG_PLATFORM),linux)
ifeq ($(CONFIG_LINKMODE),)
CONFIG_LINKMODE=shared
endif
endif

ifeq ($(CONFIG_USE_FREETYPE),)
CONFIG_USE_FREETYPE=y
endif

ifeq ($(CONFIG_USE_LIBPNG),)
CONFIG_USE_LIBPNG=y
endif

ifeq ($(CONFIG_AUDIO_BACKEND),)
CONFIG_AUDIO_BACKEND=libaudio
endif

config:
	@printf "#ifndef __RSGE_CONFIG_H_\n" > $(INCLUDE_SRC)/rsge/config.h
	@printf "#define __RSGE_CONFIG_H_ 1\n\n" >> $(INCLUDE_SRC)/rsge/config.h
	@printf "#define CONFIG_PLATFORM $(CONFIG_PLATFORM)\n" >> $(INCLUDE_SRC)/rsge/config.h
	@printf "#define CONFIG_AUDIO_BACKEND $(CONFIG_AUDIO_BACKEND)\n" >> $(INCLUDE_SRC)/rsge/config.h
ifeq ($(CONFIG_USE_FREETYPE),y)
	@printf "#define CONFIG_USE_FREETYPE 1\n" >> $(INCLUDE_SRC)/rsge/config.h
else
	@printf "#define CONFIG_USE_FREETYPE 0\n" >> $(INCLUDE_SRC)/rsge/config.h
endif
ifeq ($(CONFIG_USE_LIBPNG),y)
	@printf "#define CONFIG_USE_LIBPNG 1\n" >> $(INCLUDE_SRC)/rsge/config.h
else
	@printf "#define CONFIG_USE_LIBPNG 0\n" >> $(INCLUDE_SRC)/rsge/config.h
endif
	@printf "\n#endif" >> $(INCLUDE_SRC)/rsge/config.h
	@printf "CONFIG_PLATFORM=$(CONFIG_PLATFORM)\n" > $(CONFIG_SRC)/Makefile
	@printf "CONFIG_USE_FREETYPE=$(CONFIG_USE_FREETYPE)\n" >> $(CONFIG_SRC)/Makefile
	@printf "CONFIG_USE_LIBPNG=$(CONFIG_USE_LIBPNG)\n" >> $(CONFIG_SRC)/Makefile
	@printf "CONFIG_AUDIO_BACKEND=$(CONFIG_AUDIO_BACKEND)\n" >> $(CONFIG_SRC)/Makefile
ifeq ($(CONFIG_PLATFORM),linux)
	@printf "CONFIG_LINKMODE=$(CONFIG_LINKMODE)" >> $(CONFIG_SRC)/Makefile
endif