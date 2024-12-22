#pragma once;
#include "camera.h"

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up    = glm::normalize(glm::cross(right, front));
}

Camera::Camera(glm::vec3 position,
               glm::vec3 up,
               glm::vec3 front)
{
    this->position = position;
    this->up = up;
    this->front = front;
    updateCameraVectors();
}


void Camera::update(GLFWwindow* window, float deltaTime)
{
    this->deltaTime = deltaTime;

    processInput(window);

    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 direction = glm::normalize(position - target);

    right = glm::normalize(glm::cross(up, direction));
    up = glm::cross(direction, right);

    if(!glm::epsilonEqual(fov, targetFov, glm::epsilon<float>()))
    {
        fov = glm::mix(fov, targetFov, zoomLerpSpeed * deltaTime);
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }

    updateCameraVectors();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Camera::processInput(GLFWwindow *window)
{
    glm::vec3 camDirection = glm::vec3(0.0f, 0.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camDirection += front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camDirection -= front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camDirection -= right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camDirection += right;

    if(camDirection != glm::vec3(0.0f, 0.0f, 0.0f))
    {
        camDirection = glm::normalize(camDirection);
        position += camDirection * speed * deltaTime;
    }
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static glm::vec2 lastMousePos = glm::vec2(xpos, ypos);

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
    front = glm::normalize(camDirection);
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    targetFov = fov - (float)yoffset * zoomSensitivity;
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}