
#include "camera.h"
Camera::Camera(glm::ivec2& displaySize)
{
	position = glm::vec3(0, 0, 20);
	target = glm::vec3(0, 0, 0);
	aspectRatio = (float)displaySize.x / (float)displaySize.y;
	float znear = 0.1f;
	float zfar = 1000.0f;
	projection = glm::perspective(glm::radians(60.0f), aspectRatio, znear, zfar);
	view = glm::lookAt(position, target, glm::vec3(0,1,0));
}
