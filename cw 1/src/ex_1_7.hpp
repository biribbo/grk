#pragma once
#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "ext.hpp"
#include <vector>

#include "Shader_Loader.h"
#include "Render_Utils.h"

GLuint program; // Shader ID

GLuint quadVAO;

Core::Shader_Loader shaderLoader;

// Przechowuje pozycjê kwadratu (x, y)
glm::vec2 quadPos(0.0f, 0.0f);

void renderScene(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

    float time = glfwGetTime();

    glUseProgram(program);

    // Definiowanie macierzy transformacji
    glm::mat4 transformation = glm::mat4(1.0f);
    transformation = glm::translate(transformation, glm::vec3(quadPos, 0.0f));  // Przesuwanie kwadratu do pozycji myszy

    // Przekazanie macierzy transformacji do shadera
    glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
    Core::drawVAOIndexed(quadVAO, 6);

    glUseProgram(0);

    glfwSwapBuffers(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Zmienna do przechowania przekszta³conych wspó³rzêdnych
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Przekszta³cenie wspó³rzêdnych okna do wspó³rzêdnych przestrzeni ekranu
    float x = 2.0f * xpos / width - 1.0f;
    float y = -(2.0f * ypos / height - 1.0f);

    // Przypisanie przekszta³conych wspó³rzêdnych do zmiennej quadPos
    quadPos = glm::vec2(x, y);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    printf("%f,%f\n", xpos, ypos);
}

void init(GLFWwindow* window) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    program = shaderLoader.CreateProgram("shaders/shader_1_2.vert", "shaders/shader_1_2.frag");

    // Kwadrat z pocz¹tku
    float points[] = {
        -0.1,-0.1,0.0,1.0,
         0.1,-0.1,0.0,1.0,
         0.1, 0.1,0.0,1.0,
        -0.1, 0.1,0.0,1.0,
    };
    unsigned int indices[] = {
        0,1,3,
        1,2,3,
    };
    quadVAO = Core::initVAOIndexed(points, indices, 4, 4, 6);
}

void shutdown(GLFWwindow* window)
{
    shaderLoader.DeleteProgram(program);
}

// obs³uga wejœcia
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// g³ówna funkcja pêtli renderuj¹cej
void renderLoop(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        renderScene(window);
        glfwPollEvents();
    }
}
