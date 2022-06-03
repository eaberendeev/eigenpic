// Author: Evgeny Berendeev
// Budker Institute of Nuclear Physics of Siberian Branch Russian Academy of Sciences
// beren@inp.nsk.su
// (c) 2022, for licensing details see the LICENSE file

#pragma once

#ifndef UTIL_H
#define UTIL_H
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <map>
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <algorithm>

// Util for read parameters
void split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
void read_params_to_string(const std::string& target,const std::string& filename,
	std::vector< std::vector<std::string> > &params);

// Define basic types
typedef Eigen::SparseMatrix<double, Eigen::ColMajor> Operator;
typedef Eigen::VectorXd Field;
typedef Eigen::Vector2i Vector2i;
typedef Eigen::Vector3i Vector3i;
typedef Eigen::Vector2d Vector2d;
typedef Eigen::Vector3d Vector3d;

//general indexing routine (row major)
inline constexpr int ind(int x, int y, int z, int c, int Nx, int Ny, int Nz, int Nc)
{
	return(c + Nc*(z + Nz*(y + Ny*x)));
}


//calculate weight for particle position and grid point, Triangular Shaped Cloud (second order)
inline double weight(double partpos, int index, double grid_shift = 0.)
{
	double distance = fabs(partpos - index - grid_shift);

	if (distance <= 0.5)
		return 0.75 - (distance*distance);
	distance -= 1.5;
	if (distance <= 0.)
		return 0.5 * distance*distance;
	return 0.;
}

inline int int_value(double dblValue)
{ 
	return static_cast<int>(dblValue < 0 ? dblValue - 0.5 : dblValue + 0.5);
}


void split_by_threads(int n, int ithr, int nthr, int& origin, int& split);


#endif
