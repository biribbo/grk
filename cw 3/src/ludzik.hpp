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

glm::vec3 cameraPos = glm::vec3(-1.f, 0, 0);
glm::vec3 cameraDir = glm::vec3(1.f, 0.f, 0.f);
GLuint bodyVAO, rightLegVAO, leftLegVAO, bodyVBO, rightLegVBO, leftLegVBO;

float aspectRatio = 1.f;

glm::mat4 leftLegMatrix() {
	float time = glfwGetTime();

	glm::vec3 scaleVector = glm::vec3(0.1f, 1.1f, 0.05f);
	glm::mat4 scale = glm::scale(scaleVector);

	glm::vec3 transVector = glm::vec3(0.f, 0.28f * (-sin(time) + 1.0f) - 0.5f, -0.25f);
	glm::mat4 translation = glm::translate(transVector);

	return translation * scale;
}

glm::mat4 rightLegMatrix() {
	float time = glfwGetTime();

	glm::vec3 scaleVector = glm::vec3(0.1f, 1.1f, 0.05f);
	glm::mat4 scale = glm::scale(scaleVector);

	glm::vec3 transVector = glm::vec3(0.f, 0.28f * (-sin(time) + 1.0f) - 0.5f, 0.25f);
	glm::mat4 translation = glm::translate(transVector);

	return translation * scale;
}

glm::mat4 bodyTransformationMatrix() {
	float time = glfwGetTime();
	glm::vec3 scaleVector = glm::vec3(6.0f, 1.1f, 0.5f);
	glm::mat4 scale = glm::scale(scaleVector);

	glm::vec3 transVector = glm::vec3(0.f, 0.25f * (-sin(time) + 1.0f), 0.f);
	glm::mat4 translation = glm::translate(transVector);

	float angle = glm::radians(45.0f * (0.5f * (cos(time) + 1.0)));
	glm::mat4 rotation = glm::mat4({
		1.f, 0.f, 0.f, 0.f,
		0.f, cos(angle), -sin(angle), 0.f,
		0.f, sin(angle), cos(angle), 0.f,
		0.f, 0.f, 0.f, 1.f
	});

	return scale * rotation; //* translation;
}

glm::mat4 createCameraMatrix(){
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

glm::mat4 createPerspectiveMatrix(float fov){
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
	glm::mat4 rightlegtransformation;
	glm::mat4 leftlegtransformation;

	glUseProgram(program_body);

	glBindVertexArray(bodyVAO);
	bodytransformation = createPerspectiveMatrix(45.0f) * createCameraMatrix() * bodyTransformationMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program_body, "transformation"), 1, GL_FALSE, (float*)&bodytransformation);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glBindVertexArray(rightLegVAO);
	rightlegtransformation = createPerspectiveMatrix(45.0f) * createCameraMatrix() * rightLegMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program_body, "transformation"), 1, GL_FALSE, (float*)&rightlegtransformation);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glBindVertexArray(leftLegVAO);
	leftlegtransformation = createPerspectiveMatrix(45.0f) * createCameraMatrix() * leftLegMatrix();
	glUniformMatrix4fv(glGetUniformLocation(program_body, "transformation"), 1, GL_FALSE, (float*)&leftlegtransformation);
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
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glGenVertexArrays(1, &rightLegVAO);
	glBindVertexArray(rightLegVAO);
	glGenBuffers(1, &rightLegVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rightLegVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glGenVertexArrays(1, &leftLegVAO);
	glBindVertexArray(leftLegVAO);
	glGenBuffers(1, &leftLegVBO);
	glBindBuffer(GL_ARRAY_BUFFER, leftLegVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
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
		cameraPos -= cameraSide * moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		cameraPos += cameraSide * moveSpeed;
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