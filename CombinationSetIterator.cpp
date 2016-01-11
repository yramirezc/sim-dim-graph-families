/*
 * CombinationSetIterator.cpp
 *
 *  Created on: 12/2/2015
 *      Author: Yunior Ramirez-Cruz
 */

#include "CombinationSetIterator.h"

//-------------------------------------------------------------------------------------------

CombinationSetIterator::CombinationSetIterator(unsigned int PoolSz, unsigned int CombSz)
{
	CurrentCombination.clear();
	PoolSize = PoolSz;
	CombinationSize = CombSz;
	for (unsigned int i = 0; i < CombinationSize; i++)
		CurrentCombination.push_back(PoolSize - CombinationSize + i);
	LastGenerated = false;
}

//-------------------------------------------------------------------------------------------

CombinationSetIterator::~CombinationSetIterator()
{
}

//-------------------------------------------------------------------------------------------

bool CombinationSetIterator::NextCombination(vector<unsigned int> &Combination)
{
	Combination = CurrentCombination;
	return NextCombAux(0);
}

//-------------------------------------------------------------------------------------------

bool CombinationSetIterator::NextCombAux(unsigned int item_ord)
{
	if (LastGenerated)
		return false;
	if (item_ord == CurrentCombination.size() - 1 && CurrentCombination.at(item_ord) == item_ord)
		LastGenerated = true;
	else if (CurrentCombination.at(item_ord) > item_ord)
		CurrentCombination.at(item_ord) = CurrentCombination.at(item_ord) - 1;
	else
	{
		NextCombAux(item_ord + 1);
		CurrentCombination.at(item_ord) = CurrentCombination.at(item_ord + 1) - 1;
	}
	return true;
}

//-------------------------------------------------------------------------------------------

void CombinationSetIterator::Reset()
{
	CurrentCombination.clear();
	for (unsigned int i = 0; i < CombinationSize; i++)
		CurrentCombination.push_back(PoolSize - CombinationSize + i);
	LastGenerated = false;
}

//-------------------------------------------------------------------------------------------
