#pragma once
#include "platform.h"
#include "glm/glm.hpp"
#include "glad/glad.h"
struct Buffer
{
    GLuint handle;
    GLenum type;
    u32 size;
    u32 head;
    void* data;//mapped data
};