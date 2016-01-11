/*
 * GraphFamily.cpp
 *
 *  Created on: 14/1/2015
 *      Author: Yunior Ramirez-Cruz
 */

#include "GraphFamily.h"

//-------------------------------------------------------------------------------------------

GraphFamily::GraphFamily(unsigned int VertCount, unsigned int FamSize, vector<vector<vector<bool> > > &AdjMatrices)
{
	VertexCount = VertCount;
	FamilySize = FamSize;
	BottonUpGlobalMin = TopDownGlobalMin = VertexCount + 1;
	// Initialize distance matrices with values of the adjacency matrices
	for (unsigned int i = 0; i < FamilySize; i++)
	{
		vector<vector<unsigned int> > DistMatrix;
		for (unsigned int u = 0; u < VertexCount - 1; u++)
		{
			vector<unsigned int> DMRow;
			for (unsigned int v = 0; v < AdjMatrices.at(i).at(u).size(); v++)
				if (AdjMatrices.at(i).at(u).at(v))
					DMRow.push_back(1);
				else
					DMRow.push_back(VertexCount);   // This is the maximum possible distance plus 1
			DistMatrix.push_back(DMRow);
		}
		DistanceMatrices.push_back(DistMatrix);
	}
	FinishComputationDistanceMatrices();
}

//-------------------------------------------------------------------------------------------

GraphFamily::GraphFamily(string path)
{
	// It will be assumed that the file has the correct format
	ifstream file_reader;
	file_reader.open(path.c_str(), std::ifstream::in);
	file_reader >> FamilySize >> VertexCount;
	BottonUpGlobalMin = TopDownGlobalMin = VertexCount + 1;
	file_reader.get();   // Skip the new line character still remaining to get to the lines specifying the adjacency matrices
	// Initialize distance matrices with values of the adjacency matrices read from file
	for (unsigned int i = 0; i < FamilySize; i++)
	{
		vector<vector<unsigned int> > DistMatrix;
		for (unsigned int u = 0; u < VertexCount - 1; u++)
		{
			vector<unsigned int> DMRow;
			string ln;
			file_reader >> ln;
			for (unsigned int v = 0; v < VertexCount - u - 1; v++)
				if (ln[v] == '1')
					DMRow.push_back(1);
				else
					DMRow.push_back(VertexCount);   // This is the maximum possible distance plus 1
			DistMatrix.push_back(DMRow);
		}
		DistanceMatrices.push_back(DistMatrix);
	}
	file_reader.close();
	FinishComputationDistanceMatrices();
}

//-------------------------------------------------------------------------------------------

void GraphFamily::FinishComputationDistanceMatrices()
{
	// Distance matrices are initialized with the adjacency matrices
	// Compute non-1 distances
	bool ChangesOccurred;
	for (unsigned int i = 0; i < DistanceMatrices.size(); i++)
	{
		do
		{
			ChangesOccurred = false;
			for (unsigned int u = 0; u < VertexCount - 1; u++)
				for (unsigned int v = u + 1; v < VertexCount; v++)
					for (unsigned int w = 0; w < VertexCount; w++)
						if (w != u && w != v)
						{
							unsigned int dist_uw = distance(i, u, w);
							unsigned int dist_vw = distance(i, v, w);
							unsigned int dist_uv = distance(i, u, v);
							if (dist_uv > dist_uw + dist_vw)
							{
								set_distance(i, u, v, dist_uw + dist_vw);
								ChangesOccurred = true;
							}
						}
		}
		while (ChangesOccurred);
	}
}

//-------------------------------------------------------------------------------------------

GraphFamily::~GraphFamily()
{
	// Nothing to destroy
}

//-------------------------------------------------------------------------------------------

unsigned int GraphFamily::distance(unsigned int GraphOrder, unsigned int u, unsigned int v)
{
	if (u < v)
		return DistanceMatrices.at(GraphOrder).at(u).at(v - u - 1);
	else if (u > v)
		return DistanceMatrices.at(GraphOrder).at(v).at(u - v - 1);
	else
		return 0;
}

//-------------------------------------------------------------------------------------------

unsigned int GraphFamily::distance2(unsigned int GraphOrder, unsigned int u, unsigned int v)
{
	unsigned int dist = distance(GraphOrder, u, v);
	if (dist <= 1)
		return dist;
	else
		return 2;
}

//-------------------------------------------------------------------------------------------

void GraphFamily::set_distance(unsigned int GraphOrder, unsigned int u, unsigned int v, unsigned int dist)
{
	if (u < v)
		DistanceMatrices.at(GraphOrder).at(u).at(v - u - 1) = dist;
	else if (u > v)
		DistanceMatrices.at(GraphOrder).at(v).at(u - v - 1) = dist;
}

//-------------------------------------------------------------------------------------------

bool GraphFamily::MustDistinguish(dim_type DimensionType, unsigned int GraphOrder, unsigned int u, unsigned int v)
{
	switch (DimensionType)
	{
	case DT_METRIC:
	case DT_ADJACENCY:
	case DT_K_METRIC:
	case DT_K_ADJACENCY:
	case DT_STRONG_METRIC:
	case DT_K_STRONG_METRIC:
		return bool(u != v);
	case DT_LOCAL_METRIC:
	case DT_LOCAL_ADJACENCY:
	case DT_K_LOCAL_METRIC:
	case DT_K_LOCAL_ADJACENCY:
		return bool(distance(GraphOrder, u, v) == 1);
	default:;
	}
	return false;
}

//-------------------------------------------------------------------------------------------

bool GraphFamily::Distinguishes(dim_type DimensionType, unsigned int GraphOrder, unsigned int x, unsigned int u, unsigned int v)   // Does x distinguish u and v on GraphOrder-th graph as needed by DimensionType?
{
	switch (DimensionType)
	{
	case DT_METRIC:
	case DT_LOCAL_METRIC:
	case DT_K_METRIC:
	case DT_K_LOCAL_METRIC:
		return bool(distance(GraphOrder, x, u) != distance(GraphOrder, x, v));
	case DT_ADJACENCY:
	case DT_LOCAL_ADJACENCY:
	case DT_K_ADJACENCY:
	case DT_K_LOCAL_ADJACENCY:
		return bool(distance2(GraphOrder, x, u) != distance2(GraphOrder, x, v));
	case DT_STRONG_METRIC:
	case DT_K_STRONG_METRIC:
		return bool((distance(GraphOrder, x, u) == distance(GraphOrder, x, v) + distance(GraphOrder, u, v)) || (distance(GraphOrder, x, v) == distance(GraphOrder, x, u) + distance(GraphOrder, u, v)));
	default:;
	}
	return false;
}

//-------------------------------------------------------------------------------------------

bool GraphFamily::CheckGenerator(dim_type DimensionType, set<unsigned int> &PotentialGenerator, unsigned int k)
{
	switch (DimensionType)
	{
	case DT_METRIC:
	case DT_LOCAL_METRIC:
	case DT_ADJACENCY:
	case DT_LOCAL_ADJACENCY:
	case DT_STRONG_METRIC:
		for (unsigned int i = 0; i < FamilySize; i++)
			for (unsigned int u = 0; u < VertexCount - 1; u++)
				for (unsigned int v = u + 1; v < VertexCount; v++)
					if (MustDistinguish(DimensionType, i, u, v) && PotentialGenerator.find(u) == PotentialGenerator.end() && PotentialGenerator.find(v) == PotentialGenerator.end())
					{
						bool FoundDistinguisher = false;
						for (set<unsigned int>::iterator it = PotentialGenerator.begin(); !FoundDistinguisher && it != PotentialGenerator.end(); it++)
							if (Distinguishes(DimensionType, i, *it, u, v))
								FoundDistinguisher = true;
						if (!FoundDistinguisher)
							return false;
					}
		return true;
	case DT_K_METRIC:
	case DT_K_LOCAL_METRIC:
	case DT_K_ADJACENCY:
	case DT_K_LOCAL_ADJACENCY:
	case DT_K_STRONG_METRIC:
		if (k == 2)   // For k == 2 not all pairs need to be checked, as well as for k == 1 (cases above)
		{
			for (unsigned int i = 0; i < FamilySize; i++)
				for (unsigned int u = 0; u < VertexCount - 1; u++)
					for (unsigned int v = u + 1; v < VertexCount; v++)
						if (MustDistinguish(DimensionType, i, u, v) && (PotentialGenerator.find(u) == PotentialGenerator.end() || PotentialGenerator.find(v) == PotentialGenerator.end()))
						{
							unsigned int DistinguisherCount = 0;
							for (set<unsigned int>::iterator it = PotentialGenerator.begin(); DistinguisherCount < 2 && it != PotentialGenerator.end(); it++)
								if (Distinguishes(DimensionType, i, *it, u, v))
									DistinguisherCount++;
							if (DistinguisherCount < 2)
								return false;
						}
			return true;
		}
		else
		{
			for (unsigned int i = 0; i < FamilySize; i++)
				for (unsigned int u = 0; u < VertexCount - 1; u++)
					for (unsigned int v = u + 1; v < VertexCount; v++)
						if (MustDistinguish(DimensionType, i, u, v))
						{
							unsigned int DistinguisherCount = 0;
							for (set<unsigned int>::iterator it = PotentialGenerator.begin(); DistinguisherCount < k && it != PotentialGenerator.end(); it++)
								if (Distinguishes(DimensionType, i, *it, u, v))
									DistinguisherCount++;
							if (DistinguisherCount < k)
								return false;
						}
			return true;
		}
		break;
	default:;
	}
	return false;
}

//-------------------------------------------------------------------------------------------

unsigned int GraphFamily::SimDimBottomUpBFS(dim_type DimensionType, unsigned int StartAt, unsigned int k)
{
	for (unsigned int gen_size = StartAt; gen_size < VertexCount; gen_size++)
	{
		vector<unsigned int> gen;
		CombinationSetIterator CombIter(VertexCount, gen_size);
		CombIter.NextCombination(gen);
		do
		{
			set<unsigned int> CandidateGenerator(gen.begin(), gen.end());
			if (CheckGenerator(DimensionType, CandidateGenerator, k))
				return gen_size;
		}
		while (CombIter.NextCombination(gen));
	}
	return VertexCount;
}

//-------------------------------------------------------------------------------------------

unsigned int GraphFamily::SimDimTopDownBFS(dim_type DimensionType, unsigned int k)
{
	// Initialization
	set<unsigned int> WholeVertexSet;
	for (unsigned int i = 0; i < VertexCount; i++)
		WholeVertexSet.insert(i);
	vector<set<unsigned int> > Complements;
	for (unsigned int i = 0; i < VertexCount; i++)
	{
		set<unsigned int> complement;
		complement.insert(i);
		Complements.push_back(complement);
	}
	// Search
	unsigned int MinGensSize = VertexCount - 1;
	while (Complements.size())
	{
		vector<set<unsigned int> > NextIterationComplements;
		for (unsigned int i = 0; i < Complements.size() - 1; i++)
			for (unsigned int j = i + 1; j < Complements.size(); j++)
			{
				// Check if i-th and j-th complement may be merged at this step
				set<unsigned int> compl_union;
				compl_union = Complements.at(i);
				for (set<unsigned int>::iterator it = Complements.at(j).begin(); it != Complements.at(j).end(); it++)
					if (compl_union.find(*it) == compl_union.end())
						compl_union.insert(*it);
				if (compl_union.size() == VertexCount - MinGensSize + 1)
				{
					set<unsigned int> CandidateGenerator;
					for (unsigned int i = 0; i < VertexCount; i++)
						if (compl_union.find(i) == compl_union.end())
							CandidateGenerator.insert(i);
					if (CheckGenerator(DimensionType, CandidateGenerator, k))
						NextIterationComplements.push_back(compl_union);
				}
			}
		if (NextIterationComplements.size())
			MinGensSize--;
		Complements = NextIterationComplements;
	}
	return MinGensSize;
}

//-------------------------------------------------------------------------------------------

unsigned int GraphFamily::SimDimBottomUpDFS(dim_type DimensionType, unsigned int k)
{
	BottonUpGlobalMin = VertexCount;
	unsigned int min_dim = BottonUpGlobalMin + 1;
	for (unsigned int i = 0; i < VertexCount; i++)
	{
		set<unsigned int> InitCandidate;
		InitCandidate.insert(i);
		unsigned int aux_dim = BottomUpDFSAux(DimensionType, k, InitCandidate);
		if (aux_dim < min_dim)
			min_dim = aux_dim;
	}
	return min_dim;
}

//-------------------------------------------------------------------------------------------

unsigned int GraphFamily::BottomUpDFSAux(dim_type DimensionType, unsigned int k, set<unsigned int> &CurrentCandidate)
{
	if (CurrentCandidate.size() < BottonUpGlobalMin)
		if (CheckGenerator(DimensionType, CurrentCandidate, k))
		{
			BottonUpGlobalMin = CurrentCandidate.size();
			return CurrentCandidate.size();
		}
		else
			if (CurrentCandidate.size() + 1 < BottonUpGlobalMin)
			{
				unsigned int min_dim = VertexCount + 1;
				for (unsigned int i = 0; i < VertexCount; i++)
					if (CurrentCandidate.find(i) == CurrentCandidate.end())
					{
						set<unsigned int> NewCandidate = CurrentCandidate;
						NewCandidate.insert(i);
						unsigned int aux_dim = BottomUpDFSAux(DimensionType, k, NewCandidate);
						if (aux_dim < min_dim)
							min_dim = aux_dim;
					}
				return min_dim;
			}
			else
				return BottonUpGlobalMin;
	else
		return BottonUpGlobalMin;
}

//-------------------------------------------------------------------------------------------

unsigned int GraphFamily::SimDimTopDownDFS(dim_type DimensionType, unsigned int k)
{
	TopDownGlobalMin = VertexCount + 1;
	unsigned int min_dim = VertexCount + 1;
	for (unsigned int i = 0; i < VertexCount; i++)
	{
		set<unsigned int> InitCandidate;
		for (unsigned int j = 0; j < i; j++)
			InitCandidate.insert(j);
		for (unsigned int j = i + 1; j < VertexCount; j++)
			InitCandidate.insert(j);
		unsigned int aux_dim = TopDownDFSAux(DimensionType, k, InitCandidate);
		if (aux_dim < min_dim)
			min_dim = aux_dim;
	}
	return min_dim;
}

//-------------------------------------------------------------------------------------------

unsigned int GraphFamily::TopDownDFSAux(dim_type DimensionType, unsigned int k, set<unsigned int> &CurrentCandidate)
{
	if (CurrentCandidate.size() < TopDownGlobalMin)
		if (CheckGenerator(DimensionType, CurrentCandidate, k))
		{
			unsigned int min_dim = VertexCount + 1;
			for (set<unsigned int>::iterator it = CurrentCandidate.begin(); it != CurrentCandidate.end(); it++)
			{
				set<unsigned int> NewCandidate = CurrentCandidate;
				NewCandidate.erase(*it);
				unsigned int aux_dim = TopDownDFSAux(DimensionType, k, NewCandidate);
				if (aux_dim < min_dim)
					min_dim = aux_dim;
			}
			if (min_dim < TopDownGlobalMin)
				TopDownGlobalMin = min_dim;
			return min_dim;
		}
		else
		{
			if (CurrentCandidate.size() + 1 < TopDownGlobalMin)
				TopDownGlobalMin = CurrentCandidate.size() + 1;
			return CurrentCandidate.size() + 1;
		}
	else
	{
		unsigned int min_dim = VertexCount + 1;
		for (set<unsigned int>::iterator it = CurrentCandidate.begin(); it != CurrentCandidate.end(); it++)
		{
			set<unsigned int> NewCandidate = CurrentCandidate;
			NewCandidate.erase(*it);
			unsigned int aux_dim = TopDownDFSAux(DimensionType, k, NewCandidate);
			if (aux_dim < min_dim)
				min_dim = aux_dim;
		}
		if (min_dim < TopDownGlobalMin)
			TopDownGlobalMin = min_dim;
		return min_dim;
	}
}

//-------------------------------------------------------------------------------------------

void GraphFamily::InitializeInvertedIndex(dim_type CalculationToPerform, bool CountItSelf)
{
	InvertedIndex.clear();
	for (unsigned int x = 0; x < VertexCount; x++)
	{
		set<unsigned int> DistinguishedPairs;
		for (unsigned int u = 0; u < VertexCount - 1; u++)
			for (unsigned int v = u + 1; v < VertexCount; v++)
				if (x != u && x != v)
				{
					for (unsigned int i = 0; i < FamilySize; i++)
						if (MustDistinguish(CalculationToPerform, i, u, v) && Distinguishes(CalculationToPerform, i, x, u, v))
							DistinguishedPairs.insert(VertexCount * VertexCount * i + VertexCount * u + v);
				}
				else if (CountItSelf)
					for (unsigned int i = 0; i < FamilySize; i++)
						if (MustDistinguish(CalculationToPerform, i, u, v))
							DistinguishedPairs.insert(VertexCount * VertexCount * i + VertexCount * u + v);
		InvertedIndex.push_back(pair<unsigned int, set<unsigned int> >(x, DistinguishedPairs));
	}
}

//-------------------------------------------------------------------------------------------

void GraphFamily::InitializeDistCountIndex(dim_type CalculationToPerform, bool CountItSelf)
{
	DistCountIndex.clear();
	for (unsigned int x = 0; x < VertexCount; x++)
	{
		unsigned int DistinguishedPairsCnt = 0;
		for (unsigned int u = 0; u < VertexCount - 1; u++)
			for (unsigned int v = u + 1; v < VertexCount; v++)
				if (x != u && x != v)
				{
					for (unsigned int i = 0; i < FamilySize; i++)
						if (MustDistinguish(CalculationToPerform, i, u, v) && Distinguishes(CalculationToPerform, i, x, u, v))
							DistinguishedPairsCnt++;
				}
				else if (CountItSelf)
					for (unsigned int i = 0; i < FamilySize; i++)
						if (MustDistinguish(CalculationToPerform, i, u, v))
							DistinguishedPairsCnt++;
		DistCountIndex.push_back(pair<unsigned int, unsigned int>(x, DistinguishedPairsCnt));
	}
}

//-------------------------------------------------------------------------------------------

bool GraphFamily::DistCntIndEntryComparerGreedyPruning(pair<unsigned int, unsigned int> &entry1, pair<unsigned int, unsigned int> &entry2)
{
	return (entry1.second < entry2.second);
}

//-------------------------------------------------------------------------------------------

bool GraphFamily::InvIndEntryComparerGreedyAggregation(pair<unsigned int, set<unsigned int> > &entry1, pair<unsigned int, set<unsigned int> > &entry2)
{
	return (entry1.second.size() > entry2.second.size());
}

//-------------------------------------------------------------------------------------------

unsigned int GraphFamily::SimDimGreedyPruningDistCntInd(dim_type CalculationToPerform, unsigned int k)
{
	// Initialization
	InitializeDistCountIndex(CalculationToPerform);
	sort(DistCountIndex.begin(), DistCountIndex.end(), DistCntIndEntryComparerGreedyPruning);
	set<unsigned int> CandidateGenerator;
	for (unsigned int i = 0; i < VertexCount; i++)   // Initially the whole vertex set
		CandidateGenerator.insert(i);
	// Search
	unsigned int i = 0;
	do
	{
		CandidateGenerator.erase(DistCountIndex.at(i).first);
		i++;
	}
	while (CheckGenerator(CalculationToPerform, CandidateGenerator, k));
	return CandidateGenerator.size() + 1;
}

//-------------------------------------------------------------------------------------------

unsigned int GraphFamily::SimDimGreedyAggregationInvInd(dim_type CalculationToPerform, unsigned int k)
{
	// Initialization
	InitializeInvertedIndex(CalculationToPerform);
	sort(InvertedIndex.begin(), InvertedIndex.end(), InvIndEntryComparerGreedyAggregation);
	set<unsigned int> CandidateGenerator;   // Initially empty
	// Search
	unsigned int i = 0;
	bool GeneratorFound = false;
	do
	{
		CandidateGenerator.insert(InvertedIndex.at(i).first);
		if (CheckGenerator(CalculationToPerform, CandidateGenerator, k))
			GeneratorFound = true;
		else
		{
			for (unsigned int j = i + 1; j < VertexCount; j++)
			{
				set<unsigned int> difference;
				for (set<unsigned int>::iterator it = InvertedIndex.at(j).second.begin(); it != InvertedIndex.at(j).second.end(); it++)
					if (InvertedIndex.at(i).second.find(*it) == InvertedIndex.at(i).second.end())
						difference.insert(*it);
				InvertedIndex.at(j).second = difference;
			}
			i++;
			sort(InvertedIndex.begin() + i, InvertedIndex.end(), InvIndEntryComparerGreedyAggregation);
		}
	}
	while (!GeneratorFound);
	return CandidateGenerator.size();
}

//-------------------------------------------------------------------------------------------

// Calling LocalSearchIteration once runs the standard local search

vector<unsigned int> GraphFamily::SimDimRandomizedLocalSearch(dim_type DimensionType, vector<unsigned int> &LocalSearchesCounts, unsigned int MaxIters_x_LocalSearch, unsigned int CandSolCount, unsigned int k)
{
	// Values in LocalSearchesCounts are assumed to be ordered incrementally
	vector<unsigned int> Solutions;
	srand(time(NULL));
	unsigned int CurrentCount = 0;
	unsigned int BestSolution = LocalSearchIteration(DimensionType, MaxIters_x_LocalSearch, CandSolCount, k);
	if (LocalSearchesCounts.at(LocalSearchesCounts.size() - 1) == 1)
		Solutions.push_back(BestSolution);
	for (unsigned int i = 1; i < LocalSearchesCounts.at(LocalSearchesCounts.size() - 1); i++)
	{
		unsigned int NewSolution = LocalSearchIteration(DimensionType, MaxIters_x_LocalSearch, CandSolCount, k);
		if (NewSolution < BestSolution)
			BestSolution = NewSolution;
		if (i == LocalSearchesCounts.at(CurrentCount) - 1)
		{
			Solutions.push_back(BestSolution);
			CurrentCount++;
		}
	}
	return Solutions;
}

//-------------------------------------------------------------------------------------------

unsigned int GraphFamily::LocalSearchIteration(dim_type DimensionType, unsigned int MaxIters, unsigned int CandSolCount, unsigned int k)
{
	vector<unsigned int> CurrentSolution;
	for (unsigned int i = 0; i < VertexCount; i++)
		CurrentSolution.push_back(VertexCount - i - 1);
	random_shuffle(CurrentSolution.begin(), CurrentSolution.end());
	unsigned int CurrentSolutionResolvabilityThreshold = SolutionResolvabilityThreshold(DimensionType, CurrentSolution, k, k);
	bool SolutionImproved = true;
	for (unsigned int iter = 0; SolutionImproved && iter < MaxIters; iter++)
	{
		SolutionImproved = false;
		for (unsigned int cand = 0; cand < CandSolCount; cand++)
		{
			vector<unsigned int> NewCandidateSolution;
			PermVGenSetNewSolution(CurrentSolution, CurrentSolutionResolvabilityThreshold, NewCandidateSolution, NS_LOCAL);
			unsigned int NewCandidateSolutionResolvabilityThreshold = SolutionResolvabilityThreshold(DimensionType, NewCandidateSolution, k, k);
			if (NewCandidateSolutionResolvabilityThreshold < CurrentSolutionResolvabilityThreshold)
			{
				CurrentSolutionResolvabilityThreshold = NewCandidateSolutionResolvabilityThreshold;
				CurrentSolution = NewCandidateSolution;
				SolutionImproved = true;
			}
		}
	}
	return CurrentSolutionResolvabilityThreshold;
}

//-------------------------------------------------------------------------------------------

unsigned int GraphFamily::SolutionResolvabilityThreshold(dim_type DimensionType, vector<unsigned int> &Perm, unsigned int StartAt, unsigned int k)
{
	// Possibly inefficient way to check
	for (unsigned int i = StartAt; i < VertexCount - 1; i++)
	{
		set<unsigned int> CandidateGenerator(Perm.begin(), Perm.begin() + i);
		if (CheckGenerator(DimensionType, CandidateGenerator, k))
			return i;
	}
	return VertexCount;
}

//-------------------------------------------------------------------------------------------

// Obtain a new candidate solution.
// Local variant:
// A randomly chosen vertex in the interval 0,..., ResolvabilityThreshold is switched with another randomly chosen vertex
// Exploratory variant: perform a number of local changes ranging from one third to two thirds of VertexCount

void GraphFamily::PermVGenSetNewSolution(vector<unsigned int> &CurrentSolution, unsigned int ResolvabilityThresholdCurrentSolution, vector<unsigned int> &NewCandidateSolution, new_solution_type solution_type)
{
    if (solution_type == NS_LOCAL)
    {
    	NewCandidateSolution = CurrentSolution;
    	unsigned int first_item = rand() % ResolvabilityThresholdCurrentSolution, second_item = rand() % VertexCount;
    	while (first_item == second_item)
    	    second_item = rand() % VertexCount;
    	unsigned int tmp = NewCandidateSolution.at(first_item);
    	NewCandidateSolution.at(first_item) = NewCandidateSolution.at(second_item);
    	NewCandidateSolution.at(second_item) = tmp;
    }
    else   // NS_EXPLORATORY
    {
        unsigned int removal_number = VertexCount / 3 + rand() % (VertexCount / 3);  // Make a number of local changes ranging from one third to two thirds of VertexCount
        NewCandidateSolution = CurrentSolution;
        for (unsigned int i = 0; i < removal_number; i++)
        {
        	PermVGenSetNewSolution(CurrentSolution, ResolvabilityThresholdCurrentSolution, NewCandidateSolution, NS_LOCAL);
        	CurrentSolution = NewCandidateSolution;
        }
    }
}

//-------------------------------------------------------------------------------------------
