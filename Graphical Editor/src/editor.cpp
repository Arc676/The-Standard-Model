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

#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "simconf.h"

SimConf simConfig;
char outputFilename[255] = "sim.conf";

Particle* particles;
int particleArraySize = 10;

void glfwErrorCallback(int error, const char* description) {
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

void saveConfiguration() {
	FILE* file = fopen(outputFilename, "wb");
	simconf_writeSimulationConfiguration(&simConfig, file);
	tsmIO_writeParticlesRaw(particles, simConfig.particleCount, file);
	fclose(file);
}

bool resizeArray() {
	particleArraySize *= 2;
	Particle* newSpace = (Particle*)realloc(particles, particleArraySize * sizeof(Particle));
	if (newSpace) {
		particles = newSpace;
	} else {
		printf("Failed to allocate more memory to particles.\n");
		free(particles);
		return false;
	}
	return true;
}

int main() {
	glfwSetErrorCallback(glfwErrorCallback);
	if (!glfwInit()) {
		return 1;
	}
	#if __APPLE
	const char* glslVersion = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#else
	const char* glslVersion = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	#endif

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(500, 500, "The Standard Model: Simulation Configuration Editor", NULL, NULL);
	if (!window) {
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize OpenGL loader\n");
		return 1;
	}

	simConfig.frame_dt = 1;
	simConfig.particleCount = 0;
	particles = (Particle*)malloc(particleArraySize * sizeof(Particle));

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glslVersion);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("The Standard Model: Simulation Configuration Editor")) {
			ImGui::Text("Time step per frame");
			ImGui::SameLine();
			ImGui::InputDouble("##DT", &(simConfig.frame_dt));

			if (ImGui::CollapsingHeader("Particles")) {
				if (ImGui::Button("Add Particle")) {
					int idx = simConfig.particleCount++;
					particles[idx] = (Particle){
						idx, 0, ELECTRON, {0, 0, 0}, {1, 0, 0}, 1.0, 1.0, 0.5
					};
					if (simConfig.particleCount >= particleArraySize) {
						resizeArray();
					}
				}

				for (int i = 0; i < simConfig.particleCount; i++) {
					if (ImGui::TreeNode("Particle")) {
						if (ImGui::Button("Delete Particle")) {
							Particle* dst = particles + i;
							memmove(dst, dst + 1, (particleArraySize - i - 1) * sizeof(Particle));
							simConfig.particleCount--;
							break;
						}
					}
				}
			}

			ImGui::Text("Filename");
			ImGui::SameLine();
			ImGui::InputText("##Filename", outputFilename, 255);
			if (ImGui::Button("Save Configuration")) {
				saveConfiguration();
			}

			if (ImGui::Button("Exit")) {
				break;
			}
			ImGui::End();
		}
		ImGui::Render();
		int displayW, displayH;
		glfwGetFramebufferSize(window, &displayW, &displayH);
		glViewport(0, 0, displayW, displayH);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();

	free(particles);

	return 0;
}
