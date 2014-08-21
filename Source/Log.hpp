#ifndef LOG_HPP
#define LOG_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>

class Log
{
    public:
        enum Type
        {
            None,
            Info,
            Warning,
            Error,
        };

        Log(std::string serverAddress, std::string appName, std::string appVersion);
        ~Log();

        static Log* getInstance();

        bool isOpen() const;
        bool isNetworked() const;

        static std::string getTime(std::string timeFormat);
        static std::string getType(Log::Type type);

        friend Log& operator<<(Log& log, std::string message)
        {
            message = Log::getTime("%Y-%m-%d %X") + Log::getType(log.mType) + log.mAppName + " " + log.mAppVersion + " " + sf::IpAddress::getPublicAddress().toString() + " " + message;

            if (log.isNetworked())
            {
                if (log.mServer.send(sf::Packet() << "Log" << message) != sf::Socket::Done)
                {
                    log.mIsNetworked = false;
                    if (log.isOpen())
                    {
                        log.mFile << message << std::endl;
                    }
                    else
                    {
                        std::cout << message << std::endl;
                    }
                }
                return log;
            }
            else if (log.isOpen())
            {
                log.mFile << message << std::endl;
            }
            else
            {
                std::cout << message << std::endl;
            }
            return log;
        }

        friend Log& operator<<(Log& log, Type type)
        {
            log.mType = type;
            return log;
        }

    private:
        static Log* mInstance;

        std::string mAppName;
        std::string mAppVersion;

        sf::TcpSocket mServer;
        std::ofstream mFile;

        bool mIsNetworked;

        Type mType;
};

#endif // LOG_HPP
