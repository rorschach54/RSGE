#include <rsge/snd/init.h>

ALCcontext* rsge_audio_openal_ctx = NULL;
ALCdevice* rsge_audio_openal_dev = NULL;

rsge_error_e rsge_audio_init() {
	ALboolean enumeration = alcIsExtensionPresent(NULL,"ALC_ENUMERATION_EXT");
	if(enumeration == AL_FALSE) return RSGE_ERROR_OPENAL;

	const ALchar* defaultDeviceName = alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER);
	rsge_audio_openal_dev = alcOpenDevice(defaultDeviceName);
	if(!rsge_audio_openal_dev) return RSGE_ERROR_OPENAL;

	alGetError();

	rsge_audio_openal_ctx = alcCreateContext(rsge_audio_openal_dev,NULL);
	if(!alcMakeContextCurrent(rsge_audio_openal_ctx)) return RSGE_ERROR_OPENAL;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_audio_uninit() {
	if(rsge_audio_openal_ctx != NULL) {
		alcMakeContextCurrent(NULL);
		alcDestroyContext(rsge_audio_openal_ctx);
	}
	if(rsge_audio_openal_dev != NULL) alcCloseDevice(rsge_audio_openal_dev);
	return RSGE_ERROR_NONE;
}
