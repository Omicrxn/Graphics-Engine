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
	Model& model = app->models[this->model];
	Mesh& mesh = app->meshes[model.meshIdx];
	for (u32 i = 0; i < mesh.submeshes.size(); ++i)
	{
		GLuint vao = FindVAO(mesh, i, programTexturedGeometry);
		glBindVertexArray(vao);
		u32 submeshMaterialIdx = model.materialIdx[i];
		Material& submeshMaterial = app->materials[submeshMaterialIdx];

		GLint ambientLoc = glGetUniformLocation(programTexturedGeometry.handle, "uMaterialAmbient");
		GLint diffuseLoc = glGetUniformLocation(programTexturedGeometry.handle, "uMaterialDiffuse");
		GLint specularLoc = glGetUniformLocation(programTexturedGeometry.handle, "uMaterialSpecular");
		GLint shininessLoc = glGetUniformLocation(programTexturedGeometry.handle, "uMaterialShininess");
		//Default values in case the material does not have a field
		glm::vec3 defaultAmbient = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 defaultDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 defaultSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
		float defaultShininess = 32.0f;

		// Use actual values if they are not zero
		glm::vec3 actualAmbient = (submeshMaterial.ambient == glm::vec3(0.0f, 0.0f, 0.0f)) ? defaultAmbient : submeshMaterial.ambient;
		glm::vec3 actualDiffuse = (submeshMaterial.diffuse == glm::vec3(0.0f, 0.0f, 0.0f)) ? defaultDiffuse : submeshMaterial.diffuse;
		glm::vec3 actualSpecular = (submeshMaterial.specular == glm::vec3(0.0f, 0.0f, 0.0f)) ? defaultSpecular : submeshMaterial.specular;
		float actualShininess = (submeshMaterial.shininess == 0.0f) ? defaultShininess : submeshMaterial.shininess;

		glUniform3fv(ambientLoc, 1, glm::value_ptr(actualAmbient));
		glUniform3fv(diffuseLoc, 1, glm::value_ptr(actualDiffuse));
		glUniform3fv(specularLoc, 1, glm::value_ptr(actualSpecular));
		glUniform1f(shininessLoc, actualShininess);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, app->textures[submeshMaterial.albedoTextureIdx].handle);
		glUniform1i(app->texturedGeometryProgramIdx, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, app->textures[submeshMaterial.specularTextureIdx].handle);
		glUniform1i(app->texturedGeometryProgramIdx, 1);

		Submesh& submesh = mesh.submeshes[i];
		glDrawElements(GL_TRIANGLES, submesh.indices.size(), GL_UNSIGNED_INT, (void*)(u64)submesh.indexOffset);

	}
	glBindVertexArray(0);
	glUseProgram(0);
}

glm::mat4 Entity::transformPositionScaleRotation(const glm::vec3& pos, const glm::vec3& scaleFactor, float rotationDeg)
{
	worldMatrix = glm::translate(worldMatrix, pos);
	worldMatrix = glm::scale(worldMatrix, scaleFactor);
	worldMatrix = glm::rotate(worldMatrix, glm::radians(rotationDeg), glm::vec3(0.0f, 1.0f, 0.0f));;
	return worldMatrix;
}

