#pragma once
#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"

#include "Shader_Loader.h"
#include "Render_Utils.h"

GLuint program; // Shader ID

GLuint quadVAO;

Core::Shader_Loader shaderLoader;


void renderScene(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

    // Zmienna "time" przechowuje czas który up³yn¹³ od uruchomienia aplikacji
    float time = glfwGetTime();

    glUseProgram(program);

    glm::mat4 translation = glm::mat4(
        1, 0, 0, -0.2f,
        0, 1, 0, sin(time) - 0.2f,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    translation = glm::transpose(translation);  // Transpose the translation matrix

    // Define the rotation matrix for the rotating square on the right
    float angle = time;  // Angle in radians
    glm::mat4 rotation = glm::mat4(
        cos(angle), 0, sin(angle), 0.78f,
        0, 1, 0, sin(time),
        -sin(angle), 0, cos(angle), 0,
        0, 0, 0, 1  // Up and down movement using sin(time)
    );
    rotation = glm::transpose(rotation);  // Transpose the rotation matrix

    // Draw the moving square with the translation matrix
    glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, &translation[0][0]);
    Core::drawVAOIndexed(quadVAO, 6);

    // Draw the rotating square with the rotation matrix
    glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, &rotation[0][0]);
    Core::drawVAOIndexed(quadVAO, 6);

    glUseProgram(0);

    glfwSwapBuffers(window);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void init(GLFWwindow* window) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    program = shaderLoader.CreateProgram("shaders/shader_1_2.vert", "shaders/shader_1_2.frag");

    //Przekopiuj kod do ladowania z poprzedniego zadania
    float verticles[] = {
        0.2f, 0.3f, 0.0f, 1.0f,
        -0.5f, 0.3f, 0.0f, 1.0f,
        -0.5f, -0.2f, 0.0f, 1.0f,
        0.2f, -0.2f, 0.0f, 1.0f
    };

    const unsigned int indexes[] = {
    0, 1, 2,
    2, 3, 0
    };

    quadVAO = Core::initVAOIndexed(verticles, indexes, 16, 4, 6);
}

void shutdown(GLFWwindow* window)
{
    shaderLoader.DeleteProgram(program);
}


//obsluga wejscia
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// funkcja jest glowna petla
void renderLoop(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        renderScene(window);
        glfwPollEvents();
    }
}
//}