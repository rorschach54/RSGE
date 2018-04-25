#ifndef __RSGE_SND_SOURCE_H_
#define __RSGE_SND_SOURCE_H_ 1

#include <AL/al.h>
#include <AL/alc.h>
#include <rsge/snd/buffer.h>
#include <rsge/config.h>
#include <rsge/error.h>
#include <linmath.h>

#include <stdlib.h>
#include <stdio.h>

#if CONFIG_AUDIO_BACKEND == libaudio
#include <audio/wave.h>
#elif CONFIG_AUDIO_BACKEND == alut
#include <AL/alut.h>
#endif

typedef struct {
    /**
     * \brief The OpenAL source ID.
     */
    ALuint source;
    
    /**
     * \brief The position of the source.
     */
    vec3 pos;
    
    /**
     * \brief The velocity of the source.
     */
    vec3 velocity;
    
    /**
     * \brief The pitch of the source.
     */
    float pitch;
    
    /**
     * \brief The gain of the source.
     */
    float gain;
} rsge_audio_source_t;

/**
 * \fn rsge_error_e rsge_audio_source_create(rsge_audio_source_t* source)
 * \brief Creates a new sound source.
 * \param source The pointer to store the source.
 * \return An error code.
 */
rsge_error_e rsge_audio_source_create(rsge_audio_source_t* source);

/**
 * \fn rsge_error_e rsge_audio_source_destroy(rsge_audio_source_t* source)
 * \brief Destroys the sound source.
 * \param source The pointer to the source.
 * \return An error code.
 */
rsge_error_e rsge_audio_source_destroy(rsge_audio_source_t* source);

/**
 * \fn rsge_error_e rsge_audio_source_play(rsge_audio_source_t* source,rsge_audio_buffer_t* buffer)
 * \brief Plays an audio buffer.
 * \param source The pointer to the source.
 * \param buffer The buffer to play.
 * \return An error code.
 */
rsge_error_e rsge_audio_source_play(rsge_audio_source_t* source,rsge_audio_buffer_t* buffer);

/**
 * \fn rsge_error_e rsge_audio_source_isplaying(rsge_audio_source_t* source,int* status)
 * \brief Is the source playing.
 * \param source The pointer to the source.
 * \param status The ponter to store the status.
 * \return An error code.
 */
rsge_error_e rsge_audio_source_isplaying(rsge_audio_source_t* source,int* status);

#endif