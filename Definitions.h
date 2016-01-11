/*
 * Definitions.h
 *
 *  Created on: 14/1/2015
 *      Author: Yunior Ramirez-Cruz
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

enum dim_type {DT_METRIC, DT_ADJACENCY, DT_LOCAL_METRIC, DT_LOCAL_ADJACENCY, DT_STRONG_METRIC, DT_K_METRIC, DT_K_ADJACENCY, DT_K_LOCAL_METRIC, DT_K_LOCAL_ADJACENCY, DT_K_STRONG_METRIC};

enum new_solution_type {NS_LOCAL, NS_EXPLORATORY};

struct sda_plus_type {unsigned int SdA, fam_type;};

enum greedy_init_type {GIT_GREEDY_AGGR_INV_IND, GIT_GREEDY_PR_DIST_CNT_IND, GIT_GREEDY_PR_USEF_IND};

#endif /* DEFINITIONS_H_ */
