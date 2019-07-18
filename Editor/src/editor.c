// Copyright (C) 2019 Arc676/Alessandro Vinciguerra <alesvinciguerra@gmail.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation (version 3).

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simconf.h"
#include "particle.h"

void printHelp() {
	printf("Available commands: help, add, remove, dt, done\n");
	printf("More details are available in the man page\n");
}

double getDT() {
	char input[10];
	printf("Enter time delta per frame: ");
	fgets(input, sizeof(input), stdin);
	return (double)strtol(input, (char**)NULL, 10);
}

int main() {
	char input[255];
	SimConf simConfig;

	printf("The Standard Model: Simulation Configuration Editor\n");

	simConfig.frame_dt = 1;
	simConfig.particleCount = 0;
	int size = 10;
	Particle* particles = malloc(size * sizeof(Particle));
	while (1) {
		printf("Enter command: ");
		fgets(input, sizeof(input), stdin);
		input[strlen(input) - 1] = 0;

		if (!strcmp(input, "help")) {
			printHelp();
		} else if (!strcmp(input, "dt")) {
			simConfig.frame_dt = getDT();
		} else if (!strcmp(input, "add")) {
			int idx = simConfig.particleCount++;
			particles[idx] = (Particle){
				idx, 0, ELECTRON, {0, 0, 0}, {1, 0, 0}, 1.0, 1.0, 0.5
			};
			if (simConfig.particleCount >= size) {
				size *= 2;
				Particle* new = realloc(particles, size * sizeof(Particle));
				if (new) {
					particles = new;
				} else {
					printf("Failed to allocate more memory to particles.\n");
					free(particles);
					return 1;
				}
			}
		} else if (!strcmp(input, "remove")) {
			printf("Particles so far:\n");
			char name[100];
			for (int i = 0; i < simConfig.particleCount; i++) {
				particle_typeToString(particles + i, name);
				printf("%d: %s\n", i, name);
			}
			printf("Enter index of particle to remove: ");
			fgets(input, sizeof(input), stdin);
			int idx = (int)strtol(input, (char**)NULL, 10);
			Particle* dst = particles + idx;
			memmove(dst, dst + 1, (size - idx - 1) * sizeof(Particle));
			simConfig.particleCount--;
		} else if (!strcmp(input, "done")) {
			break;
		} else {
			printf("Unknown command\n");
		}
	}

	printf("Enter filename: ");
	fgets(input, sizeof(input), stdin);
	input[strlen(input) - 1] = 0;
	FILE* file = fopen(input, "wb");
	simconf_writeSimulationConfiguration(&simConfig, file);
	tsmIO_writeParticlesRaw(particles, simConfig.particleCount, file);
	fclose(file);

	free(particles);

	return 0;
}
