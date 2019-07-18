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

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "tsmio.h"

void printParticles(Particle* particles, int count) {
	char name[100];
	for (int i = 0; i < count; i++) {
		Particle p = particles[i];
		particle_typeToString(&p, name);
		printf("%s. Pos = (%f, %f, %f). Vel = (%f, %f, %f).\n",
			name, p.position.x, p.position.y, p.position.z, p.velocity.x, p.velocity.y, p.velocity.z);
	}
}

int main(int argc, char* argv[]) {
	char inputFile[255];
	inputFile[0] = 0;
	int opt;
	while ((opt = getopt(argc, argv, "i:o:")) != -1) {
		switch (opt) {
		case 'i':
			sprintf(inputFile, "%s", optarg);
			break;
		default:
			fprintf(stderr, "Invalid arguments\n");
			return 1;
		}
	}
	if (strlen(inputFile) == 0) {
		fprintf(stderr, "No input file specified\n");
		return 1;
	}
	FILE* simData = fopen(inputFile, "rb");
	int maxParticleCount;
	tsmIO_readInt(&maxParticleCount, simData);

	double t = 0;
	tsmIO_readTime(&t, simData);

	if (tsmIO_getNextInputType(simData) != PARTICLE) {
		fclose(simData);
		fprintf(stderr, "Corrupt input file\n");
		return 2;
	}

	Particle* particles = malloc(maxParticleCount * sizeof(Particle));
	int particleCount = tsmIO_readParticles(particles, simData);

	InputType type;
	while ((type = tsmIO_getNextInputType(simData)) != SIMULATION_TERMINATED) {
		switch (type) {
		case TIME:
			tsmIO_readTime(&t, simData);
			break;
		case MULTIPLE_PARTICLES:
			particleCount = tsmIO_readParticles(particles, simData);
			printf("Time: %f\n", t);
			printParticles(particles, particleCount);
			break;
		default:
			break;
		}
	}

	free(particles);
	fclose(simData);
	return 0;
}
