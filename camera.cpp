#pragma once;
#include "camera.h"

void Camera::update(GLFWwindow* window, float deltaTime)
{
    this->deltaTime = deltaTime;

    processInput(window);

    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    if(!glm::epsilonEqual(fov, targetFov, glm::epsilon<float>()))
    {
        fov = glm::mix(fov, targetFov, zoomLerpSpeed * deltaTime);
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Camera::processInput(GLFWwindow *window)
{
    glm::vec3 camDirection = glm::vec3(0.0f, 0.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camDirection += cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camDirection -= cameraFront ;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camDirection -= glm::normalize(glm::cross(cameraFront, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camDirection += glm::normalize(glm::cross(cameraFront, cameraUp));

    if(camDirection != glm::vec3(0.0f, 0.0f, 0.0f))
    {
        camDirection = glm::normalize(camDirection);
        cameraPos += camDirection * cameraSpeed * deltaTime;
    }
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static glm::vec2 lastMousePos = glm::vec2(xpos, ypos);
    static float yaw = -90.0f;
    static float pitch = 0.0f;

    glm::vec2 offset = glm::vec2(xpos - lastMousePos.x, -ypos + lastMousePos.y);
    lastMousePos = glm::vec2(xpos, ypos);
    offset *= mouseSensitivity;

    yaw   += offset.x;
    pitch += offset.y;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 camDirection;
    camDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    camDirection.y = sin(glm::radians(pitch));
    camDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(camDirection);
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    targetFov = fov - (float)yoffset * zoomSensitivity;
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}