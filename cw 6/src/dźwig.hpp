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

GLuint program, program_body;
Core::Shader_Loader shaderLoader;

Core::RenderContext shipContext;
Core::RenderContext sphereContext;

glm::vec3 cameraPos = glm::vec3(-3.f, 1.0, 0);
glm::vec3 cameraDir = glm::vec3(1.f, 0.f, 0.f);
GLuint bodyVAO, secondVAO, thirdVAO, bodyVBO, secondVBO, thirdVBO;
GLuint fourthVAO, fourthVBO;

float aspectRatio = 1.f;


glm::mat4 firstRotation(glm::mat4 matrix = glm::mat4(1.f)) {
	float time = glfwGetTime();

	float angle = glm::radians(45.0f * (cos(time)));
	glm::mat4 rotation = glm::rotate(matrix, angle, glm::vec3(0.f, 1.f, 0.f));
	
	return rotation;
}

float firstTranslation() {
	float time = glfwGetTime();
	return 0.05f * sin(time) + 0.27f;
}

glm::mat4 firstElementMatrix() {
	glm::vec3 scaleVector = glm::vec3(2.f, 0.4f, 0.67f);
	glm::mat4 scaling = glm::scale(scaleVector);

	return scaling;
}

glm::mat4 secondElement() {
	glm::vec3 scaleVector = glm::vec3(0.5f, 0.5f, 0.16f);
	glm::mat4 scaling = glm::scale(scaleVector);

	glm::vec3 transVector = glm::vec3(0.f, 0.2f, 0.f);
	glm::mat4 translation = glm::translate(transVector);

	return scaling * translation * firstRotation();
}


glm::mat4 thirdElement() {
	float time = glfwGetTime();
	float trLen = 0.05f * sin(time);

	glm::vec3 scaleVector = glm::vec3(0.0625f, 3.f, 0.03125f);
	glm::mat4 scaling = glm::scale(scaleVector);

	glm::vec3 transVector = glm::vec3(0.f, 0.27f + trLen, 0.f);
	glm::mat4 translation = glm::translate(transVector);

	return scaling * firstRotation() * translation;
}

glm::mat4 fourthElement() {
	float time = glfwGetTime();
	float trLen = 0.3f * sin(time);
	glm::vec3 scaleVector = glm::vec3(0.4f, 0.4f, 0.8f);
	glm::mat4 scaling = glm::scale(scaleVector);

	glm::vec3 transVector = glm::vec3(0.f, 3.7f + trLen, -0.5f);
	glm::mat4 translation = glm::translate(transVector);

	float angle = glm::radians(45.0f * (cos(time)));
	glm::mat4 rotation = glm::mat4({
			cos(angle), 0.f, -sin(angle), 0.f,
			0.f, 1.f, 0.f, 0.f,
			sin(angle), 0.f, cos(angle), 0.f,
			0.f, 0.f, 0.f, 1.f
		});

	return scaling * rotation * translation;
}

glm::mat4 createCameraMatrix() {
	glm::vec3 cameraSide = glm::cross(cameraDir, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::normalize(cameraDir);
	glm::vec3 cameraUp = glm::cross(cameraSide, cameraDir);
	glm::normalize(cameraUp);
	glm::mat4 cameraRotationMatrix = glm::mat4({
		cameraSide.x, cameraSide.y, cameraSide.z, 0.,
		cameraUp.x, cameraUp.y, cameraUp.z, 0.,
		-cameraDir.x, -cameraDir.y, -cameraDir.z, 0.,
		0., 0., 0., 1.
		});
	cameraRotationMatrix = glm::transpose(cameraRotationMatrix);

	glm::mat4 cameraTranslationMatrix = glm::mat4({
		1., 0., 0., -cameraPos.x,
		0., 1., 0., -cameraPos.y,
		0., 0., 1., -cameraPos.z,
		0., 0., 0., 1.
		});
	cameraTranslationMatrix = glm::transpose(cameraTranslationMatrix);

	glm::mat4 cameraMatrix = cameraRotationMatrix * cameraTranslationMatrix;

	return cameraMatrix;
}

glm::mat4 createPerspectiveMatrix(float fov) {
	float S = 1.0f / tan(glm::radians(fov / 2.0f));
	float n = 0.1f;
	float f = 100.0f;
	glm::mat4 perspectiveMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

	return perspectiveMatrix;
}

void renderScene(GLFWwindow* window)
{
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 bodytransformation;
	glm::mat4 secondtransformation;
	glm::mat4 thirdtransformation;
	glm::mat4 fourthtransformation;

	glUseProgram(program_body);

	glBindVertexArray(bodyVAO);
	bodytransformation = createPerspectiveMatrix(45.0f) * createCameraMatrix() * firstElementMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program_body, "transformation"), 1, GL_FALSE, (float*)&bodytransformation);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glBindVertexArray(secondVAO);
	secondtransformation = createPerspectiveMatrix(45.0f) * createCameraMatrix() * secondElement();
	glUniformMatrix4fv(glGetUniformLocation(program_body, "transformation"), 1, GL_FALSE, (float*)&secondtransformation);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glBindVertexArray(thirdVAO);
	thirdtransformation = createPerspectiveMatrix(45.0f) * createCameraMatrix() * thirdElement();
	glUniformMatrix4fv(glGetUniformLocation(program_body, "transformation"), 1, GL_FALSE, (float*)&thirdtransformation);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glBindVertexArray(fourthVAO);
	fourthtransformation = createPerspectiveMatrix(45.0f) * createCameraMatrix() * fourthElement();
	glUniformMatrix4fv(glGetUniformLocation(program_body, "transformation"), 1, GL_FALSE, (float*)&fourthtransformation);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glUseProgram(0);
	glfwSwapBuffers(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	aspectRatio = static_cast<float>(width) / static_cast<float>(height);
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
}

void init(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	program = shaderLoader.CreateProgram("shaders/shader.vert", "shaders/shader.frag");
	program_body = shaderLoader.CreateProgram("shaders/shader_body.vert", "shaders/shader_body.frag");

	glGenVertexArrays(1, &bodyVAO);
	glBindVertexArray(bodyVAO);
	glGenBuffers(1, &bodyVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bodyVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));

	glGenVertexArrays(1, &secondVAO);
	glBindVertexArray(secondVAO);
	glGenBuffers(1, &secondVBO);
	glBindBuffer(GL_ARRAY_BUFFER, secondVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));

	glGenVertexArrays(1, &thirdVAO);
	glBindVertexArray(thirdVAO);
	glGenBuffers(1, &thirdVBO);
	glBindBuffer(GL_ARRAY_BUFFER, thirdVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));

	glGenVertexArrays(1, &fourthVAO);
	glBindVertexArray(fourthVAO);
	glGenBuffers(1, &fourthVBO);
	glBindBuffer(GL_ARRAY_BUFFER, fourthVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
}

void shutdown(GLFWwindow* window)
{
	shaderLoader.DeleteProgram(program);
}

void processInput(GLFWwindow* window)
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraSide, cameraDir));
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
		cameraPos -= cameraSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		cameraPos += cameraSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos -= cameraUp * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += cameraUp * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraDir = glm::vec3(glm::eulerAngleY(angleSpeed) * glm::vec4(cameraDir, 0));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraDir = glm::vec3(glm::eulerAngleY(-angleSpeed) * glm::vec4(cameraDir, 0));

}

void renderLoop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		renderScene(window);
		glfwPollEvents();
	}
}
//}