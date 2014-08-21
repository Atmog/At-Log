#include "Log.hpp"

#include <ctime>
#include <sstream>

Log* Log::mInstance = nullptr;

Log::Log(std::string serverAddress, std::string appName, std::string appVersion)
{
    // Init default type
    mType = Log::None;

    // Handle Server Address & Port
    unsigned short port = 50002; // Default port
    std::size_t found = serverAddress.find(':');
    if (found != std::string::npos) // If you want to change your port : "xx.xxx.xxx.xxx:yyyyy"
    {
        std::istringstream iss(serverAddress.substr(found + 1, serverAddress.size()));
        iss >> port;
        serverAddress = serverAddress.substr(0, found);
    }

    // Handle AppName & AppVersion
    mAppName = appName;
    mAppVersion = appVersion;

    // Try to connect to the server
    if (mServer.connect(serverAddress,port) != sf::Socket::Done)
    {
        mIsNetworked = false;

        // Open log file
        mFile.open("client.log", std::ios::app);
    }
    else
    {
        mIsNetworked = true;

        // Try to send old unsent errors
        std::vector<std::string> unsentLines;
        std::ifstream file("client.log");
        if (file)
        {
            std::string line;
            while (std::getline(file,line))
            {
                if (mServer.send(sf::Packet() << "Log" << line) != sf::Socket::Done)
                {
                    unsentLines.push_back(line);
                }
            }
        }
        file.close();

        mFile.open("client.log");
        if (mFile)
        {
            for (std::size_t i = 0; i < unsentLines.size(); ++i)
            {
                mFile << unsentLines[i] << std::endl;
            }
        }
    }

    // Set Instance
    mInstance = this;
}

Log::~Log()
{
    // Close file
    if (mFile.is_open())
    {
        mFile.close();
    }

    // Disconnect from server
    if (mIsNetworked)
    {
        mServer.send(sf::Packet() << "Disconnect");
        mIsNetworked = false;
    }

    // Update Instance
    mInstance = nullptr;
}

Log* Log::getInstance()
{
    return mInstance;
}

bool Log::isOpen() const
{
    return mFile.is_open();
}

bool Log::isNetworked() const
{
    return mIsNetworked;
}

std::string Log::getTime(std::string timeFormat)
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[20];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,20,timeFormat.c_str(),timeinfo);

    return buffer;
}

std::string Log::getType(Log::Type type)
{
    std::string str;
    switch (type)
    {
        case Log::Type::Info:
            str = " [INFO] ";
            break;

        case Log::Type::Warning:
            str = " [WARNING] ";
            break;

        case Log::Type::Error:
            str = " [ERROR] ";
            break;

        default:
            str = " ";
            break;
    }
    return str;
}
