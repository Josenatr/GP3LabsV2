#include "pch.h"
#include "Log.h"


char chosenLogType = 'e';
void Log::Debug(const std::string& msg,logType logMsg, const char* file, int line)
{
	static int count = 0;
	static int logShow = 0;
	if (count == 0) {
		std::cout << "Which logs would you like to see? a = all logs, e = error logs only, t = trace logs only" << std::endl;
		count++;
	}
	if (chosenLogType == 'a') {
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
		std::cout << "All log levels will be shown" << std::endl;
	}
	else if (chosenLogType == 'e' && logMsg == logType::ERROR) {

		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
	}
	else if (chosenLogType == 't' && logMsg == logType::TRACE) {
		std::cout << "LOG DEBUG(" << file << ", " << line << "): " << msg << std::endl;
	}

	if (chosenLogType == 't' && logShow == 0) {
		std::cout << "Only trace logs will be shown" << std::endl;
		logShow++;
	}

	if (chosenLogType == 'e' && logShow == 0) {
		std::cout << "Only error logs will be shown" << std::endl;
		logShow++;
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
