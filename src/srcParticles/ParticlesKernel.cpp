// Author: Evgeny Berendeev
// Budker Institute of Nuclear Physics of Siberian Branch Russian Academy of Sciences
// beren@inp.nsk.su
// (c) 2022, for licensing details see the LICENSE file

#include "particles.h"
#include "constants.h"
#include "simulation.h"

void Simulation::push(Vector3d& coord, Vector3d& puls, long q, double mass, double mpw, \
		   const Field& fieldE, const Field& fieldB, Field& currentDensity, double _dx, double _dt){
	// number of cell where is particles
	long xg, yg, zg;
	long n, m, k,indx, indy,indz;
	double xc, yc, zc;
	double gama;
	Vector3d US,U1,U2,T,C;
	double xn, yn,zn;
	double a,b;
	alignas(64) double sx[SMAX], sy[SMAX], sz[SMAX];
	alignas(64) double sx05[SMAX], sy05[SMAX], sz05[SMAX];
	alignas(64) double sx_n[SMAX], sy_n[SMAX], sz_n[SMAX];
	alignas(64) double jx[SMAX][SMAX][SMAX];
	alignas(64) double jy[SMAX][SMAX][SMAX];
	alignas(64) double jz[SMAX][SMAX][SMAX];
	alignas(64) Vector3d E_loc = {0., 0., 0.};
	alignas(64) Vector3d B_loc = {0., 0., 0.};

	const double dtp = 0.5 * _dt;
	const double conx = _dx / (6*_dt) * mpw;
	const double cony = _dx / (6*_dt) * mpw;
	const double conz = _dx / (6*_dt) * mpw;
	const double dx3 = _dx*_dx*_dx;

	
	xc = coord[0] / _dx;
	yc = coord[1] / _dx;
	zc = coord[2] / _dx;

	xg = long(xc);
	yg = long(yc);
	zg = long(zc);
	
	for(n = 0; n < SMAX; ++n){
		sx[n] = weight(xc, xg + n - CELLS_SHIFT) / _dx;
		sx05[n] = weight(xc, xg + n - CELLS_SHIFT, 0.5) / _dx;
		sy[n] = weight(yc, yg + n - CELLS_SHIFT) / _dx;
		sy05[n] = weight(yc, yg + n - CELLS_SHIFT, 0.5) / _dx;
		sz[n] = weight(xc, xg + n - CELLS_SHIFT) / _dx;
		sz05[n] = weight(zc, zg + n - CELLS_SHIFT, 0.5) / _dx;
	}
		
	for(n = 0; n < SMAX; ++n){
			indx = xg + n;

		for(m = 0; m < SMAX; ++m){
			indy = yg  + m;
			for(k = 0; k < SMAX; ++k){

				jx[n][m][k] = 0.;
				jy[n][m][k] = 0.;
				jz[n][m][k] = 0.;
				
				indz = zg  + k;
				E_loc[0] += dx3 * sx05[n] * sy[m] * sz[k] * fieldE[vind(indx,indy,indz,0)] ;
				E_loc[1] += dx3 * sx[n] * sy05[m] * sz[k] * fieldE[vind(indx,indy,indz,1)] ;
				E_loc[2] += dx3 * sx[n] * sy[m] * sz05[k] * fieldE[vind(indx,indy,indz,2)] ;
				B_loc[0] += dx3 * sx[n] * sy05[m] * sz05[k] * fieldB[vind(indx,indy,indz,0)] ;
				B_loc[1] += dx3 * sx05[n] * sy[m] * sz05[k] * fieldB[vind(indx,indy,indz,1)] ;
				B_loc[2] += dx3 * sx05[n] * sy05[m] * sz[k] * fieldB[vind(indx,indy,indz,2)] ;
			}
		}
	}

	U1 = puls + q * dtp * E_loc;
	a = q * dtp / sqrt(1. + U1.dot(U1) );
	T = a * B_loc;
	b = 2. / (1. + T.dot(T) );
	C = b * T;
		
	US = U1 + U1.cross(T);
		
	U2 = U1 + US.cross(C);
		
	puls = U2 + q * dtp * E_loc;
		
	gama = 1. / sqrt(mass * mass + puls.dot(puls) );
			
	xn = coord[0] + _dt * puls[0] * gama;
	yn = coord[1] + _dt * puls[1] * gama;
	zn = coord[2] + _dt * puls[2] * gama;
	coord = Vector3d(xn,yn,zn);

	for(n = 0; n < SMAX; ++n){
		sx_n[n] = weight(xn / _dx, xg + n - CELLS_SHIFT) / _dx;
		sy_n[n] = weight(yn / _dx, yg + n - CELLS_SHIFT) / _dx;
		sz_n[n] = weight(zn / _dx, zg + n - CELLS_SHIFT) / _dx;
	}

	for(n = 0; n < SMAX; ++n){
		indx = xg  + n;
		for(m = 0; m < SMAX; ++m){
			indy = yg + m;
			for(k = 0; k < SMAX; ++k){
		  
				if(n == 0) jx[n][m][k] = -q * conx * (sx_n[n] - sx[n]) *  (sy_n[m] * (2*sz_n[k] + sz[k]) + sy[m] * (2 * sz[k] + sz_n[k]));

				if(n > 0 && n < SMAX-1) jx[n][m][k] = jx[n-1][m][k] - q * conx * (sx_n[n] - sx[n]) *  (sy_n[m] * (2*sz_n[k] + sz[k]) + sy[m] * (2 * sz[k] + sz_n[k]));
				
				if(m == 0) jy[n][m][k] = -q * cony * (sy_n[m] - sy[m]) *(sx_n[n] * (2*sz_n[k] + sz[k]) + sx[n] * (2 * sz[k] + sz_n[k]));
				if(m > 0 && m < SMAX-1) jy[n][m][k] = jy[n][m-1][k] -q * cony * (sy_n[m] - sy[m]) *(sx_n[n] * (2*sz_n[k] + sz[k]) + sx[n] * (2 * sz[k] + sz_n[k]));
				
				if(k == 0) jz[n][m][k] = -q * conz * (sz_n[k] - sz[k]) * (sy_n[m] * (2*sx_n[n] + sx[n]) + sy[m] * (2 * sx[n] + sx_n[n]));
				if(k > 0 && k < SMAX-1) jz[n][m][k] = jz[n][m][k-1]-q * conz  * (sz_n[k] - sz[k]) * (sy_n[m] * (2*sx_n[n] + sx[n]) + sy[m] * (2 * sx[n] + sx_n[n]));
			
				indz = zg + k;
			
				currentDensity[vind(indx,indy,indz,0)] += jx[n][m][k];
				currentDensity[vind(indx,indy,indz,1)] += jy[n][m][k];
				currentDensity[vind(indx,indy,indz,2)] += jz[n][m][k];
			}
		}
	}
		
}

// void ParticlesArray::move(Mesh& mesh,long timestep){
// 	Vector3d r;
// 	Vector3d p;
// 	Vector3d r_glob;
// 	Particle particle;
// 	bool in_area;

// 	if (charge == 0) return;
	
// 	long k = 0;
// 	long kmax = size();

// 	while (k < kmax ) {
// 		r = particlesData(k).coord;
// 		p = particlesData(k).pulse;

		
// 			push(r,p, charge, mass(k), mpw(k), mesh.fieldE, mesh.fieldB, mesh.fieldJ, ELas);

		
// 		particlesData(k).coord = r;
// 		particlesData(k).pulse = p;
// 		k++;
// 	}
	
// 	k = 0;
// 	while (k < size()) {

// 		r = particlesData(k).coord;
		
// 		lostXLeft = (r.x() < Dx * _world.region.dampCells[0].x() );
		 
// 		lostXRight = (r.x() >= Dx*(_world.region.numCells.x() - _world.region.dampCells[1].x() ));
		
// 		lostY = (r.y() <= 2*Dy || r.y() >= Dy*_world.region.numCells.y() - 2*Dy);
// 		lostZ = (r.z() <= 2*Dz || r.z() >= Dz*_world.region.numCells.z() - 2*Dz);
		
// 		in_area = ! (lostXLeft || lostXRight || lostY || lostZ);
		
// 		if( in_area )  
// 		  ++k;
// 		else{
// 			particlesData.del(k);
// 		}
// 	}


// }

