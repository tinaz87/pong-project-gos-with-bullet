#pragma once

#include "InitAudioWrapper.h"

class AudioWrapper{


public:	

	AudioWrapper(InitAudioWrapper* i_initAudioWrapper);

	//HRESULT InitializeXAudio();

	HRESULT UpdateAudio(float fElapsedTime);

	HRESULT PrepareAudio(const LPCWSTR fileName);

	HRESULT PlayAudio();
	
	void PauseAudio();

	void ResumeAudio();

	void StopAudio();

	void CleanupAudio();

	~AudioWrapper(){


		pSourceVoice->DestroyVoice();

		delete pbSampleData;

		//initAudioWrapper->~InitAudioWrapper();

	}

	inline bool IsPlaing() const{

		return audioState.isPlaing;
	}

	inline bool IsStopped() const{

		return  audioState.isStopped;
	}

	inline bool IsPaused() const{

		return audioState.isPaused;
	}

	inline bool IsLoopActive() const{

		return audioState.Loop;
	}

	inline bool IsInitialized() const{

		return audioState.isInitialized;
	}

	inline FLOAT32 GetVolume() const{

		return audioState.volume;
	}

	inline bool Is3DSoundEnable() const{

		return audioState.sound3DEnabled;
	}

	inline bool IsReady() const{

		return audioState.isReady;
	}

	inline void setEmitterPosition(const D3DXVECTOR3& iEPsition){

		initAudioWrapper->vEmitterPos = iEPsition;
	}

	inline void setListenerPosition(const D3DXVECTOR3& iLPsition){

		initAudioWrapper->vListenerPos = iLPsition;
	}

	inline void TranslateEmitterPosition(const D3DXVECTOR3& iEPsition){

		initAudioWrapper->vEmitterPos += iEPsition;

	}

	inline void TranslateListenerPosition(const D3DXVECTOR3& iLPsition){

		initAudioWrapper->vListenerPos += iLPsition;

	}

private:

	InitAudioWrapper* initAudioWrapper;

	struct STATE_AUDIO{

		STATE_AUDIO(){

			isInitialized = false;
			isPaused = false;
			isPlaing = false;
			isStopped = false;
			Loop = false;
			volume = 0;
			sound3DEnabled = false;
			isReady = false;


		}

		 bool		isInitialized;

		 bool		isReady;

		 bool		isPlaing;
		 bool		isPaused;
		 bool		isStopped;

		 bool		Loop;
		 FLOAT32	volume;

		 bool		sound3DEnabled;

	};

	STATE_AUDIO audioState;

	WCHAR wavFilePath[ MAX_PATH ];

	void initialize3DSound(XAUDIO2_DEVICE_DETAILS& details);

	HRESULT hr;


	//// XAudio2
	//IXAudio2* pXAudio2;
	//IXAudio2MasteringVoice* pMasteringVoice;
	//IXAudio2SubmixVoice* pSubmixVoice;

	IXAudio2SourceVoice* pSourceVoice;

	//IUnknown* pReverbEffect;
	BYTE* pbSampleData;
	XAUDIO2_BUFFER buffer;


	// 3D
	//X3DAUDIO_HANDLE x3DInstance;
	int nFrameToApply3DAudio;

	/*DWORD dwChannelMask;
	UINT32 nChannels;*/

	//X3DAUDIO_DSP_SETTINGS dspSettings;
	//X3DAUDIO_LISTENER listener;
	//X3DAUDIO_EMITTER emitter;
	//X3DAUDIO_CONE emitterCone;


	//D3DXVECTOR3 vListenerPos;
	//D3DXVECTOR3 vEmitterPos;

	/*float fListenerAngle;
	bool  fUseListenerCone;
	bool  fUseInnerRadius;
	bool  fUseRedirectToLFE;

	FLOAT32 emitterAzimuths[INPUTCHANNELS];
	FLOAT32 matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];*/


};