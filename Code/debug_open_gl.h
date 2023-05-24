#include "platform.h"
#include <glad/glad.h>

void OnGlError(GLenum source, GLenum type,GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		return;
	ELOG("*OpenGL debug message: %5", message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API: ELOG(" - source: GL_DEBUG_SOURCE_API"); break; // Calls to the OpenGL API
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: ELOG(" - source: GL_DEBUG_SOURCE_WINDOW_SYSTEM"); break; // Calls to a window-system API
	case GL_DEBUG_SOURCE_SHADER_COMPILER: ELOG(" - source: GL_DEBUG_SOURCE_SHADER_COMPILER"); break; // A compiler for a shading language
	case GL_DEBUG_SOURCE_THIRD_PARTY:ELOG(" - source: GL_DEBUG_SOURCE_THIRD_PARTY"); break; // An application associated with OpenGL
	case GL_DEBUG_SOURCE_APPLICATION: ELOG(" - source: GL_DEBUG_SOURCE_APPLICATION"); break; // Generated by the user of this applicat
	case GL_DEBUG_SOURCE_OTHER: ELOG(" - source: GL_DEBUG_ SOURCE_OTHER*"); break; // Sone source that isn't one of these
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		ELOG(" - type: GL_DEBUG_TYPE_ERROR*"); break; // An error, typically from the API
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: ELOG(" - type: GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR"); break; // Some behavior marked deprecated I
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: ELOG(" - type: GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR"); break; // Something has invoked undefined bi
	case GL_DEBUG_TYPE_PORTABILITY:ELOG(" - type: GL_DEBUG_TYPE_PORTABILITY"); break; // Some functionality the user relies upon
	case GL_DEBUG_TYPE_PERFORMANCE:ELOG(" - type: GL DEBUG TYPE PERFORMANCE"); break; // Code has triggered possible performance
	case GL_DEBUG_TYPE_MARKER:ELOG(" - type: GL_DEBUG_TYPE_MARKER"); break; // Conmand stream annotation
	case GL_DEBUG_TYPE_PUSH_GROUP:ELOG(" - type: GL_ DEBUG TYPE_PUSH GROUP"); break; // Group pushing
	case GL_DEBUG_TYPE_POP_GROUP:ELOG(" - type: GL_DEBUG_TYPE_POP_GROUP"); break; // foo
	case GL_DEBUG_TYPE_OTHER:ELOG(" - type: GL_DEBUG_TYPE_OTHER"); break; // Some type that isn't one of these
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH: ELOG("* -severity: GL_DEBUG_SEUERITY_HIGH"); break; // All OpenGL Errors, shader compilation/link
	case GL_DEBUG_SEVERITY_MEDIUM: ELOG(" - severity: GL_DEBUG_SEVERITY_MEDIUM"); break; // Major performance warnings, shader compi:
	case GL_DEBUG_SEVERITY_LOW: ELOG(" - severity: GL_DEBUG_SEVERITY_LOW*"); break; // Redundant state change perfornance warning,
	case GL_DEBUG_SEVERITY_NOTIFICATION: ELOG("- severity: GL_DEBUG_SEVERITY_NOTIFICATION"); break; // Anything that isn't an error or per
	}


}