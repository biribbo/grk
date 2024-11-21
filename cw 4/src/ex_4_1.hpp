#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"

#include "Box.cpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>



GLuint program;
Core::Shader_Loader shaderLoader;

Core::RenderContext shipContext;
Core::RenderContext sphereContext;

glm::vec3 cameraPos = glm::vec3(-4.f, 0, 0);
glm::vec3 cameraDir = glm::vec3(1.f, 0.f, 0.f);
GLuint VAO,VBO;

float aspectRatio = 1.f;

glm::vec3 spaceshipPos;
glm::vec3 spaceshipDir;

glm::vec3 lightColor = glm::vec3(0.9, 0.7, 0.8);

glm::mat4 createCameraMatrix()
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir,glm::vec3(0.f,1.f,0.f)));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraSide,cameraDir));
	//glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);
	glm::mat4 cameraRotrationMatrix = glm::mat4({
		cameraSide.x,cameraSide.y,cameraSide.z,0,
		cameraUp.x,cameraUp.y,cameraUp.z ,0,
		-cameraDir.x,-cameraDir.y,-cameraDir.z,0,
		0.,0.,0.,1.,
		});

	cameraRotrationMatrix = glm::transpose(cameraRotrationMatrix);

	glm::mat4 cameraMatrix = cameraRotrationMatrix * glm::translate(-cameraPos);

	cameraMatrix = glm::mat4({
		1.,0.,0.,cameraPos.x,
		0.,1.,0.,cameraPos.y,
		0.,0.,1.,cameraPos.z,
		0.,0.,0.,1.,
		});

	cameraMatrix = glm::transpose(cameraMatrix);
	return Core::createViewMatrix(cameraPos, cameraDir, cameraUp);

	return cameraMatrix;
}

glm::mat4 createPerspectiveMatrix()
{
	glm::mat4 perspectiveMatrix;
	float n = 0.05;
	float f = 20.;
	float a1 = glm::min(aspectRatio, 1.f);
	float a2 = glm::min(1 / aspectRatio, 1.f);
	perspectiveMatrix = glm::mat4({
		1,0.,0.,0.,
		0.,1,0.,0.,
		0.,0.,(f+n) / (n - f),2*f * n / (n - f),
		0.,0.,-1.,0.,
		});

	
	perspectiveMatrix=glm::transpose(perspectiveMatrix);

	return perspectiveMatrix;
}

glm::mat4 createSpaceshipMatrix() {
	glm::vec3 spaceshipSide = glm::normalize(glm::cross(spaceshipDir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 spaceshipUp = glm::normalize(glm::cross(spaceshipSide, spaceshipDir));

	glm::mat4 spaceshipRotationMatrix = glm::mat4({
		spaceshipSide.x, spaceshipSide.y, spaceshipSide.z, 0.f,
		spaceshipUp.x, spaceshipUp.y, spaceshipUp.z, 0.f,
		-spaceshipDir.x, -spaceshipDir.y, -spaceshipDir.z, 0.f,
		0.f, 0.f, 0.f, 1.f
		});

	spaceshipRotationMatrix = glm::transpose(spaceshipRotationMatrix); // Transpose = inverse for orthonormal matrix

	glm::mat4 fixedRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.f, 1.f, 0.f));

	glm::mat4 spaceshipMatrix = glm::translate(glm::mat4(1.0f), spaceshipPos) * spaceshipRotationMatrix * fixedRotation;

	return spaceshipMatrix;
}


void drawObjectColor(Core::RenderContext& context, glm::mat4 modelMatrix, glm::vec3 color) {

	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * createCameraMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniform1f(glGetUniformLocation(program, "near"), 0.05f);
	glUniform1f(glGetUniformLocation(program, "far"), 20.0f);
	glUniform3f(glGetUniformLocation(program, "color"), color.r, color.g, color.b);
	Core::DrawContext(context);
}

void renderScene(GLFWwindow* window)
{
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 transformation;
	float time = glfwGetTime();

	glm::mat4 planet = glm::mat4({
		0.5f, 0., 0., 0.,
		0., 0.5f, 0., 3 * cos(time),
		0., 0., 0.5f, 3 * sin(time),
		0., 0., 0., 1.
		});

	planet = glm::transpose(planet);

	glm::mat4 moon = glm::mat4({
		0.1f, 0., 0., 0.,
		0., 0.1f, 0., 5 * sin(time),
		0., 0., 0.1f, 5 * cos(time),
		0., 0., 0., 1.0
		});
	moon = glm::transpose(moon);

	glm::mat4 moonOrbit = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f * time), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotation around Y-axis
	glm::mat4 moonTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f)); // Offset from the planet
	glm::mat4 moonScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.07f)); // Smaller sphere
	moon = planet * moonOrbit * moonTranslation * moonScale;

	glUseProgram(program);
	drawObjectColor(sphereContext, glm::mat4(), glm::vec3(0.3, 0.3, 0.2));
	drawObjectColor(sphereContext, planet, glm::vec3(0.9, 0.9, 0.2));
	drawObjectColor(sphereContext, moon, glm::vec3(0.5, 0.7, 0.9));  // Moon 2
	drawObjectColor(shipContext, createSpaceshipMatrix(), glm::vec3(0.5, 0.7, 0.9));  // Moon 2

	glUseProgram(0);
	glfwSwapBuffers(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	aspectRatio = width / float(height);
	glViewport(0, 0, width, height);
}
void loadModelToContext(std::string path, Core::RenderContext& context)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	context.initFromAssimpMesh(scene->mMeshes[0]);
}

void init(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	program = shaderLoader.CreateProgram("shaders/shader_4_1.vert", "shaders/shader_4_1.frag");

	loadModelToContext("./models/sphere.obj",sphereContext);

	loadModelToContext("./models/spaceship.obj", shipContext);
}

void shutdown(GLFWwindow* window)
{
	shaderLoader.DeleteProgram(program);
}

//obsluga wejscia
void processInput(GLFWwindow* window)
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f,1.f,0.f)));
	float angleSpeed = 0.05f;
	float moveSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraDir * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraDir * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		cameraPos += cameraSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		cameraPos -= cameraSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraDir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(cameraDir, 0));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraDir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(cameraDir, 0));

	cameraDir = glm::normalize(-cameraPos);
	spaceshipPos = cameraPos + 1.5f * cameraDir + glm::vec3(0.f, -0.5f, 0.f);
	spaceshipDir = cameraDir;
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