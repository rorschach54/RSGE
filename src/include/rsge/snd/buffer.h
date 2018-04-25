#ifndef __RSGE_SND_BUFFER_H_
#define __RSGE_SND_BUFFER_H_ 1

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

typedef struct {
    /**
     * \brief The OpenAL buffer ID.
     */
    ALuint buffer;
} rsge_audio_buffer_t;

/**
 * \fn rsge_error_e rsge_audio_buffer_create(rsge_audio_buffer_t* buffer)
 * \brief Creates a new audio buffer.
 * \param buffer The pointer to store the audio buffer.
 * \return An error code.
 */
rsge_error_e rsge_audio_buffer_create(rsge_audio_buffer_t* buffer);

/**
 * \fn rsge_error_e rsge_audio_buffer_destroy(rsge_audio_buffer_t* buffer)
 * \brief Destroyes the audio buffer.
 * \param buffer The pointer to store the audio buffer.
 * \return An error code.
 */
rsge_error_e rsge_audio_buffer_destroy(rsge_audio_buffer_t* buffer);

#endif