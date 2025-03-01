#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    bool firstMouse;
    float lastX, lastY;

    Camera(glm::vec3 startPos);

    glm::mat4 getViewMatrix();

    void processKeyboard(int key, float deltaTime);
    void processMouse(double xpos, double ypos);
};

#endif
