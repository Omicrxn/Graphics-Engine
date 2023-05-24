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
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Light(LightType type,
		glm::vec3 color,
		glm::vec3 direction,
		glm::vec3 position) : type(type), color(color), direction(direction), position(position){
		ambient = glm::vec3(0.4f, 0.5f, 0.5f);
		diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		specular = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	void initLight(App* app);
	void updateLight(App* app);
	void renderLight(App* app);
};