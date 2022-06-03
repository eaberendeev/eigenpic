// Author: Andreas Kempf, Ruhr-Universitaet Bochum, ank@tp4.rub.de
// (c) 2014, for licensing details see the LICENSE file

#include <algorithm>
#include <iostream>
#include <utility>

#include "communicator.h"

using namespace std;

using Eigen::Vector3d;
using Eigen::Vector3i;

//Constructor sets up MPI communication
Communicator::Communicator(int argc, char* argv[], 
	  const Vector3i& globalCellSize, const Vector3i& ghostCells, int numCPUs) :
	numCPUs(numCPUs),
	globalCellSize(globalCellSize),
	ghostCells(ghostCells)
{
	for(int dim = 0; dim < 3; dim++) {
		simSize[dim] = globalCellSize[dim];
		std::cout << simSize[dim] << " size \n";
	}
	int rank = 0;
	split_by_threads(globalCellSize[0],rank,numCPUs,simSize[0],cellOffset);
	totalSize = simSize+2*ghostCells;
}

//clean up
Communicator::~Communicator() {
	//MPI_Finalize();
}
