/*
 * GraphFamily.h
 *
 *  Created on: 14/1/2015
 *      Author: Yunior Ramirez-Cruz
 */

#ifndef GRAPHFAMILY_H_
#define GRAPHFAMILY_H_

#include "Definitions.h"
#include "CombinationSetIterator.h"

class GraphFamily
{
	protected:
		unsigned int VertexCount, FamilySize;
		vector<vector<vector<unsigned int> > > DistanceMatrices;

		unsigned int BottonUpGlobalMin, TopDownGlobalMin;

		vector<pair<unsigned int, set<unsigned int> > > InvertedIndex;
		vector<pair<unsigned int, unsigned int> > DistCountIndex;

		// Constructors' auxiliary methods
		void FinishComputationDistanceMatrices();

		// Common SimDim service methods
		unsigned int distance(unsigned int, unsigned int, unsigned int);
		unsigned int distance2(unsigned int, unsigned int, unsigned int);
		void set_distance(unsigned int, unsigned int, unsigned int, unsigned int);
		bool MustDistinguish(dim_type, unsigned int, unsigned int, unsigned int);
		bool Distinguishes(dim_type, unsigned int, unsigned int, unsigned int, unsigned int);
		bool CheckGenerator(dim_type, set<unsigned int>&, unsigned int = 1);
		unsigned int SolutionResolvabilityThreshold(dim_type, vector<unsigned int>&, unsigned int, unsigned int = 1);

		// Specific SimDim methods' auxiliary functions
		// For exhaustive search methods
		unsigned int BottomUpDFSAux(dim_type, unsigned int, set<unsigned int>&);
		unsigned int TopDownDFSAux(dim_type, unsigned int, set<unsigned int>&);
		// For greedy methods
		static bool InvIndEntryComparerGreedyAggregation(pair<unsigned int, set<unsigned int> >&, pair<unsigned int, set<unsigned int> >&);
		static bool DistCntIndEntryComparerGreedyPruning(pair<unsigned int, unsigned int>&, pair<unsigned int, unsigned int>&);
		void InitializeInvertedIndex(dim_type, bool = true);
		void InitializeDistCountIndex(dim_type, bool = true);
		// For metaheuristic methods
		void PermVGenSetNewSolution(vector<unsigned int>&, unsigned int, vector<unsigned int>&, new_solution_type);
		virtual unsigned int LocalSearchIteration(dim_type, unsigned int, unsigned int, unsigned int = 1);
	public:
		// Constructors and destroyers
		GraphFamily(unsigned int, unsigned int, vector<vector<vector<bool> > >&);
		GraphFamily(string);
		virtual ~GraphFamily();
		// Access methods
		inline unsigned int GetVertexCount() {return VertexCount;}
		inline unsigned int GetFamilySize() {return FamilySize;}
		// Exhaustive search SimDim methods
		virtual unsigned int SimDimBottomUpBFS(dim_type, unsigned int = 1, unsigned int = 1);
		virtual unsigned int SimDimTopDownBFS(dim_type, unsigned int = 1);
		virtual unsigned int SimDimBottomUpDFS(dim_type, unsigned int = 1);
		virtual unsigned int SimDimTopDownDFS(dim_type, unsigned int = 1);
		// Greedy SimDim methods
		virtual unsigned int SimDimGreedyAggregationInvInd(dim_type, unsigned int = 1);
		virtual unsigned int SimDimGreedyPruningDistCntInd(dim_type, unsigned int = 1);
		// Metaheuristic SimDim methods
		virtual vector<unsigned int> SimDimRandomizedLocalSearch(dim_type, vector<unsigned int>&, unsigned int, unsigned int, unsigned int = 1);
};

#endif /* GRAPHFAMILY_H_ */
