#pragma once
#ifdef _DEBUG
#define LOG_DEBUG(msg, logMsg) Log::Debug(msg, logMsg, __FILE__, __LINE__)
#define GL_ATTEMPT(func) func; \
if(Log::CheckGLError(__FILE__, __LINE__)) __debugbreak()
#define CHECK_GL_ERROR() Log::CheckGLError(__FILE__, __LINE__)
#else //shouldn't really be outputting to console if this is a release build
#define LOG_DEBUG(msg)
#define GL_ATTEMPT(func) func
#define CHECK_GL_ERROR()
#endif
//TODO: implement different log levels
enum logType
{
	TRACE, ERROR
};
class Log
{
private:
	Log();
public:
	static void Debug(const std::string& msg, logType logMsg, const char* file, int line);
	static bool CheckGLError(const char* file, int line);
};
