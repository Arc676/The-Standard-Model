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

#include "simconf.h"

void simconf_readSimulationConfiguration(SimConf* simconf, FILE* file) {
	fread(simconf, sizeof(SimConf), 1, file);
}

void simconf_writeSimulationConfiguration(SimConf* simconf, FILE* file) {
	fwrite(simconf, sizeof(SimConf), 1, file);
}

void simconf_readInitialParticles(SimConf* simconf, Particle* particles, FILE* file) {
	tsmIO_readParticles(particles, simconf->particleCount, file);
}
