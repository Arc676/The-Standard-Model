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

#include "tsmio.h"

InputType tsmIO_getNextInputType(FILE* file) {
	InputType type;
	fread(&type, sizeof(InputType), 1, file);
	return type;
}

void tsmIO_writeInputType(InputType type, FILE* file) {
	fwrite(&type, sizeof(InputType), 1, file);
}

void tsmIO_readInt(int* value, FILE* file) {
	fread(value, sizeof(int), 1, file);
}

void tsmIO_writeInt(int* value, FILE* file) {
	fwrite(value, sizeof(int), 1, file);
}

void tsmIO_readParticle(Particle* particle, FILE* file) {
	fread(particle, sizeof(Particle), 1, file);
}

void tsmIO_writeParticle(Particle* particle, FILE* file) {
	tsmIO_writeInputType(PARTICLE, file);
	fwrite(particle, sizeof(Particle), 1, file);
}

int tsmIO_readParticles(Particle* particles, FILE* file) {
	int count;
	tsmIO_readInt(&count, file);
	tsmIO_readParticlesRaw(particles, count, file);
	return count;
}

void tsmIO_readParticlesRaw(Particle* particles, int count, FILE* file) {
	fread(particles, sizeof(Particle), count, file);
}

void tsmIO_writeParticles(Particle* particles, int count, FILE* file) {
	tsmIO_writeInputType(MULTIPLE_PARTICLES, file);
	tsmIO_writeInt(&count, file);
	tsmIO_writeParticlesRaw(particles, count, file);
}

void tsmIO_writeParticlesRaw(Particle* particles, int count, FILE* file) {
	fwrite(particles, sizeof(Particle), count, file);
}

void tsmIO_readTime(double* time, FILE* file) {
	fread(time, sizeof(double), 1, file);
}

void tsmIO_writeTime(double* time, FILE* file) {
	tsmIO_writeInputType(TIME, file);
	fwrite(time, sizeof(double), 1, file);
}
