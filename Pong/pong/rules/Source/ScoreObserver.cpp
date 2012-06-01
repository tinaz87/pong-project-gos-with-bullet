#include "ScoreObserver.h"

ScoreData::ScoreData(const unsigned iscoreA,const unsigned iscoreB)
	:scoreA(iscoreA),scoreB(iscoreB)
{

}

ScoreData::~ScoreData()
{

}

const unsigned int ScoreData::getScoreA() const 
{
	return scoreA;
}

const unsigned int ScoreData::getScoreB() const
{
	return scoreB;
}

void ScoreData::setScoreA(const unsigned int iscoreA)
{
	scoreA= iscoreA;
}

void ScoreData::setScoreB(const unsigned int iscoreB)
{
	scoreB= iscoreB;
}
