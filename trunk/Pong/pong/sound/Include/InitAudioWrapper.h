#pragma once
#include <windows.h>
#include <strsafe.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <conio.h>

#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>

#include "d3dx9math.h"

#include "SDKwavefile.h"


//--------------------------------------------------------------------------------------
// Helper macros
//--------------------------------------------------------------------------------------
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

//-----------------------------------------------------------------------------
// Global defines
//-----------------------------------------------------------------------------
#define INPUTCHANNELS 1  // number of source channels
#define OUTPUTCHANNELS 8 // maximum number of destination channels supported in this sample

#define NUM_PRESETS 30

// Specify sound cone to add directionality to listener for artistic effect:
// Emitters behind the listener are defined here to be more attenuated,
// have a lower LPF cutoff frequency,
// yet have a slightly higher reverb send level.
static const X3DAUDIO_CONE Listener_DirectionalCone = { X3DAUDIO_PI*5.0f/6.0f, X3DAUDIO_PI*11.0f/6.0f, 1.0f, 0.75f, 0.0f, 0.25f, 0.708f, 1.0f };

// Specify LFE level distance curve such that it rolls off much sooner than
// all non-LFE channels, making use of the subwoofer more dramatic.
static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[3] = { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve          = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };

// Specify reverb send level distance curve such that reverb send increases
// slightly with distance before rolling off to silence.
// With the direct channels being increasingly attenuated with distance,
// this has the effect of increasing the reverb-to-direct sound ratio,
// reinforcing the perception of distance.
static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[3] = { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve          = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

// Constants to define our world space
const INT           XMIN = -1000;
const INT           XMAX = 1000;
const INT           ZMIN = -1000;
const INT           ZMAX = 1000;



class InitAudioWrapper{

public :
	
	InitAudioWrapper();

	HRESULT InitializeXAudio();

	~InitAudioWrapper(){

		if(pSubmixVoice)
			pSubmixVoice->DestroyVoice();

		if(pMasteringVoice)
			pMasteringVoice->DestroyVoice();

		


		if(emitterAzimuths)
			delete[] emitterAzimuths;

		if(matrixCoefficients)
		delete[] matrixCoefficients;

		if(pReverbEffect)
			SAFE_RELEASE(pReverbEffect);

		if(pXAudio2)
			SAFE_RELEASE(pXAudio2);	



		CoUninitialize();
	
	
	}

private:

	void initialize3DSound(XAUDIO2_DEVICE_DETAILS& details);

	// XAudio2
	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasteringVoice;
	IXAudio2SubmixVoice* pSubmixVoice;

	//3D
	X3DAUDIO_HANDLE x3DInstance;


	IUnknown* pReverbEffect;

	D3DXVECTOR3 vListenerPos;
	D3DXVECTOR3 vEmitterPos;

	X3DAUDIO_DSP_SETTINGS dspSettings;
	X3DAUDIO_LISTENER listener;
	X3DAUDIO_EMITTER emitter;
	X3DAUDIO_CONE emitterCone;

	DWORD dwChannelMask;
	UINT32 nChannels;

	float fListenerAngle;
	bool  fUseListenerCone;
	bool  fUseInnerRadius;
	bool  fUseRedirectToLFE;

	FLOAT32 emitterAzimuths[INPUTCHANNELS];
	FLOAT32 matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];


	HRESULT hr;

	friend class AudioWrapper;

};
