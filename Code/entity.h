#pragma once
#include "platform.h"
struct App;
class Entity {
public:
	glm::mat4 worldMatrix;
	u32 modelIndex;
	u32 localParamsOffset;
	u32 localParamsSize;
	u32 model;

	Entity(glm::vec3 pos,glm::vec3 scale,float rotation, u32& model):model(model) {
		worldMatrix = glm::mat4(1.0f);
		transformPositionScaleRotation(pos,scale,rotation);
	}
	void initEntity(App* app, const char* filename);
	void updateModel(App* app);
	void renderModel(App* app);

private:
	glm::mat4 transformPositionScaleRotation(const glm::vec3& pos, const glm::vec3& scaleFactor, float rotationDeg);

};


