#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include "Camera.hpp"
#include "Model.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

struct GameVars {
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    bool firstMouse = true;
    bool cursorEnabled = false;
    float lastX = 400, lastY = 300;
    int screenWidth = 800, screenHeight = 600;
    int frameCount = 0;
    float fpsTime = 0.0f;
};
GameVars gameVars;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    gameVars.screenWidth = width;
    gameVars.screenHeight = height;
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (gameVars.cursorEnabled) return;
    camera.processMouse(xpos, ypos, gameVars.deltaTime);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        gameVars.cursorEnabled = false;
    }
}

void processInput(GLFWwindow* window) {
    float currentFrame = glfwGetTime();
    gameVars.deltaTime = currentFrame - gameVars.lastFrame;
    gameVars.lastFrame = currentFrame;

    std::cout << "Delta Time: " << gameVars.deltaTime << std::endl;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.processKeyboard(GLFW_KEY_W, gameVars.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.processKeyboard(GLFW_KEY_S, gameVars.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.processKeyboard(GLFW_KEY_A, gameVars.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.processKeyboard(GLFW_KEY_D, gameVars.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.processKeyboard(GLFW_KEY_SPACE, gameVars.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) camera.processKeyboard(GLFW_KEY_C, gameVars.deltaTime);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        gameVars.cursorEnabled = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void updateFPSCounter(GLFWwindow* window) {
    gameVars.frameCount++;
    float currentTime = glfwGetTime();
    float elapsedTime = currentTime - gameVars.fpsTime;

    if (elapsedTime >= 1.0f) {
        float fps = gameVars.frameCount / elapsedTime;
        std::stringstream ss;
        ss << "Game window - FPS: " << fps;
        glfwSetWindowTitle(window, ss.str().c_str());

        gameVars.frameCount = 0;
        gameVars.fpsTime = currentTime;
    }
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(gameVars.screenWidth, gameVars.screenHeight, "Game window", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Model model("C:/Users/julie/source/repos/FPS/FPS/sniper.fbx");

    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aTexCoord;

        out vec2 TexCoord;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec2 TexCoord;

        out vec4 FragColor;

        uniform sampler2D texture1;

        void main() {
            FragColor = texture(texture1, TexCoord);
        }
    )";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glEnable(GL_DEPTH_TEST);

    gameVars.fpsTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)gameVars.screenWidth / (float)gameVars.screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        glUseProgram(shaderProgram);
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        model.draw();

        updateFPSCounter(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
