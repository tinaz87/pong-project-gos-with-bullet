
#include "sound.h"



Sound::Sound():loop(false),data(NULL),m_ready(false){
	
	// Initialize OpenAL and clear the error bit.

	alutInit(NULL, 0);
	alGetError();


	listenerPos[0] =-2;
	listenerPos[1] = 0;
	listenerPos[2] = 0;


	listenerVel[0] = 0;
	listenerVel[1] = 0;
	listenerVel[2] = 0;

	listenerOri[0] = listenerOri[1] = 0;
	listenerOri[2] = -1.0f;
	listenerOri[3] = 0.0f;
	listenerOri[4] = 1.0f;
	listenerOri[5] = 0.0f;


	sourcePos[0] = -2.0f;
	sourcePos[1] = sourcePos[2] = 0;

	sourceVel[0] = sourceVel[1] = sourceVel[2] = 0;

	alListenerfv(AL_POSITION,listenerPos);
	alListenerfv(AL_VELOCITY,listenerVel);
	alListenerfv(AL_ORIENTATION,listenerOri);

	alGetError(); // clear any error messages

}

Sound::~Sound(){

	alDeleteSources(1,&Source);
	alDeleteBuffers(1,&Buffer);

	ALCcontext* Context=alcGetCurrentContext();

	//Get device for active context
	ALCdevice* Device=alcGetContextsDevice(Context);

	//Disable context
	alcMakeContextCurrent(NULL);

	//Release context(s)
	alcDestroyContext(Context);

	//Close device
	alcCloseDevice(Device);

}

bool Sound::isStopped()const{

	return getSourceState() == AL_STOPPED;
}


bool Sound::isPlaying()const{
	
	return  getSourceState() == AL_PLAYING;
}

bool Sound::isPaused()const{


	return getSourceState() == AL_PAUSED;
}



void Sound::LoadSound(char* filename){

	// Generate buffers, or else no sound will happen!
	// Load wav data into a buffer.

	alGenBuffers(1, &Buffer);

	if(alGetError() != AL_NO_ERROR)
		return ;

	
	alutLoadWAVFile(filename, &format, &data, &size, &freq, &loop);
	alBufferData(Buffer, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	// Bind the buffer with the source.

	alGenSources(1, &Source);

	if(alGetError() != AL_NO_ERROR)
		return ;

	alSourcei (Source, AL_BUFFER,   Buffer   );
	alSourcef (Source, AL_PITCH,    1.0      );
	alSourcef (Source, AL_GAIN,     1.0      );
	alSourcefv(Source, AL_POSITION, sourcePos);
	alSourcefv(Source, AL_VELOCITY, sourceVel);
	alSourcei (Source, AL_LOOPING,  loop     );

	// Do another error check and return.

	if(alGetError() != AL_NO_ERROR)
		return ;

	m_ready = true;

}


bool Sound::isReady()const {

	return m_ready;
}

void Sound::Update(const float time){

	
	

}


void Sound::ResetPitch(){

	alSourcef (Source, AL_PITCH,    1.0      );
	
}

void Sound::ResetVolume(){

	alSourcef (Source, AL_GAIN,     1.0      );
}
void Sound::IncreasePitch(){

	float pitch;

	alGetSourcef(Source,AL_PITCH,&pitch);

	pitch = pitch + 0.000001f;
	if (pitch > 2.0f)
	{
		pitch =2.0f;
	}


	alSourcef (Source, AL_PITCH,     pitch      );

}

void Sound::DecreasePitch(){

	float pitch;

	alGetSourcef(Source,AL_PITCH,&pitch);

	pitch = pitch - 0.000001f;
	if (pitch < 0.5f)
	{
		pitch =0.5f;
	}

	alSourcef (Source, AL_PITCH,     pitch    );

}

void Sound::IncreaseVolume(){

	float volume;

	alGetSourcef(Source,AL_GAIN,&volume);

	volume = volume + 0.000001f;
	if (volume > 1.0f)
	{
		volume =1.0f;
	}


	alSourcef (Source, AL_GAIN,     volume      );

}

void Sound::DecreaseVolume(){

	float volume;

	alGetSourcef(Source,AL_GAIN,&volume);

	volume = volume - 0.000001f;
	if (volume < 0.0f)
	{
		volume = 0.0f;
	}



	alSourcef (Source, AL_GAIN,     volume    );

}

void Sound::SetVolume(const float ivolume /* = 1.0f */){

	alSourcef (Source, AL_GAIN,     ivolume      );

}

void Sound::SetPitch(const float ipitch /* = 1.0f */){

	alSourcef (Source, AL_PITCH,    ipitch      );

}

void Sound::LoopSound(const bool iloop){

	loop = iloop;
	alSourcei (Source, AL_LOOPING,  iloop     );

}

const bool Sound::inLoop()const{

	return loop;

}

void Sound::SetListenerPosition(const float x,const float y,const float z){

	listenerPos[0] = x;
	listenerPos[1] = y;
	listenerPos[2] = z;

	alListenerfv(AL_POSITION,listenerPos);

}

void Sound::TranslateListenerPosition(const float x,const float y,const float z){

	listenerPos[0] += x;
	listenerPos[1] += y;
	listenerPos[2] += z;

	alListenerfv(AL_POSITION,listenerPos);

}

void Sound::SetSourcePosition(const float x,const float y,const float z){

	sourcePos[0] = x;
	sourcePos[1] = y;
	sourcePos[2] = z;

	alSourcefv(Source, AL_POSITION, sourcePos);
}


void Sound::TranslateSourcePosition(const float x,const float y,const float z){

	sourcePos[0] += x;
	sourcePos[1] += y;
	sourcePos[2] += z;

	alSourcefv(Source, AL_POSITION, sourcePos);

}

void Sound::SetSourceVelocity(const float x,const float y,const float z){

	sourceVel[0] = x;
	sourceVel[1] = y;
	sourceVel[2] = z;

	alSourcefv(Source, AL_VELOCITY, sourceVel);
}

void Sound::ChangeSourceVelocity(const float x,const float y,const float z){

	sourceVel[0] += x;
	sourceVel[1] += y;
	sourceVel[2] += z;

	alSourcefv(Source, AL_VELOCITY, sourceVel);


}
void Sound::Play(){

	
	alSourcePlay(Source);

}

void Sound::Stop(){

	alSourceStop(Source);
	alSourceRewind(Source);
	
}

void Sound::Pause(){

	alSourcePause(Source);
}

void Sound::Resume(){

	Sound::Play();
}

const ALint Sound::getSourceState()const{

	ALint status;

	alGetSourcei(Source,AL_SOURCE_STATE,&status);

	return status;
}