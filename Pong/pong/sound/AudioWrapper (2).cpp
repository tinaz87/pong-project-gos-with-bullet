#include "AudioWrapper.h"
#include <cassert>


AudioWrapper::AudioWrapper(InitAudioWrapper* i_initAudioWrapper):/*pXAudio2(nullptr),pMasteringVoice(nullptr)
							,*/hr(NULL),pSourceVoice(nullptr)
							,pbSampleData(nullptr),audioState()
{
	assert(i_initAudioWrapper);
	initAudioWrapper = i_initAudioWrapper;
	audioState.isInitialized = true;
	audioState.sound3DEnabled = true;

}



//--------------------------------------------------------------------------------------
// Helper function to try to find the location of a media file
//--------------------------------------------------------------------------------------
HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename )
{
	
	bool bFound = false;

	if( NULL == strFilename || strFilename[0] == 0 || NULL == strDestPath || cchDest < 10 )
		return E_INVALIDARG;

	// Get the exe name, and exe path
	WCHAR strExePath[MAX_PATH] = {0};
	WCHAR strExeName[MAX_PATH] = {0};
	WCHAR* strLastSlash = NULL;
	GetModuleFileName( NULL, strExePath, MAX_PATH );
	strExePath[MAX_PATH - 1] = 0;
	strLastSlash = wcsrchr( strExePath, TEXT( '\\' ) );
	if( strLastSlash )
	{
		wcscpy_s( strExeName, MAX_PATH, &strLastSlash[1] );

		// Chop the exe name from the exe path
		*strLastSlash = 0;

		// Chop the .exe from the exe name
		strLastSlash = wcsrchr( strExeName, TEXT( '.' ) );
		if( strLastSlash )
			*strLastSlash = 0;
	}

	wcscpy_s( strDestPath, cchDest, strFilename );
	if( GetFileAttributes( strDestPath ) != 0xFFFFFFFF )
		return S_OK;

	// Search all parent directories starting at .\ and using strFilename as the leaf name
	WCHAR strLeafName[MAX_PATH] = {0};
	wcscpy_s( strLeafName, MAX_PATH, strFilename );

	WCHAR strFullPath[MAX_PATH] = {0};
	WCHAR strFullFileName[MAX_PATH] = {0};
	WCHAR strSearch[MAX_PATH] = {0};
	WCHAR* strFilePart = NULL;

	GetFullPathName( L".", MAX_PATH, strFullPath, &strFilePart );
	if( strFilePart == NULL )
		return E_FAIL;

	while( strFilePart != NULL && *strFilePart != '\0' )
	{
		swprintf_s( strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, strLeafName );
		if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
		{
			wcscpy_s( strDestPath, cchDest, strFullFileName );
			bFound = true;
			break;
		}

		swprintf_s( strFullFileName, MAX_PATH, L"%s\\%s\\%s", strFullPath, strExeName, strLeafName );
		if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
		{
			wcscpy_s( strDestPath, cchDest, strFullFileName );
			bFound = true;
			break;
		}

		swprintf_s( strSearch, MAX_PATH, L"%s\\..", strFullPath );
		GetFullPathName( strSearch, MAX_PATH, strFullPath, &strFilePart );
	}
	if( bFound )
		return S_OK;

	// On failure, return the file as the path but also return an error code
	wcscpy_s( strDestPath, cchDest, strFilename );
	
	return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
}


HRESULT AudioWrapper::PrepareAudio(const LPCWSTR fileName){

	if(!audioState.isInitialized)
		return E_FAIL;

	// Se non ho inizializzato nessuna Voice
	if( pSourceVoice )
    {
        pSourceVoice->Stop( 0 );
        pSourceVoice->DestroyVoice();
        pSourceVoice = 0;
    }

	WCHAR strFilePath[ MAX_PATH ];
    

    wcscpy_s( wavFilePath, MAX_PATH, L"Media\\Wavs\\" );
    wcscat_s( wavFilePath, MAX_PATH, fileName );

	// Trovo il File Audio.
    if( FAILED( hr = FindMediaFileCch( strFilePath, MAX_PATH, wavFilePath ) ) )
	{
		wprintf( L"Failed to find media file: %s\n", wavFilePath );
		return hr;
	}

	// Carico il file audio
	CWaveFile wav;

	if( FAILED( hr = wav.Open( strFilePath, NULL, WAVEFILE_READ ) ) )
	{
		wprintf( L"Failed to open media file: %s\n", wavFilePath );
		return hr;
	}

	// Get format of wave file
    WAVEFORMATEX* pwfx = wav.GetFormat();

    // Calculate how many bytes and samples are in the wave
    DWORD cbWaveSize = wav.GetSize();

    // Read the sample data into memory
    SAFE_DELETE_ARRAY( pbSampleData );

    pbSampleData = new BYTE[ cbWaveSize ];


	if (FAILED(hr=wav.Read(pbSampleData,cbWaveSize,&cbWaveSize)) )
	{
		wprintf( L"Failed to read media file: %s\n", wavFilePath );
		return hr;
	}

	//
    // Play the wave using a source voice that sends to both the submix and mastering voices
    //
	XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
	sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LOW PASS FILTER
    sendDescriptors[0].pOutputVoice = initAudioWrapper->pMasteringVoice;
    sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LOW PASS FILTER reverb-path -- omit for better performance at the cost of less realistic occlusion
    sendDescriptors[1].pOutputVoice = initAudioWrapper->pSubmixVoice;
    const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };

	if (FAILED(hr = initAudioWrapper->pXAudio2->CreateSourceVoice(&pSourceVoice,pwfx,0,2.0f,NULL,&sendList)))
	{
		wprintf( L"Failed to CreateSourceVoice of : %s\n", wavFilePath );
		return hr;
	}

	// Submit the wave sample data using an XAUDIO2_BUFFER structure
	ZeroMemory(&buffer,sizeof(buffer));
    buffer.pAudioData = pbSampleData;
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes = cbWaveSize;
    buffer.LoopCount = 0; // Numero di Loop max 255 : XAUDIO2_LOOP_INFINITE

    
	if (FAILED(hr = pSourceVoice->SubmitSourceBuffer( &buffer )))
	{
		wprintf( L"Failed to SubmitSourceBuffer of : %s\n", wavFilePath );
		return hr;
	}

  
	audioState.isReady = true;
	audioState.isStopped = true;

    nFrameToApply3DAudio = 0;

    return S_OK;

}



HRESULT AudioWrapper::UpdateAudio(float fElapsedTime){

	if( !audioState.isInitialized )
        return S_FALSE;

	if( nFrameToApply3DAudio == 0 ){


		// Cotrollo se io ascoltatore mi sono spostato
		if( initAudioWrapper->vListenerPos.x != initAudioWrapper->listener.Position.x || initAudioWrapper->vListenerPos.z != initAudioWrapper->listener.Position.z){

			D3DXVECTOR3 vDelta = initAudioWrapper->vListenerPos - initAudioWrapper->listener.Position;

            initAudioWrapper->fListenerAngle = float( atan2( vDelta.x, vDelta.z ) );

            vDelta.y = 0.0f;
            D3DXVec3Normalize( &vDelta, &vDelta );

            initAudioWrapper->listener.OrientFront.x = vDelta.x;
            initAudioWrapper->listener.OrientFront.y = 0.f;
            initAudioWrapper->listener.OrientFront.z = vDelta.z;

		}

		// TODO: Da togliere
		/*Controlli Vari*/
		if (initAudioWrapper->fUseListenerCone) 
        {
            initAudioWrapper->listener.pCone = (X3DAUDIO_CONE*)&Listener_DirectionalCone;
        }
        else
        {
            initAudioWrapper->listener.pCone = NULL;
        }
        if (initAudioWrapper->fUseInnerRadius)
        {
            initAudioWrapper->emitter.InnerRadius = 2.0f;
            initAudioWrapper->emitter.InnerRadiusAngle = X3DAUDIO_PI/4.0f;
        }
        else
        {
            initAudioWrapper->emitter.InnerRadius = 0.0f;
            initAudioWrapper->emitter.InnerRadiusAngle = 0.0f;
        }

		if( fElapsedTime > 0 )
        {
            D3DXVECTOR3 lVelocity = ( initAudioWrapper->vListenerPos - initAudioWrapper->listener.Position ) / fElapsedTime;
            initAudioWrapper->listener.Position = initAudioWrapper->vListenerPos;
            initAudioWrapper->listener.Velocity = lVelocity;

            D3DXVECTOR3 eVelocity = ( initAudioWrapper->vEmitterPos - initAudioWrapper->emitter.Position ) / fElapsedTime;
            initAudioWrapper->emitter.Position = initAudioWrapper->vEmitterPos;
            initAudioWrapper->emitter.Velocity = eVelocity;
        }

        DWORD dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
            | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_LPF_REVERB
            | X3DAUDIO_CALCULATE_REVERB;
        if (initAudioWrapper->fUseRedirectToLFE)
        {
            // On devices with an LFE channel, allow the mono source data
            // to be routed to the LFE destination channel.
            dwCalcFlags |= X3DAUDIO_CALCULATE_REDIRECT_TO_LFE;
        }

		 X3DAudioCalculate( initAudioWrapper->x3DInstance, &initAudioWrapper->listener, &initAudioWrapper->emitter, dwCalcFlags,&initAudioWrapper->dspSettings );

		IXAudio2SourceVoice* voice = pSourceVoice;
        if( voice )
        {
            //// Apply X3DAudio generated DSP settings to XAudio2
            //voice->SetFrequencyRatio( initAudioWrapper->dspSettings.DopplerFactor );
            //voice->SetOutputMatrix( initAudioWrapper->pMasteringVoice, INPUTCHANNELS, initAudioWrapper->nChannels,initAudioWrapper->matrixCoefficients );

            //voice->SetOutputMatrix(initAudioWrapper->pSubmixVoice, 1, 1, &initAudioWrapper->dspSettings.ReverbLevel);

            //XAUDIO2_FILTER_PARAMETERS FilterParametersDirect = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * initAudioWrapper->dspSettings.LPFDirectCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
            //voice->SetOutputFilterParameters(initAudioWrapper->pMasteringVoice, &FilterParametersDirect);
            //XAUDIO2_FILTER_PARAMETERS FilterParametersReverb = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * initAudioWrapper->dspSettings.LPFReverbCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
            //voice->SetOutputFilterParameters(initAudioWrapper->pSubmixVoice, &FilterParametersReverb);
        }

	}

	XAUDIO2_VOICE_STATE state;

	pSourceVoice->GetState(&state);

	if (state.SamplesPlayed == 0)
	{
		StopAudio();
	}

	nFrameToApply3DAudio++;
    nFrameToApply3DAudio &= 1;

    return S_OK;


}

void AudioWrapper::StopAudio(){

	pSourceVoice->Stop(0,XAUDIO2_COMMIT_NOW);
	pSourceVoice->FlushSourceBuffers();

	buffer.PlayBegin = 0; // Restart Sound

	if (FAILED(hr = pSourceVoice->SubmitSourceBuffer( &buffer )))
	{
		wprintf( L"StopAudio: Failed to SubmitSourceBuffer" );
		return;
	}

	audioState.isStopped = true;
	audioState.isPaused = false;
	audioState.isPlaing = false;

	wprintf( L" Audio Stopped");
}


void AudioWrapper::ResumeAudio(){

	if( !audioState.isInitialized || !audioState.isPaused )
		return;

	audioState.isPaused = false;
	audioState.isPlaing = true;

	wprintf( L" Audio Resume");
	initAudioWrapper->pXAudio2->StartEngine();
}

void AudioWrapper::PauseAudio()
{
    if( !audioState.isInitialized || !audioState.isPlaing )
        return;

	audioState.isPaused = true;
	audioState.isPlaing = false;

	wprintf( L" Audio Paused");
    initAudioWrapper->pXAudio2->StopEngine();
	
}


HRESULT AudioWrapper::PlayAudio(){

	// Se non è gia in Play allora falla  partire
	if ( ( !audioState.isPlaing && !audioState.isPaused ) && FAILED(hr = pSourceVoice->Start( 0 )))
	{
		wprintf( L"Failed to Start  : %s\n", wavFilePath );
		return hr;
	}


	audioState.isStopped = false;
	// Se tento di fare play mentre sono in pausa
	if ( audioState.isPaused ){
		wprintf( L" Audio is Paused");
		audioState.isPlaing = false;
		

	}else{
		wprintf( L" Audio Plaing");
		audioState.isPlaing = true;
		
	}

	return hr;

}
//-----------------------------------------------------------------------------
// Releases XAudio2
//-----------------------------------------------------------------------------
void AudioWrapper::CleanupAudio()
{
    if( !audioState.isInitialized )
        return;

    if( pSourceVoice )
    {
        pSourceVoice->DestroyVoice();
        pSourceVoice = NULL;
    }

    if( initAudioWrapper->pSubmixVoice )
    {
        initAudioWrapper->pSubmixVoice->DestroyVoice();
        initAudioWrapper->pSubmixVoice = NULL;
    }

    if( initAudioWrapper->pMasteringVoice )
    {
        initAudioWrapper->pMasteringVoice->DestroyVoice();
        initAudioWrapper->pMasteringVoice = NULL;
    }

    initAudioWrapper->pXAudio2->StopEngine();
    SAFE_RELEASE( initAudioWrapper->pXAudio2 );
    SAFE_RELEASE( initAudioWrapper->pReverbEffect );

    SAFE_DELETE_ARRAY( pbSampleData );

    CoUninitialize();

    audioState.isInitialized = false;
}
