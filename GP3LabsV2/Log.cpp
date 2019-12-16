#include "pch.h"
#include "Log.h"


char possibleType = 'a';
void Log::Debug(const std::string& msg,logType logMsg, const char* file, int line)
{
	std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg <<
		std::endl;

	if (possibleType == 'a') {
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
	}
	else if (possibleType == 'e' && logMsg == logType::ERROR) {
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
	}
	else if (possibleType == 't' && logMsg == logType::TRACE) {
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
	}
}
bool Log::CheckGLError(const char* file, int line)
{
	GLenum err;
	bool errorFound = false;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "GL ERROR(" << file << ", " << line << "): " <<
			glewGetErrorString(err) << std::endl;
		errorFound = true;
	}
	return errorFound;
}
