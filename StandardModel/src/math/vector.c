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

#include "vector.h"

double norm2(Vector3* v) {
	return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

void vector3_add(Vector3* dst, Vector3* a, Vector3* b) {
	dst->x = a->x + b->x;
	dst->y = a->y + b->y;
	dst->z = a->z + b->z;
}

void vector3_mul(Vector3* dst, Vector3* v, double k) {
	dst->x = v->x * k;
	dst->y = v->y * k;
	dst->z = v->z * k;
}
