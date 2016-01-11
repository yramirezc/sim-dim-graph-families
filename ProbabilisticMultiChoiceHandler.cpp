/*
 * ProbabilisticMultiChoiceHandler.cpp
 *
 *  Created on: 24/2/2015
 *      Author: Yunior Ramirez-Cruz
 */

#include "ProbabilisticMultiChoiceHandler.h"

//-------------------------------------------------------------------------------------------

ProbabilisticMultiChoiceHandler::ProbabilisticMultiChoiceHandler(bool StartRandomGenerator)
{
	if (StartRandomGenerator)
		srand(time(NULL));
}

//-------------------------------------------------------------------------------------------

ProbabilisticMultiChoiceHandler::~ProbabilisticMultiChoiceHandler()
{
	// Nothing to destroy
}

//-------------------------------------------------------------------------------------------

unsigned int ProbabilisticMultiChoiceHandler::GetChoice(vector<double> &PreferenceLevels, set<unsigned int> &InvalidChoices)
{
	// Initialize multi-choice decision structures
	vector<unsigned int> Choices;
	vector<unsigned int> Thresholds;
	double SumRelevantPrefLev = 0;
	for (unsigned int i = 0; i < PreferenceLevels.size(); i++)
		if (InvalidChoices.find(i) == InvalidChoices.end())
		{
			Choices.push_back(i);
			SumRelevantPrefLev += PreferenceLevels.at(i);
		}
	unsigned int TkCount = 0, EstTkCount = 1000000;
	for (unsigned int i = 0; i < Choices.size(); i++)
	{
		unsigned int ChTkCount = round(EstTkCount * (PreferenceLevels.at(Choices.at(i)) / SumRelevantPrefLev));
		TkCount += ChTkCount;
		Thresholds.push_back(TkCount);
	}
	// Probabilistic decision (inefficient implementation, replace with binary search)
	unsigned int tk = rand() % TkCount;
	if (tk < Thresholds.at(0))
		return 0;
	for (unsigned int ch = 1; ch < Thresholds.size(); ch++)
		if (tk >= Thresholds.at(ch - 1) && tk < Thresholds.at(ch))
			return Choices.at(ch);
	return Choices.at(Thresholds.size() - 1);
}

//-------------------------------------------------------------------------------------------
