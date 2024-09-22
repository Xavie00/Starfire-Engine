#pragma once
#include <GLFW/glfw3.h>
#include "components.h"
#include "cachedECS.h"
#include "eventManager.h"


bool key_pressed(GLFWwindow* window, int key) {
	if (glfwGetKey(window, key) == GLFW_PRESS) {
		return true;
	}
}

bool key_released(GLFWwindow* window, int key) {
	if (glfwGetKey(window, key) == GLFW_RELEASE) {
		return true;
	}
}

