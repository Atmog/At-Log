#ifndef LOG_HPP
#define LOG_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <SFML/Network/Http.hpp>
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

	static Log* getInstance();

	friend Log& operator<<(Log& log, std::string message)
	{
	    sf::Http::Request request("/* here your uri */", sf::Http::Request::Post);
	    std::ostringstream stream;
	    stream << "action=insert";

	    if(log.mType == Type::Info)
	        stream << "&type=info";
	    else if(log.mType == Type::Warning)
		stream << "&type=warning";
	    else if(log.mType == Type::Error)
		stream << "&type=error";
	    else
		stream << "&type=none";

	    if(log.mAppName != "")
		stream << "&appname=" << log.mAppName;

	    if(log.mAppVersion != "")
		stream << "&appversion=" << log.mAppVersion;

	    stream << "&address=" << sf::IpAddress::getPublicAddress().toString();

	    if(log.mUsername != "")
		stream << "&username=" << log.mUsername;

	    stream << "&content=" << message;

	    request.setBody(stream.str());

	    sf::Http http("/* her your url */");
	    http.sendRequest(request);
	    return log;
	}

	friend Log& operator<<(Log& log, Type type)
	{
	    log.mType = type;
	    return log;
	}

        void setAppName(std::string appName);
	void setAppVersion(std::string appVersion);
	void setUsername(std::string username);


    private:
        static Log* mInstance;

        Type mType;
        std::string mAppName;
        std::string mAppVersion;
        std::string mUsername;
};

#endif // LOG_HPP
