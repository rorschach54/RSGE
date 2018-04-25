#include <rsge/snd/buffer.h>
#include <string.h>

rsge_error_e rsge_audio_buffer_create(rsge_audio_buffer_t* buffer) {
    memset(buffer,0,sizeof(rsge_audio_buffer_t));
    
    ALCenum alError;
    
    alGenBuffers(1,&buffer->buffer);
    alError = alGetError();
    if(alError != AL_NO_ERROR) return RSGE_ERROR_OPENAL;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_audio_buffer_destroy(rsge_audio_buffer_t* buffer) {
    alDeleteBuffers(1,&buffer->buffer);
    memset(buffer,0,sizeof(rsge_audio_buffer_t));
    return RSGE_ERROR_NONE;
}