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

#ifndef TSMIO_H
#define TSMIO_H

#include <stdio.h>

#include "particle.h"

typedef enum InputType {
	PARTICLE = 0,
	MULTIPLE_PARTICLES,
	TIME,
	SIMULATION_TERMINATED
} InputType;

InputType tsmIO_getNextInputType(FILE* file);

void tsmIO_writeInputType(InputType type, FILE* file);

void tsmIO_readInt(int* value, FILE* file);

void tsmIO_writeInt(int* value, FILE* file);

void tsmIO_readParticle(Particle* particle, FILE* file);

void tsmIO_writeParticle(Particle* particle, FILE* file);

int tsmIO_readParticles(Particle* particles, FILE* file);

void tsmIO_readParticlesRaw(Particle* particles, int count, FILE* file);

void tsmIO_writeParticles(Particle* particles, int count, FILE* file);

void tsmIO_writeParticlesRaw(Particle* particles, int count, FILE* file);

void tsmIO_readTime(double* time, FILE* file);

void tsmIO_writeTime(double* time, FILE* file);

#endif
