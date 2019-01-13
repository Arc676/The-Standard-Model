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

#include "particle.h"

void particle_applyForce(Particle* particle, Vector3* force, double dt) {
	if (!particle->mass) {
		return;
	}
	Vector3 acceleration;
	vector3_mul(&acceleration, force, dt / particle->mass);
	vector3_add(&(particle->velocity), &(particle->velocity), &acceleration);
}

void particle_update(Particle* particle, double dt) {
	Vector3 ds = particle->velocity;
	vector3_mul(&ds, &ds, dt);
	vector3_add(&(particle->position), &(particle->position), &ds);
}

void particle_typeToString(Particle* particle, char* str) {
	switch (particle->type) {
	case BOSON:
		sprintf(str, "Boson");
		break;
	case FERMION:
		sprintf(str, "Fermion");
		break;
	case LEPTON:
		sprintf(str, "Lepton");
		break;
	case QUARK:
		sprintf(str, "Quark");
		break;
	case ELECTRON:
		sprintf(str, "Electron");
		break;
	case MUON:
		sprintf(str, "Muon");
		break;
	case TAU:
		sprintf(str, "Tau");
		break;
	case NEUTRINO:
		sprintf(str, "Neutrino");
		break;
	case ENEUTRINO:
		sprintf(str, "Electron Neutrino");
		break;
	case MNEUTRINO:
		sprintf(str, "Muon Neutrino");
		break;
	case TNEUTRINO:
		sprintf(str, "Tau Neutrino");
		break;
	case UPQ:
		sprintf(str, "Up Quark");
		break;
	case DOWNQ:
		sprintf(str, "Down Quark");
		break;
	case CHARMQ:
		sprintf(str, "Charm Quark");
		break;
	case STRANGEQ:
		sprintf(str, "Strange Quark");
		break;
	case TOPQ:
		sprintf(str, "Top Quark");
		break;
	case BOTTOMQ:
		sprintf(str, "Bottom Quark");
		break;
	case GLUON:
		sprintf(str, "Gluon");
		break;
	case PHOTON:
		sprintf(str, "Photon");
		break;
	case HIGGS:
		sprintf(str, "Higgs Boson");
		break;
	case ZWBOSON:
		if (particle->charge > 0) {
			sprintf(str, "W+ Boson");
		} else if (particle->charge < 0) {
			sprintf(str, "W- Boson");
		} else {
			sprintf(str, "Z Boson");
		}
		break;
	}
}
