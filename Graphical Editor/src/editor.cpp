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

void RadioButtonF(const char* label, float* v, float v_button) {
	if (ImGui::RadioButton(label, *v == v_button)) {
		*v = v_button;
	}
}

void RadioButtonD(const char* label, double* v, double v_button) {
	if (ImGui::RadioButton(label, *v == v_button)) {
		*v = v_button;
	}
}

int indexForType(ParticleType type) {
	switch (type) {
	case ELECTRON:
		return 0;
	case MUON:
		return 1;
	case TAU:
		return 2;
	case ENEUTRINO:
		return 3;
	case MNEUTRINO:
		return 4;
	case TNEUTRINO:
		return 5;
	case UPQ:
		return 6;
	case DOWNQ:
		return 7;
	case CHARMQ:
		return 8;
	case STRANGEQ:
		return 9;
	case TOPQ:
		return 10;
	case BOTTOMQ:
		return 11;
	case GLUON:
		return 12;
	case PHOTON:
		return 13;
	case HIGGS:
		return 14;
	case ZWBOSON:
		return 16;
	default:
		return -1;
	}
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
						idx, 0, ELECTRON, {0, 0, 0}, {0, 0, 0}, particle_getMassForType(ELECTRON, 1), -1.0, 0.5f
					};
					if (simConfig.particleCount >= particleArraySize) {
						resizeArray();
					}
				}

				for (int i = 0; i < simConfig.particleCount; i++) {
					char buffer[100];
					sprintf(buffer, "Particle %d", i);
					if (ImGui::TreeNode(buffer)) {
						Particle* particle = particles + i;

						ImGui::Text("Particle Type");
						ImGui::SameLine();
						static const char* types[] = {
							"Electron", "Muon", "Tau",
							"Electron Neutrino", "Muon Neutrino", "Tau Neutrino",
							"Up Quark", "Down Quark",
							"Charm Quark", "Strange Quark",
							"Top Quark", "Bottom Quark",
							"Gluon", "Photon", "Higgs Boson", "Z or W Boson"
						};
						static const ParticleType ptypes[] = {
							ELECTRON, MUON, TAU,
							ENEUTRINO, MNEUTRINO, TNEUTRINO,
							UPQ, DOWNQ,
							CHARMQ, STRANGEQ,
							TOPQ, BOTTOMQ,
							GLUON, PHOTON, HIGGS, ZWBOSON
						};
						static const int PARTICLE_COUNT = 16;
						const char* selected = types[indexForType(particle->type)];
						sprintf(buffer, "##Type%d", i);
						if (ImGui::BeginCombo(buffer, selected)) {
							for (int i = 0; i < PARTICLE_COUNT; i++) {
								bool isSelected = (selected == types[i]);
								sprintf(buffer, "%s##TypeSelect%d", types[i], i);
								if (ImGui::Selectable(buffer, isSelected)) {
									selected = types[i];
									particle_switchToType(particle, ptypes[i]);
								}
								if (isSelected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}

						ImGui::Text("Strangeness");
						ImGui::SameLine();
						sprintf(buffer, "##Strangeness%d", i);
						ImGui::InputInt(buffer, &(particle->strangeness), 0, 0, ImGuiInputTextFlags_ReadOnly);

						ImGui::Text("Mass");
						ImGui::SameLine();
						sprintf(buffer, "##Mass%d", i);
						ImGui::InputDouble(buffer, &(particle->mass), 0, 0, "%g", ImGuiInputTextFlags_ReadOnly);

						ImGui::Text("Charge");
						ImGui::SameLine();
						if (particle->type == ZWBOSON) {
							double charge = particle->charge;
							sprintf(buffer, "+1##ChargeP%d", i);
							RadioButtonD(buffer, &(particle->charge), 1);
							ImGui::SameLine();
							sprintf(buffer, "0##Charge0%d", i);
							RadioButtonD(buffer, &(particle->charge), 0);
							ImGui::SameLine();
							sprintf(buffer, "-1##ChargeN%d", i);
							RadioButtonD(buffer, &(particle->charge), -1);
							if (charge != particle->charge) {
								particle->mass = particle_getMassForType(ZWBOSON, particle->charge);
							}
						} else {
							sprintf(buffer, "##Charge%d", i);
							ImGui::InputDouble(buffer, &(particle->charge), 0, 0, "%g", ImGuiInputTextFlags_ReadOnly);
						}

						ImGui::Text("Spin");
						ImGui::SameLine();
						if (particle->type & FERMION) {
							sprintf(buffer, "+1/2##Spin%d", i);
							RadioButtonF(buffer, &(particle->spin), 0.5f);
							ImGui::SameLine();
							sprintf(buffer, "-1/2##Spin%d", i);
							RadioButtonF(buffer, &(particle->spin), -0.5f);
						} else {
							if (particle->type == HIGGS) {
								sprintf(buffer, "##Spin%d", i);
								ImGui::InputFloat(buffer, &(particle->spin), 0, 0, "%g", ImGuiInputTextFlags_ReadOnly);
							} else {
								sprintf(buffer, "+1##Spin%d", i);
								RadioButtonF(buffer, &(particle->spin), 1.f);
								ImGui::SameLine();
								sprintf(buffer, "-1##Spin%d", i);
								RadioButtonF(buffer, &(particle->spin), -1.f);
							}
						}

						ImGui::Text("Initial Position (x,y,z)");
						sprintf(buffer, "##PosX%d", i);
						ImGui::InputDouble(buffer, &(particle->position.x));
						sprintf(buffer, "##PosY%d", i);
						ImGui::InputDouble(buffer, &(particle->position.y));
						sprintf(buffer, "##PosZ%d", i);
						ImGui::InputDouble(buffer, &(particle->position.z));

						ImGui::Text("Initial Velocity (x,y,z)");
						sprintf(buffer, "##VelX%d", i);
						ImGui::InputDouble(buffer, &(particle->velocity.x));
						sprintf(buffer, "##VelY%d", i);
						ImGui::InputDouble(buffer, &(particle->velocity.y));
						sprintf(buffer, "##VelZ%d", i);
						ImGui::InputDouble(buffer, &(particle->velocity.z));

						if (ImGui::Button("Switch to Antiparticle")) {
							particle_switchToAntiparticle(particle);
						}

						bool didDelete = false;
						if (ImGui::Button("Delete Particle")) {
							Particle* dst = particles + i;
							memmove(dst, dst + 1, (particleArraySize - i - 1) * sizeof(Particle));
							simConfig.particleCount--;
							didDelete = true;
						}
						ImGui::TreePop();
						if (didDelete) {
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
