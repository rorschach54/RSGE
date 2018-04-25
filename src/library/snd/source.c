#include <rsge/snd/source.h>
#include <string.h>

extern ALCcontext* rsge_audio_openal_ctx;
extern ALCdevice* rsge_audio_openal_dev;

rsge_error_e rsge_audio_source_create(rsge_audio_source_t* source) {
    memset(source,0,sizeof(rsge_audio_source_t));
    source->pitch = source->gain = 1;
    
    ALCenum alError;
    
    alGenSources(1,&source->source);
    alError = alGetError();
    if(alError != AL_NO_ERROR) return RSGE_ERROR_OPENAL;
    
    alSourcef(source->source,AL_PITCH,source->pitch);
    alError = alGetError();
    if(alError != AL_NO_ERROR) return RSGE_ERROR_OPENAL;
    
    alSourcef(source->source,AL_GAIN,source->gain);
    alError = alGetError();
    if(alError != AL_NO_ERROR) return RSGE_ERROR_OPENAL;
    
    alSource3f(source->source,AL_POSITION,source->pos[0],source->pos[1],source->pos[2]);
    alError = alGetError();
    if(alError != AL_NO_ERROR) return RSGE_ERROR_OPENAL;
    
    alSource3f(source->source,AL_VELOCITY,source->velocity[0],source->velocity[1],source->velocity[2]);
    alError = alGetError();
    if(alError != AL_NO_ERROR) return RSGE_ERROR_OPENAL;
    
    alSourcei(source->source,AL_LOOPING,AL_FALSE);
    alError = alGetError();
    if(alError != AL_NO_ERROR) return RSGE_ERROR_OPENAL;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_audio_source_destroy(rsge_audio_source_t* source) {
    alDeleteSources(1,&source->source);
    memset(source,0,sizeof(rsge_audio_source_t));
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_audio_source_play(rsge_audio_source_t* source,rsge_audio_buffer_t* buffer) {
    ALCenum alError;
    alSourcei(source->source,AL_BUFFER,buffer->buffer);
    alError = alGetError();
    if(alError != AL_NO_ERROR) return RSGE_ERROR_OPENAL;
    
    alSourcePlay(source->source);
    alError = alGetError();
    if(alError != AL_NO_ERROR) return RSGE_ERROR_OPENAL;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_audio_source_isplaying(rsge_audio_source_t* source,int* status) {
    ALCenum alError;
    ALint source_state;
    alGetSourcei(source->source,AL_SOURCE_STATE,&source_state);
    alError = alGetError();
    if(alError != AL_NO_ERROR) return RSGE_ERROR_OPENAL;
    *status = source_state == AL_PLAYING;
    return RSGE_ERROR_NONE;
}