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

void tsmIO_readParticle(Particle* particle, FILE* file) {
	fread(particle, sizeof(Particle), 1, file);
}

void tsmIO_writeParticle(Particle* particle, FILE* file) {
	tsmIO_writeInputType(PARTICLE, file);
	fwrite(particle, sizeof(Particle), 1, file);
}

void tsmIO_readParticles(Particle* particles, int count, FILE* file) {
	fread(particles, sizeof(Particle), count, file);
}

void tsmIO_writeParticles(Particle* particles, int count, FILE* file) {
	fwrite(particles, sizeof(Particle), count, file);
}
