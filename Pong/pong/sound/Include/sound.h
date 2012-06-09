#pragma once
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <al.h>
#include <alc.h>
#include <alut.h>


#define NUM_BUFFERS 1
#define NUM_SOURCES 1
#define NUM_ENVIRONMENTS 1


class Sound{

public:

	Sound();

	~Sound();

	void LoadSound(char* filename);

	void Play();
	void Stop();
	void Pause();
	void Resume();

	void Update(const float time);

	void SetSourcePosition(const float x,const float y,const float z);
	void TranslateSourcePosition(const float x,const float y,const float z);

	void SetSourceVelocity(const float x,const float y,const float z);

	// increase/decrease source velocity
	void ChangeSourceVelocity(const float x,const float y,const float z);

	void SetListenerPosition(const float x,const float y,const float z);
	void TranslateListenerPosition(const float x,const float y,const float z);

	void SetVolume(const float ivolume = 1.0f);
	void IncreaseVolume();
	void DecreaseVolume();

	void IncreasePitch();
	void DecreasePitch();

	void ResetPitch();
	void ResetVolume();

	void SetPitch(const float ipitch = 1.0f);

	void LoopSound(const bool iloop);

	const bool inLoop()const;

	bool isReady()const;
	bool isStopped() const;
	bool isPlaying() const;
	bool isPaused() const;

private:

	const ALint getSourceState() const;

	// Listener
	ALfloat	listenerPos[3];
	ALfloat listenerVel[3];
	ALfloat listenerOri[6];
	
	// Source
	ALfloat	sourcePos[3];
	ALfloat	sourceVel[3];


	ALuint  Buffer;
	ALuint  Source;
	ALuint  environment[NUM_ENVIRONMENTS];

	ALsizei size,freq;
	ALenum  format;
	ALvoid  *data;
	ALboolean loop;

	bool m_ready;
};