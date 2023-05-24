#pragma once
#include "platform.h"
struct App;
class Entity {
public:
	glm::mat4 worldMatrix;
	u32 modelIndex;
	u32 localParamsOffset;
	u32 localParamsSize;

	Entity(glm::vec3 pos) {
		worldMatrix = glm::mat4(1.0f);
		transformPositionScale(pos, glm::vec3(1, 1, 1));
	}
	void initEntity(App* app, const char* filename);
	void updateModel(App* app);
	void renderModel(App* app);

private:
	glm::mat4 transformPositionScale(const glm::vec3& pos, const glm::vec3& scaleFactor);

};


