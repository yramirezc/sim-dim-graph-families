/*
 * CombinationSetIterator.h
 *
 *  Created on: 12/2/2015
 *      Author: Yunior Ramirez-Cruz
 */

#ifndef COMBINATIONSETITERATOR_H_
#define COMBINATIONSETITERATOR_H_

#include "Definitions.h"

// Will generate combinations of CombinationSize elements out of PoolSize
// The source is assumed to be the integer range [0, ... , PoolSize - 1]

class CombinationSetIterator
{
	protected:
		unsigned int PoolSize, CombinationSize;
		vector<unsigned int> CurrentCombination;
		bool LastGenerated;
		bool NextCombAux(unsigned int);
	public:
		CombinationSetIterator(unsigned int, unsigned int);
		virtual ~CombinationSetIterator();
		bool NextCombination(vector<unsigned int>&);
		void Reset();
};

#endif /* COMBINATIONSETITERATOR_H_ */
