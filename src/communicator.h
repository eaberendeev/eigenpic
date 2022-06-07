// Author: Evgeny Berendeev
// Budker Institute of Nuclear Physics of Siberian Branch Russian Academy of Sciences
// beren@inp.nsk.su
// (c) 2022, for licensing details see the LICENSE file

#pragma once

#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "util.h"

//MPI communication class
class Communicator {
	private:
		int numCPUs; // how many MPI-ranks (CPUs)
		Vector3i globalCellSize; //size of the global simulation (in cells)
		Vector3i ghostCells; //how many ghost cells in each direction

		int myRank; //our own rank

		int cellOffset; //our local offset in global cell space

		Vector3i simSize; //our local simulation size
		Vector3i totalSize; //local sim size + ghost_cells

		//helper indexing routine taking into account ghost cells
		inline int indg(int x, int y, int z, int c, int nx, int ny, int nz, int nc) {
			return(ind(x + ghostCells[0], y + ghostCells[1], z + ghostCells[2], c, nx, ny, nz, nc));
		}

	public:
		Communicator(int argc, char* argv[], const Vector3i& globalCellSize, const Vector3i& ghostCells, int numCPUs);
		~Communicator();

		Vector3i get_local_size() const {return(simSize);}

};

#endif
