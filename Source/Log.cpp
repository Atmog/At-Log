#include "Log.hpp"

Log* Log::mInstance = nullptr;

Log* Log::getInstance()
{
    if(mInstance == nullptr)
        mInstance = new Log();
    return mInstance;
}

void Log::setAppName(std::string appName)
{
    mAppName = appName;
}

void Log::setAppVersion(std::string appVersion)
{
    mAppVersion = appVersion;
}

void Log::setUsername(std::string username)
{
    mUsername = username;
}

