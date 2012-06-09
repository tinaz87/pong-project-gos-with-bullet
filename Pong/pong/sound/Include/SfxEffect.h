#pragma once
#include "ObjectProperty.h"
#include "sound.h"


class SfxEffect :public ObjectProperty{

public :

	static const StringHash SFX_EFFECT_ID;

	SfxEffect(const ObjectId& objectId);

	~SfxEffect();

	const bool isToPlay() const;

	void setToPlay();
	//const bool isToStop() const;
	void setSound(char* const  filename);

	void setDescription(std::string& desc){}

	void PrepareAudio();

	const bool isReady()const;

	void Play();

	void setLoopActive(const bool iactive);

	void Update(float time);

	const bool isInPlay() const;



private:

	bool m_toPlay;

	bool m_Loop;
	
	std::string m_description;

	Sound* m_audio;

	char* m_fileName;

};