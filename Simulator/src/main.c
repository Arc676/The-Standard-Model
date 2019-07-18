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
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "simulation.h"

extern int terminationRequested;

void termRequested(int signal) {
	terminationRequested = 1;
}

int main(int argc, char* argv[]) {
	int signals[4] = { SIGUSR2, SIGINT, SIGQUIT, SIGTERM };
	for (int i = 0; i < 4; i++) {
		if (signal(signals[i], termRequested)) {
			fprintf(stderr, "Failed to set signal handler\n");
			return 2;
		}
	}
	char outputFile[255];
	outputFile[0] = 0;
	char inputFile[255];
	inputFile[0] = 0;
	double finalTime = 100;
	int opt;
	while ((opt = getopt(argc, argv, "o:i:T:")) != -1) {
		switch (opt) {
		case 'o':
			sprintf(outputFile, "%s", optarg);
			break;
		case 'i':
			sprintf(inputFile, "%s", optarg);
			break;
		case 'T':
			finalTime = (double)strtol(optarg, (char**)NULL, 10);
			break;
		default:
			fprintf(stderr, "Invalid arguments\n");
			return 1;
		}
	}
	if (strlen(outputFile) == 0) {
		fprintf(stderr, "No output file specified\n");
		return 1;
	}
	if (strlen(inputFile) == 0) {
		fprintf(stderr, "No input file specified\n");
		return 1;
	}
	runSimulation(inputFile, outputFile, finalTime);
	return 0;
}
