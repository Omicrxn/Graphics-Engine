#include "dice.h"
#include "engine.h"
#include <glad/glad.h>
void initDice(App* app, u32(*loadProgram)(App*, const char*, const char*),u32(*loadTexture2D)(App*, const char*))
{
	// TODO: Initialize your resources here!
	// - vertex buffers
	struct VertexV3V2
	{
		glm::vec3 pos;
		glm::vec2 uv;
	};
	const VertexV3V2 vertices[] = {
		{glm::vec3(-0.5,-0.5,0.0),glm::vec2(0.0,0.0)},//bottom-left vertex
		{glm::vec3(0.5,-0.5,0.0),glm::vec2(1.0,0.0)},//bottom-right vertex
		{glm::vec3(0.5, 0.5,0.0),glm::vec2(1.0,1.0)},//top-right vertex
		{glm::vec3(-0.5, 0.5,0.0),glm::vec2(0.0,1.0)},//top-right vertex
	};
	const u16 indices[] = {
		0,1,2,
		0,2,3
	};
	// - element/index buffers
	glGenBuffers(1, &app->embeddedVertices);
	glBindBuffer(GL_ARRAY_BUFFER, app->embeddedVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &app->embeddedElements);
	glBindBuffer(GL_ARRAY_BUFFER, app->embeddedElements);
	glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// - vaos
	glGenVertexArrays(1, &app->vao);
	glBindVertexArray(app->vao);
	glBindBuffer(GL_ARRAY_BUFFER, app->embeddedVertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexV3V2), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexV3V2), (void*)12);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, app->embeddedElements);    // - programs (and retrieve uniform indices)
	glBindVertexArray(0);
	// - textures
	app->texturedGeometryProgramIdx = loadProgram(app, "shaders.glsl", "TEXTURED_GEOMETRY");
	Program& texturedGeometryProgram = app->programs[app->texturedGeometryProgramIdx];
	app->programUniformTexture = glGetUniformLocation(texturedGeometryProgram.handle, "uTexture");
	app->diceTexIdx = loadTexture2D(app, "dice.png");
	app->whiteTexIdx = loadTexture2D(app, "color_white.png");
	app->blackTexIdx = loadTexture2D(app, "color_black.png");
	app->normalTexIdx = loadTexture2D(app, "color_normal.png");
	app->magentaTexIdx = loadTexture2D(app, "color_magenta.png");
}

void renderDice(App* app )
{
	// TODO: Draw your textured quad here!
		// - clear the framebuffer
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// - set the viewport
	glViewport(0, 0, app->displaySize.x, app->displaySize.y);
	// - set the blending state
	Program& programTexturedGeometry = app->programs[app->texturedGeometryProgramIdx];
	glUseProgram(programTexturedGeometry.handle);
	glBindVertexArray(app->vao);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// - bind the texture into unit 0
	glUniform1i(app->programUniformTexture, 0);
	glActiveTexture(GL_TEXTURE0);
	GLuint textureHandle = app->textures[app->diceTexIdx].handle;
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	// - bind the program 
	//   (...and make its texture sample from unit 0)
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	// - bind the vao
	glBindVertexArray(0);
	glUseProgram(0);
	// - glDrawElements() !!!
}
