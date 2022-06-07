// Author: Evgeny Berendeev
// Budker Institute of Nuclear Physics of Siberian Branch Russian Academy of Sciences
// beren@inp.nsk.su
// (c) 2022, for licensing details see the LICENSE file

#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H


const int SMAX = 4;
const int CELLS_SHIFT = 1;


//Physical constants
namespace CGS
{
	static const double e 	= 4.80320471257e-10;
	static const double c 	= 2.99792458e10;
	static const double m 	= 9.1094093837015e-28;
	static const double mc2 = 512.;
};
	static const double PI = 3.141592653589793;

#endif
