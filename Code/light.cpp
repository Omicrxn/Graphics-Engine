#include "light.h"
#include <glad/glad.h>
#include "buffer_management.h"
#include "engine.h"

void Light::initLight(App* app)
{

}

void Light::updateLight(App* app)
{
	AlignHead(app->cbuffer, sizeof(glm::vec4));
	PushUInt(app->cbuffer, type);
	PushVec3(app->cbuffer, color);
	PushVec3(app->cbuffer, direction);
	PushVec3(app->cbuffer, position);
	PushVec3(app->cbuffer, ambient);
	PushVec3(app->cbuffer, diffuse);
	PushVec3(app->cbuffer, specular);
	PushFloat(app->cbuffer,1.0f); //constant light
	PushFloat(app->cbuffer, 0.027f); //linear light
	PushFloat(app->cbuffer, 0.0028f); //quadratic light
}
