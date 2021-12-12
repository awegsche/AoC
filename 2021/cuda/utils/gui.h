
#ifndef UTILS_GUI_H
#define UTILS_GUI_H

#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

#include "cuda_helpers.h"

using duration = std::chrono::duration<double>;


#pragma region GuiState

struct GuiState {
	ImGuiID dock_main_id;
	ImGuiID dock_id_bottom;
	ImGuiID dock_id_right;
	ImGuiID dock_id_left;

	GuiState();

	void display();
};

#pragma endregion

static void errorCallback(int error, char const* description);

#pragma region ImGui Draw Commands

void DrawStats(duration const& frame_time);

#pragma endregion

#pragma region ImGui Helpers

void beginFrameImGui();

void endFrameImGui();

void initImGui(GLFWwindow* window);

#pragma endregion

#pragma region GL and GLFW Helpers

GLFWwindow* initGLFW(char const* title, int width, int height);

bool initGL();

GLFWwindow* initGui(char const* title, int width, int height);

void cleanUpGui(GLFWwindow* window);

#pragma endregion

// TODO: refactor

class CudaTextureBuffer {
	GLuint vbo;
	GLuint scrwidth;
	GLuint scrheight;

public:
	CudaTextureBuffer(GLuint width, GLuint height)
		: scrwidth(width), scrheight(height), vbo(0) {

	}
	
	void createVBO() {
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		unsigned int size = scrwidth * scrheight * 3 * sizeof(float);
		glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		cudaGLRegisterBufferObject(vbo);
	}

	void map(void** buffer) const {
		cudaGLMapBufferObject(buffer, vbo);
	}

	void unmap() const {
		cudaGLUnmapBufferObject(vbo);
	}

	void draw() const {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexPointer(2, GL_FLOAT, 12, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glDrawArrays(GL_POINTS, 0, scrwidth * scrheight);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
};


#endif // UTILS_GUI_H
