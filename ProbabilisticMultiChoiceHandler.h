/*
 * ProbabilisticMultiChoiceHandler.h
 *
 *  Created on: 24/2/2015
 *      Author: Yunior Ramirez-Cruz
 */

#ifndef PROBABILISTICMULTICHOICEHANDLER_H_
#define PROBABILISTICMULTICHOICEHANDLER_H_

#include "Definitions.h"

class ProbabilisticMultiChoiceHandler
{
	protected:
	public:
		ProbabilisticMultiChoiceHandler(bool = false);
		virtual ~ProbabilisticMultiChoiceHandler();
		virtual unsigned int GetChoice(vector<double>&, set<unsigned int>&);
};

#endif /* PROBABILISTICMULTICHOICEHANDLER_H_ */
