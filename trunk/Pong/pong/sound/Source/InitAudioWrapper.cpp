#include "InitAudioWrapper.h"

// Must match order of g_PRESET_NAMES
XAUDIO2FX_REVERB_I3DL2_PARAMETERS g_PRESET_PARAMS[ NUM_PRESETS ] =
{
    XAUDIO2FX_I3DL2_PRESET_FOREST,
    XAUDIO2FX_I3DL2_PRESET_DEFAULT,
    XAUDIO2FX_I3DL2_PRESET_GENERIC,
    XAUDIO2FX_I3DL2_PRESET_PADDEDCELL,
    XAUDIO2FX_I3DL2_PRESET_ROOM,
    XAUDIO2FX_I3DL2_PRESET_BATHROOM,
    XAUDIO2FX_I3DL2_PRESET_LIVINGROOM,
    XAUDIO2FX_I3DL2_PRESET_STONEROOM,
    XAUDIO2FX_I3DL2_PRESET_AUDITORIUM,
    XAUDIO2FX_I3DL2_PRESET_CONCERTHALL,
    XAUDIO2FX_I3DL2_PRESET_CAVE,
    XAUDIO2FX_I3DL2_PRESET_ARENA,
    XAUDIO2FX_I3DL2_PRESET_HANGAR,
    XAUDIO2FX_I3DL2_PRESET_CARPETEDHALLWAY,
    XAUDIO2FX_I3DL2_PRESET_HALLWAY,
    XAUDIO2FX_I3DL2_PRESET_STONECORRIDOR,
    XAUDIO2FX_I3DL2_PRESET_ALLEY,
    XAUDIO2FX_I3DL2_PRESET_CITY,
    XAUDIO2FX_I3DL2_PRESET_MOUNTAINS,
    XAUDIO2FX_I3DL2_PRESET_QUARRY,
    XAUDIO2FX_I3DL2_PRESET_PLAIN,
    XAUDIO2FX_I3DL2_PRESET_PARKINGLOT,
    XAUDIO2FX_I3DL2_PRESET_SEWERPIPE,
    XAUDIO2FX_I3DL2_PRESET_UNDERWATER,
    XAUDIO2FX_I3DL2_PRESET_SMALLROOM,
    XAUDIO2FX_I3DL2_PRESET_MEDIUMROOM,
    XAUDIO2FX_I3DL2_PRESET_LARGEROOM,
    XAUDIO2FX_I3DL2_PRESET_MEDIUMHALL,
    XAUDIO2FX_I3DL2_PRESET_LARGEHALL,
    XAUDIO2FX_I3DL2_PRESET_PLATE,
};




InitAudioWrapper::InitAudioWrapper():pXAudio2(nullptr),pMasteringVoice(nullptr){}



HRESULT InitAudioWrapper::InitializeXAudio(){

	//
	// Initialize XAudio2
	//
	CoInitializeEx( NULL, COINIT_MULTITHREADED );// COINIT_MULTITHREADED
	// Initializes the thread for multithreaded object concurrency .

	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	if( FAILED( hr = XAudio2Create( &pXAudio2, flags ) ) )
	{
		wprintf( L"Failed to init XAudio2 engine: %#X\n", hr );
		return hr;
	}

	//
	// Create a mastering voice
	//
	if( FAILED( hr = pXAudio2->CreateMasteringVoice( &pMasteringVoice ) ) )
	{
		wprintf( L"Failed creating mastering voice: %#X\n", hr );
		SAFE_RELEASE( pXAudio2 );
		CoUninitialize();
		return hr;
	}


	// Check device details to make sure it's within our sample supported parameters
	XAUDIO2_DEVICE_DETAILS details;
	if( FAILED( hr = pXAudio2->GetDeviceDetails( 0, &details ) ) )
	{
		SAFE_RELEASE( pXAudio2 );
		return hr;
	}

	if( details.OutputFormat.Format.nChannels > OUTPUTCHANNELS )
	{
		SAFE_RELEASE( pXAudio2 );
		return E_FAIL;
	}

	dwChannelMask = details.OutputFormat.dwChannelMask;
	nChannels = details.OutputFormat.Format.nChannels;

	//
	// Create reverb effect
	//
	flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2FX_DEBUG;
#endif

	if( FAILED( hr = XAudio2CreateReverb( &pReverbEffect, flags ) ) )
	{
		SAFE_RELEASE( pXAudio2 );
		return hr;
	}

	//
	// Create a submix voice
	//

	// Performance tip: you need not run global FX with the sample number
	// of channels as the final mix.  For example, this sample runs
	// the reverb in mono mode, thus reducing CPU overhead.
	XAUDIO2_EFFECT_DESCRIPTOR effects[] = { { pReverbEffect, TRUE, 1 } };
	XAUDIO2_EFFECT_CHAIN effectChain = { 1, effects };

	if( FAILED( hr = pXAudio2->CreateSubmixVoice( &pSubmixVoice, 1,
		details.OutputFormat.Format.nSamplesPerSec, 0, 0,
		NULL, &effectChain ) ) )
	{
		SAFE_RELEASE( pXAudio2 );
		SAFE_RELEASE( pReverbEffect );
		return hr;
	}

	// Set default FX params
	XAUDIO2FX_REVERB_PARAMETERS native;
	ReverbConvertI3DL2ToNative( &g_PRESET_PARAMS[0], &native );
	pSubmixVoice->SetEffectParameters( 0, &native, sizeof( native ) );

	//
	// Initialize X3DAudio
	//  Speaker geometry configuration on the final mix, specifies assignment of channels
	//  to speaker positions, defined as per WAVEFORMATEXTENSIBLE.dwChannelMask
	//
	//  SpeedOfSound - speed of sound in user-defined world units/second, used
	//  only for doppler calculations, it must be >= FLT_MIN
	//
	const float SPEEDOFSOUND = X3DAUDIO_SPEED_OF_SOUND;

	X3DAudioInitialize( details.OutputFormat.dwChannelMask, SPEEDOFSOUND, x3DInstance );

	initialize3DSound(details);

}

void InitAudioWrapper::initialize3DSound(XAUDIO2_DEVICE_DETAILS& details){


	ZeroMemory(matrixCoefficients,sizeof(matrixCoefficients));

	vListenerPos = D3DXVECTOR3( 0, 0, float( ZMAX ) );
	vEmitterPos = D3DXVECTOR3( 0, 0, float( ZMAX ) );

	fListenerAngle = 0;
	fUseListenerCone = TRUE;
	fUseInnerRadius = TRUE;
	fUseRedirectToLFE = ((details.OutputFormat.dwChannelMask & SPEAKER_LOW_FREQUENCY) != 0);

	//
	// Setup 3D audio structs
	//
	listener.Position = vListenerPos;
	listener.OrientFront = D3DXVECTOR3( 0, 0, 1 );
	listener.OrientTop = D3DXVECTOR3( 0, 1, 0 );
	listener.pCone = (X3DAUDIO_CONE*)&Listener_DirectionalCone;

	emitter.pCone = &emitterCone;
	emitter.pCone->InnerAngle = 0.0f;
	// Setting the inner cone angles to X3DAUDIO_2PI and
	// outer cone other than 0 causes
	// the emitter to act like a point emitter using the
	// INNER cone settings only.
	emitter.pCone->OuterAngle = 0.0f;
	// Setting the outer cone angles to zero causes
	// the emitter to act like a point emitter using the
	// OUTER cone settings only.
	emitter.pCone->InnerVolume = 0.0f;
	emitter.pCone->OuterVolume = 1.0f;
	emitter.pCone->InnerLPF = 0.0f;
	emitter.pCone->OuterLPF = 1.0f;
	emitter.pCone->InnerReverb = 0.0f;
	emitter.pCone->OuterReverb = 1.0f;

	emitter.Position = vEmitterPos;
	emitter.OrientFront = D3DXVECTOR3( 0, 0, 1 );
	emitter.OrientTop = D3DXVECTOR3( 0, 1, 0 );
	emitter.ChannelCount = INPUTCHANNELS;
	emitter.ChannelRadius = 1.0f;
	emitter.pChannelAzimuths = emitterAzimuths;

	// Use of Inner radius allows for smoother transitions as
	// a sound travels directly through, above, or below the listener.
	// It also may be used to give elevation cues.
	emitter.InnerRadius = 2.0f;
	emitter.InnerRadiusAngle = X3DAUDIO_PI/4.0f;;

	emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
	emitter.pLFECurve    = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;
	emitter.pLPFDirectCurve = NULL; // use default curve
	emitter.pLPFReverbCurve = NULL; // use default curve
	emitter.pReverbCurve    = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;
	emitter.CurveDistanceScaler = 14.0f;
	emitter.DopplerScaler = 1.0f;

	dspSettings.SrcChannelCount = INPUTCHANNELS;
	dspSettings.DstChannelCount = nChannels;
	dspSettings.pMatrixCoefficients = matrixCoefficients;
}
