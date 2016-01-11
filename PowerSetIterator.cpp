/*
 * PowerSetIterator.cpp
 *
 *  Created on: 12/2/2015
 *      Author: Yunior Ramirez-Cruz
 */

#include "PowerSetIterator.h"

//-------------------------------------------------------------------------------------------

PowerSetIterator::PowerSetIterator(unsigned int PoolSz)
{
	PoolSize = PoolSz;
	CurrentSetDescriptor.clear();
	for (unsigned int i = 0; i < PoolSize; i++)
		CurrentSetDescriptor.push_back(false);
	LastGenerated = false;
}

//-------------------------------------------------------------------------------------------

PowerSetIterator::~PowerSetIterator()
{
	// Nothing to destroy
}

//-------------------------------------------------------------------------------------------

bool PowerSetIterator::NextSet(set<unsigned int> &Set)
{
	Set.clear();
	for (unsigned int i = 0; i < CurrentSetDescriptor.size(); i++)
		if (CurrentSetDescriptor.at(i))
			Set.insert(i);
	return NextDescriptor();
}

//-------------------------------------------------------------------------------------------

bool PowerSetIterator::NextDescriptor()
{
	if (LastGenerated)
		return false;
	int i = CurrentSetDescriptor.size() - 1;
	while (i >= 0)
		if (CurrentSetDescriptor.at(i))
		{
			CurrentSetDescriptor.at(i) = false;
			i--;
		}
		else
			break;
	if (i >= 0)
		CurrentSetDescriptor.at(i) = true;
	else
		LastGenerated = true;
	return true;
}

//-------------------------------------------------------------------------------------------

void PowerSetIterator::Reset()
{
	CurrentSetDescriptor.clear();
	for (unsigned int i = 0; i < PoolSize; i++)
		CurrentSetDescriptor.push_back(false);
	LastGenerated = false;
}

//-------------------------------------------------------------------------------------------
