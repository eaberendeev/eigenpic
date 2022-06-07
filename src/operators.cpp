// Author: Evgeny Berendeev
// Budker Institute of Nuclear Physics of Siberian Branch Russian Academy of Sciences
// beren@inp.nsk.su
// (c) 2022, for licensing details see the LICENSE file

#include "simulation.h"

using namespace std;

using Eigen::Vector3d;

typedef Eigen::Triplet<double> Trip;

//create an identity operator
void Simulation::stencil_identity() {
	vector<Trip> trips;
	trips.reserve(totalSize.prod()*3);

	//operators should overlap at borders
	//all cells + a few border cells
	for(int x = -2; x < cellsX+2; x++) {
		for(int y = -2; y < cellsY+2; y++) {
			for(int z = -2; z < cellsZ+2; z++) {
				//make it work in 1D, 2D as well
				if(x >= cellsX+ghostCells[0] || x < -ghostCells[0])
					continue;
				if(y >= cellsY+ghostCells[1] || y < -ghostCells[1])
					continue;
				if(z >= cellsZ+ghostCells[2] || z < -ghostCells[2])
					continue;

				//this is a vector operator
				for(int c = 0; c < 3; c++) {
					int cur = vindg(x, y, z, c);

					trips.push_back(Trip(cur, cur, 1.0));
				}
			}
		}
	}
	identity.setFromTriplets(trips.begin(), trips.end());
}
