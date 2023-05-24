#pragma once
#include "platform.h"
struct App;
enum LightType
{
	LightType_Directional,
	LightType_Point
};
struct Light {
	LightType type;
	glm::vec3 color;
	glm::vec3 direction;
	glm::vec3 position;
	Light(LightType type,
		glm::vec3 color,
		glm::vec3 direction,
		glm::vec3 position) : type(type), color(color), direction(direction), position(position){}
	void initLight(App* app);
	void updateLight(App* app);
	void renderLight(App* app);
};