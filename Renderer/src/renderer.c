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

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#include "tsmio.h"

#include "glrecorder.h"

// Render statistics (for FPS)
unsigned int startTime;

// Render parameters
unsigned int renderHeight = 0, renderWidth = 0;
RecorderParameters* params;
char* outputFilename = NULL;

// OpenGL data
GLuint fbo, rboColor, rboDepth;

void initGL() {
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
		exit(1);
	}
}

void renderGL() {
	// draw the simulation state
	// if offscreen, glFlush();
	glutSwapBuffers();
	EncoderState state = glrecorder_recordFrame(params);
	if (state != SUCCESS) {
		fprintf(stderr, "%s\n", glrecorder_stateToString(state));
		exit(1);
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
		exit(1);
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

int main(int argc, char* argv[]) {
	char inputFile[255];
	inputFile[0] = 0;
	int opt;
	while ((opt = getopt(argc, argv, "i:o:h:w:")) != -1) {
		switch (opt) {
		case 'i':
			sprintf(inputFile, "%s", optarg);
			break;
		case 'o':
			if (outputFilename) {
				free(outputFilename);
			}
			outputFilename = malloc(strlen(optarg));
			sprintf(outputFilename, "%s", optarg);
			break;
		case 'h':
			renderHeight = (unsigned int)strtol(optarg, (char**)NULL, 0);
			break;
		case 'w':
			renderWidth = (unsigned int)strtol(optarg, (char**)NULL, 0);
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

	GLint glutDisplay;
	int ac = 1;
	char* av[1] = {(char*)""};
	glutInit(&ac, av);
	glutInitWindowSize(renderWidth, renderHeight);
	glutDisplay = GLUT_DOUBLE;
	glutInitDisplayMode(glutDisplay | GLUT_RGBA | GLUT_DEPTH);

	params = glrecorder_initParams(renderWidth, renderHeight);

	/*
	initGL();
	glutDisplayFunc(renderGL);
	glutIdleFunc(idleGL);
	atexit(deinitGL);
	glutMainLoop();
	*/

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
