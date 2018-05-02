#ifndef __RSGE_SND_INIT_H_
#define __RSGE_SND_INIT_H_ 1

/** \file src/include/rsge/snd/init.h
 * \brief Audio initialization
 * \author Spaceboy Ross
 */

#include <AL/al.h>
#include <AL/alc.h>
#include <rsge/config.h>
#include <rsge/error.h>

#include <stdlib.h>
#include <stdio.h>

#if CONFIG_AUDIO_BACKEND == libaudio
#include <audio/wave.h>
#elif CONFIG_AUDIO_BACKEND == alut
#include <AL/alut.h>
#endif

/**
 * \fn rsge_error_e rsge_audio_init()
 * \brief Initializes the game engine's audio component, don't run this yourself because this is called automatically.
 * \return An error code.
 */
rsge_error_e rsge_audio_init();

/**
 * \fn rsge_error_e rsge_audio_uninit()
 * \brief Uninitializes the game engine's audio component, don't run this yourself because this is called automatically.
 * \return An error code.
 */
rsge_error_e rsge_audio_uninit();

#endif
