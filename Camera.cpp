#include "Camera.hpp"

Camera::Camera(glm::vec3 startPos)
    : position(startPos), front(glm::vec3(0.0f, 0.0f, -1.0f)), XZfront(glm::vec3(0.0f, 0.0f, -1.0f)),
    up(glm::vec3(0.0f, 1.0f, 0.0f)), yaw(-90.0f), 
    pitch(0.0f), speed(2.5f), sensitivity(0.1f),
    firstMouse(true), lastX(400), lastY(300) {}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(int key, float deltaTime) {
    float velocity = speed * deltaTime;
    if (key == GLFW_KEY_W) position += XZfront * velocity;
    if (key == GLFW_KEY_S) position -= XZfront * velocity;
    if (key == GLFW_KEY_A) position -= glm::normalize(glm::cross(front, up)) * velocity;
    if (key == GLFW_KEY_D) position += glm::normalize(glm::cross(front, up)) * velocity;
    if (key == GLFW_KEY_SPACE) position += up * velocity;
    if (key == GLFW_KEY_C) position -= up * velocity;

    std::cout << "Camera Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
}

void Camera::processMouse(double xpos, double ypos, float deltaTime) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch -= yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    glm::vec3 directionNoY;

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    XZfront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    XZfront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(direction);
    XZfront = glm::normalize(XZfront);

    std::cout << "Camera Yaw: " << yaw << ", Pitch: " << pitch << std::endl;
}
