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

#include "simulation.h"

int terminationRequested = 0;

void runSimulation(char inputFile[], char outputFile[], double tfinal) {
	FILE* input = fopen(inputFile, "rb");
	SimConf simConfig;
	simconf_readSimulationConfiguration(&simConfig, input);
	Particle* particles = malloc(simConfig.particleCount * 2 * sizeof(Particle));
	tsmIO_readParticlesRaw(particles, simConfig.particleCount, input);
	fclose(input);

	FILE* output = fopen(outputFile, "wb");
	tsmIO_writeInt(&(simConfig.particleCount), output);

	double t = 0;
	tsmIO_writeTime(&t, output);
	tsmIO_writeParticles(particles, simConfig.particleCount, output);

	while (t <= tfinal && !terminationRequested) {
		t += simConfig.frame_dt;
		for (int i = 0; i < simConfig.particleCount; i++) {
			particle_update(particles + i, simConfig.frame_dt);
		}
		tsmIO_writeTime(&t, output);
		tsmIO_writeParticles(particles, simConfig.particleCount, output);
	}
	tsmIO_writeInputType(SIMULATION_TERMINATED, output);

	free(particles);
	fclose(output);
}
