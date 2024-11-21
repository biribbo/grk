#pragma once
#include "glew.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>

#include "Shader_Loader.h"
#include "Render_Utils.h"

#include "Box.cpp"
float points[] = {
0.0f,1.0f,0.f,1.f,
0.7818314824680298f,0.6234898018587336f,0.f,1.f,
0.9749279121818236f,-0.22252093395631434f,0.f,1.f,
0.43388373911755823f,-0.900968867902419f,0.f,1.f,
-0.433883739117558f,-0.9009688679024191f,0.f,1.f,
-0.9749279121818236f,-0.2225209339563146f,0.f,1.f,
-0.7818314824680299f,0.6234898018587334f,0.f,1.f,
};
float hues[] = {
	0.0f, 
	0.7142857142857143f, 
	0.42857142857142855f, 
	0.14285714285714285f, 
	0.8571428571428571f, 
	0.5714285714285714f, 
	0.2857142857142857f
};

GLuint indices[] = {
	0, 2, 4, 6, 1, 3, 5, 0
};

GLuint program;
Core::Shader_Loader shaderLoader;

unsigned int VAO;

void renderScene(GLFWwindow* window)
{
    glClearColor(0.0f, 0.0f, 0.99f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float time = glfwGetTime();

    // Bindowanie VAO
    glBindVertexArray(VAO);

    // Rysowanie z użyciem EBO i trybu GL_LINE_STRIP
    glDrawElements(GL_LINE_STRIP, 8, GL_UNSIGNED_INT, 0);

    glUseProgram(0);

    glfwSwapBuffers(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void init(GLFWwindow* window)
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);
    program = shaderLoader.CreateProgram("shaders/shader_2_1b.vert", "shaders/shader_2_1b.frag");

    // Generowanie i bindowanie VAO, VBO i EBO
    GLuint VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO - Bufor wierzchołków
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // EBO - Bufor indeksów
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Kolory - przechowywane w osobnej tablicy
    GLuint colorVBO;
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(hues), hues, GL_STATIC_DRAW);

    // Ustawienia atrybutów wierzchołków (pozycje)
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Ustawienia atrybutów wierzchołków (kolory)
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Odwiązujemy VAO
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