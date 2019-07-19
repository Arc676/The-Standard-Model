//Copyright (C) 2019  Arc676/Alessandro Vinciguerra <alesvinciguerra@gmail.com>

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation (version 3).

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifdef __cplusplus
extern "C" {
#endif

#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef struct Vector3 {
	double x;
	double y;
	double z;
} Vector3;

double norm2(Vector3* v);

void vector3_add(Vector3* dst, Vector3* a, Vector3* b);

void vector3_mul(Vector3* dst, Vector3* v, double k);

#endif

#ifdef __cplusplus
}
#endif
