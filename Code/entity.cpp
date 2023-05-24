#include "entity.h"
#include "engine.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "buffer_management.h"

GLuint FindVAO(Mesh& mesh, u32 submeshIndex, const Program& program) {
	Submesh& submesh = mesh.submeshes[submeshIndex];
	//Try finding a vao for this submesh/program
	for (u32 i = 0; i < (u32)submesh.vaos.size(); ++i)
	{
		if (submesh.vaos[i].programHandle == program.handle) {
			return submesh.vaos[i].handle;
		}
	}
	GLuint vaoHandle = 0;
	//Create a new vao for this submesh/program
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBufferHandle);

	//We have to link all vertex inputs attributes to attributes in the vertex buffer
	for (u32 i = 0; i < program.vertexInputLayout.attributes.size(); ++i) {
		bool attributeWasLinked = false;
		for (u32 j = 0; j < submesh.vertexBufferLayout.attributes.size(); ++j)
		{
			if (program.vertexInputLayout.attributes[i].location == submesh.vertexBufferLayout.attributes[j].location)
			{
				const u32 index = submesh.vertexBufferLayout.attributes[j].location;
				const u32 ncomp = submesh.vertexBufferLayout.attributes[j].componentCount;
				const u32 offset = submesh.vertexBufferLayout.attributes[j].offset + submesh.vertexOffset;
				const u32 stride = submesh.vertexBufferLayout.stride;
				glVertexAttribPointer(index, ncomp, GL_FLOAT, GL_FALSE, stride, (void*)(u64)offset);
				glEnableVertexAttribArray(index);
				attributeWasLinked = true;
				break;
			}
		}
		assert(attributeWasLinked);
	}
	glBindVertexArray(0);



	//Store it in the list of vaos for this submesh
	Vao vao = { vaoHandle,program.handle };
	submesh.vaos.push_back(vao);

	return vaoHandle;
}





void Entity::updateModel(App* app)
{
	AlignHead(app->cbuffer, app->uniformBlockAlignment);

	this->localParamsOffset = app->cbuffer.head;
	PushMat4(app->cbuffer, this->worldMatrix);
	PushMat4(app->cbuffer, app->camera->getWorldViewProjectionMatrix(worldMatrix));
	this->localParamsSize = app->cbuffer.head - this->localParamsOffset;
}

void Entity::renderModel(App* app)
{

	//Binding buffer ranges to uniform blocks
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, app->cbuffer.handle, this->localParamsOffset, this->localParamsSize);
	
	Program& programTexturedGeometry = app->programs[app->texturedGeometryProgramIdx];
	glUseProgram(programTexturedGeometry.handle);
	Model& model = app->models[app->model];
	Mesh& mesh = app->meshes[model.meshIdx];
	for (u32 i = 0; i < mesh.submeshes.size(); ++i)
	{
		GLuint vao = FindVAO(mesh, i, programTexturedGeometry);
		glBindVertexArray(vao);
		u32 submeshMaterialIdx = model.materialIdx[i];
		Material& submeshMaterial = app->materials[submeshMaterialIdx];

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, app->textures[submeshMaterial.albedoTextureIdx].handle);
		glUniform1i(app->texturedGeometryProgramIdx, 0);

		Submesh& submesh = mesh.submeshes[i];
		glDrawElements(GL_TRIANGLES, submesh.indices.size(), GL_UNSIGNED_INT, (void*)(u64)submesh.indexOffset);

	}
	glBindVertexArray(0);
	glUseProgram(0);
}

glm::mat4 Entity::transformPositionScale(const glm::vec3& pos, const glm::vec3& scaleFactor)
{
	worldMatrix = glm::translate(worldMatrix, pos);
	worldMatrix = glm::scale(worldMatrix, scaleFactor);
	return worldMatrix;
}

