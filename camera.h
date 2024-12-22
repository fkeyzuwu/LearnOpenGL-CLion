#pragma once

#include <GLFW/glfw3.h>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

class Camera {
private:
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    const float cameraSpeed = 20.f; // adjust accordingly
    const float mouseSensitivity = 0.2f;
    const float zoomSensitivity = 10.0f;
    const float zoomLerpSpeed = 8.0f;
    float deltaTime = 0.0f;
public:
    float targetFov = 45.0f;
    float fov = 45.0f;

    void update(GLFWwindow* window, float deltaTime);
    glm::mat4 getViewMatrix();
    void processInput(GLFWwindow *window);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

