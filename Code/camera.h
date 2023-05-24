#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
struct App;

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 target;
	float aspectRatio;
	float znear;
	float zfar;
	glm::mat4 projection;
	glm::mat4 view;
	Camera() {
		position = glm::vec3(0, 0, 10);
		target = glm::vec3(0, 0, 0);
		aspectRatio = 16 / 9;
		float znear = 0.1f;
		float zfar = 1000.0f;
		projection = glm::perspective(glm::radians(60.0f), aspectRatio, znear, zfar);
		view = glm::lookAt(position, target, glm::vec3(0, 1, 0));
	}
	Camera(glm::ivec2& displaySize);

	glm::mat4 TransformScale(const glm::vec3& scaleFactors)
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::scale(modelMatrix, scaleFactors);
		return modelMatrix;
	}
	glm::mat4 TransformPositionScale(const glm::vec3& pos, const glm::vec3& scaleFactors)
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		modelMatrix = glm::translate(modelMatrix,pos);
		modelMatrix = glm::scale(modelMatrix, scaleFactors);
		return modelMatrix;
	}

	glm::mat4 getWorldViewProjectionMatrix(glm::mat4 worldMatrix) {
		return (projection * view * worldMatrix);
	}

};
