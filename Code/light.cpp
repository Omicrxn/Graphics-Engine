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
}
