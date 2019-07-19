// Copyright (C) 2019 Arc676/Alessandro Vinciguerra <alesvinciguerra@gmail.com>
// Based on work by Ciro Santilli available at
// https://github.com/cirosantilli/cpp-cheat/blob/70b22ac36f92e93c94f951edb8b5af7947546525/opengl/offscreen.c

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation (version 3).

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#define ARG_ERROR 1
#define CORRUPT_INPUT 2
#define RENDER_ERROR 3

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#include "tsmio.h"

#include "glrecorder.h"

typedef struct SimulationParameters {
	Particle* particles;
	int particleCount;
	double time;
	FILE* simData;
} SimulationParameters;

// Render statistics (for FPS)
unsigned int startTime;

// Render parameters
unsigned int renderHeight = 0, renderWidth = 0;
RecorderParameters* params;
SimulationParameters* simParams;

// OpenGL data
GLuint fbo, rboColor, rboDepth;

void initGL(char* outputFilename) {
	int glget;
	/*if (offscreen) {
		// Framebuffer
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// Color renderbuffer
		glGenRenderbuffers(1, &rboColor);
		glBindRenderbuffer(GL_RENDERBUFFER, rboColor);
		// Storage must be one of:
		// GL_RGBA4, GL_RGB565, GL_RGB5_A1, GL_DEPTH_COMPONENT16, GL_STENCIL_INDEX8.
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB565, renderWidth, renderHeight);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboColor);

		// Depth renderbuffer.
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, renderWidth, renderHeight);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

		glReadBuffer(GL_COLOR_ATTACHMENT0);

		// Sanity check
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER));
		glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &glget);
		assert(renderWidth < (unsigned int)glget);
		assert(renderHeight < (unsigned int)glget);
	} else {*/
		glReadBuffer(GL_BACK);
	//}

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glViewport(0, 0, renderWidth, renderHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	startTime = glutGet(GLUT_ELAPSED_TIME);
	EncoderState state = glrecorder_startEncoder(params, outputFilename, AV_CODEC_ID_MPEG1VIDEO, 25);
	if (state != SUCCESS) {
		fprintf(stderr, "%s\n", glrecorder_stateToString(state));
		exit(RENDER_ERROR);
	}
}

void renderGL() {
	Particle* particles = simParams->particles;
	FILE* simData = simParams->simData;

	InputType type = tsmIO_getNextInputType(simData);
	if (type == SIMULATION_TERMINATED) {
		free(particles);
		fclose(simData);
		exit(0);
	}
	switch (type) {
	case TIME:
		tsmIO_readTime(&(simParams->time), simData);
		return;
	case MULTIPLE_PARTICLES:
		simParams->particleCount = tsmIO_readParticles(particles, simData);
		break;
	default:
		return;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw the simulation state
	for (int i = 0; i < simParams->particleCount; i++) {
		Vector3 position = particles[i].position;
		glLoadIdentity();
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(position.x, position.y, position.z);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(position.x + .5f, position.y, position.z);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(position.x, position.y + .5f, position.z);
		glEnd();
	}

	// if offscreen, glFlush();
	glutSwapBuffers();
	EncoderState state = glrecorder_recordFrame(params);
	if (state != SUCCESS) {
		fprintf(stderr, "%s\n", glrecorder_stateToString(state));
		exit(RENDER_ERROR);
	}
}

void idleGL() {
	glutPostRedisplay();
}

void deinitGL() {
	printf("Rendering framerate: %f\n", 1000.0 * params->currentFrame / (double)(glutGet(GLUT_ELAPSED_TIME) - startTime));
	EncoderState state = glrecorder_stopEncoder(params);
	if (state != SUCCESS) {
		fprintf(stderr, "%s\n", glrecorder_stateToString(state));
		exit(RENDER_ERROR);
	}
	/*
	if (offscreen) {
		glDeleteFramebuffers(1, &fbo);
		glDeleteRenderbuffers(1, &rboColor);
		glDeleteRenderbuffers(1, &rboDepth);
	}
	*/
	glrecorder_freeParams(params);
}

void printParticles(Particle* particles, int count) {
	char name[100];
	for (int i = 0; i < count; i++) {
		Particle p = particles[i];
		particle_typeToString(&p, name);
		printf("%s. Pos = (%f, %f, %f). Vel = (%f, %f, %f).\n",
			name, p.position.x, p.position.y, p.position.z, p.velocity.x, p.velocity.y, p.velocity.z);
	}
}

int renderSimulation(char* inputFile, char* outputFile) {
	FILE* simData = fopen(inputFile, "rb");
	int maxParticleCount;
	tsmIO_readInt(&maxParticleCount, simData);

	double t;
	tsmIO_readTime(&t, simData);

	if (tsmIO_getNextInputType(simData) != PARTICLE) {
		fclose(simData);
		fprintf(stderr, "Corrupt input file\n");
		return CORRUPT_INPUT;
	}

	params = glrecorder_initParams(renderWidth, renderHeight);

	Particle* particles = malloc(maxParticleCount * sizeof(Particle));
	int particleCount = tsmIO_readParticles(particles, simData);
	simParams = malloc(sizeof(SimulationParameters));
	simParams->particles = particles;
	simParams->particleCount = particleCount;
	simParams->time = t;
	simParams->simData = simData;

	GLint glutDisplay;
	int ac = 1;
	char* av[1] = {(char*)""};
	glutInit(&ac, av);
	glutInitWindowSize(renderWidth, renderHeight);
	glutDisplay = GLUT_DOUBLE;
	glutInitDisplayMode(glutDisplay | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("TSM Renderer");

	initGL(outputFile);
	glutDisplayFunc(renderGL);
	glutIdleFunc(idleGL);
	atexit(deinitGL);
	glutMainLoop();
	return 0;
}

int main(int argc, char* argv[]) {
	char inputFile[255];
	inputFile[0] = 0;
	char outputFile[255];
	outputFile[0] = 0;
	int opt;
	while ((opt = getopt(argc, argv, "i:o:h:w:")) != -1) {
		switch (opt) {
		case 'i':
			sprintf(inputFile, "%s", optarg);
			break;
		case 'o':
			sprintf(outputFile, "%s", optarg);
			break;
		case 'h':
			renderHeight = (unsigned int)strtol(optarg, (char**)NULL, 0);
			break;
		case 'w':
			renderWidth = (unsigned int)strtol(optarg, (char**)NULL, 0);
			break;
		default:
			fprintf(stderr, "Invalid arguments\n");
			return ARG_ERROR;
		}
	}
	if (strlen(inputFile) == 0) {
		fprintf(stderr, "No input file specified\n");
		return ARG_ERROR;
	}
	if (strlen(outputFile) == 0) {
		fprintf(stderr, "No output file specified\n");
		return ARG_ERROR;
	}

	return renderSimulation(inputFile, outputFile);
}
