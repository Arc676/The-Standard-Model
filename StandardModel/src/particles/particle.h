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

#ifndef PARTICLES_H
#define PARTICLES_H

#include <stdio.h>

#include "vector.h"

/**
 * Particle classification stored in 8 bits
 * gggg fftt
 * The two least significant bits (tt) indicate whether the particle is a boson (01) or fermion (10).
 * The next two bits (ff) indicate the subdivisions of fermions: leptons (01) and quarks (10).
 * The four most significant bits (gggg) uniquely identify each particle within its own family. The information stored in these bits
 * depends on the particle type.
 *
 * Leptons:
 * ntme fftt
 * The four bits store
 * n - whether the particle is a neutrino
 * t, m, or e - the generation of the particle (tau, muon, or electron)
 *
 * Quarks:
 * 32du fftt
 * The four bits store
 * 3 or 2 - whether the particle is in the third or second generation of quarks (ordered by increasing mass)
 * d or u - whether the particle is a down- or up-type quark
 *
 * Bosons:
 * whpg fftt
 * The four bits store
 * w, h, p, or g - whether the particle is a weak force mediator, the Higgs boson, a photon, or a gluon (a unique value is assigned
 * 			to each boson, except the W and Z bosons)
 */
typedef enum ParticleType {
	BOSON		= 0b0001,
	FERMION		= 0b0010,

	LEPTON		= 0b0100 | FERMION,
	QUARK		= 0b1000 | FERMION,

	ELECTRON	= (0b0001 << 4) | LEPTON,
	MUON		= (0b0010 << 4) | LEPTON,
	TAU		= (0b0100 << 4) | LEPTON,

	NEUTRINO	= (0b1000 << 4) | LEPTON,
	ENEUTRINO	= ELECTRON | NEUTRINO,
	MNEUTRINO	= MUON | NEUTRINO,
	TNEUTRINO	= TAU | NEUTRINO,

	UPQ		= (0b0001 << 4) | QUARK,
	DOWNQ		= (0b0010 << 4) | QUARK,

	CHARMQ		= (0b0100 << 4) | UPQ,
	STRANGEQ	= (0b0100 << 4) | DOWNQ,

	TOPQ		= (0b1000 << 4) | UPQ,
	BOTTOMQ		= (0b1000 << 4) | DOWNQ,

	GLUON		= (0b0001 << 4) | BOSON,
	PHOTON		= (0b0010 << 4) | BOSON,
	HIGGS		= (0b0100 << 4) | BOSON,
	ZWBOSON		= (0b1000 << 4) | BOSON
} ParticleType;

typedef struct Particle {
	int entityID;
	int strangeness;
	ParticleType type;
	Vector3 position;
	Vector3 velocity;
	double mass;
	double charge;
	float spin;
} Particle;

void particle_applyForce(Particle* particle, Vector3* force, double dt);

void particle_update(Particle* particle, double dt);

void particle_typeToString(Particle* particle, char* str);

void particle_switchToType(Particle* particle, ParticleType type);

void particle_switchToAntiparticle(Particle* particle);

double particle_getMassForType(ParticleType type, double charge);

#endif

#ifdef __cplusplus
}
#endif
