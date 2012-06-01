#pragma once
#include "ObjectId.h"

class ScoreData
{
public:
	ScoreData(const unsigned scoreA,const unsigned scoreB);
	~ScoreData();

	const unsigned int getScoreA() const;
	const unsigned int getScoreB() const;
	void setScoreA(const unsigned int iscoreA);
	void setScoreB(const unsigned int iscoreB);

private:
	unsigned int scoreA;
	unsigned int scoreB;
};


class ScoreObserver
{
public:
	virtual ~ScoreObserver() {}
	virtual void ScoreEvent(const ScoreData& score)= 0;
};