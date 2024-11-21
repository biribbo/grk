#include "glew.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>

#include "Shader_Loader.h"
#include "Render_Utils.h"

#include "ex_2_1a.hpp"

int main(int argc, char** argv)
{
    // Inicjalizacja glfw
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Ustawienia kontekstu OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Tworzenie okna
    GLFWwindow* window = glfwCreateWindow(500, 500, "FirstWindow", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Ładowanie OpenGL za pomocą GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed: " << glewGetErrorString(glewInit()) << std::endl;
        return -1;
    }

    glViewport(0, 0, 500, 500);

    // Inicjalizacja
    init(window);

    // Uruchomienie głównej pętli renderowania
    renderLoop(window);

    // Zakończenie i czyszczenie
    shutdown(window);
    glfwTerminate();
    return 0;
}
