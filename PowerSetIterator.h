/*
 * PowerSetIterator.h
 *
 *  Created on: 12/2/2015
 *      Author: Yunior Ramirez-Cruz
 */

#ifndef POWERSETITERATOR_H_
#define POWERSETITERATOR_H_

#include "Definitions.h"

class PowerSetIterator
{
	protected:
		unsigned int PoolSize;
		vector<bool> CurrentSetDescriptor;
		bool LastGenerated;
		bool NextDescriptor();
	public:
		PowerSetIterator(unsigned int);
		virtual ~PowerSetIterator();
		bool NextSet(set<unsigned int>&);
		void Reset();
};

#endif /* POWERSETITERATOR_H_ */
