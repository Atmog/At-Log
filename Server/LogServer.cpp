#include "LogServer.hpp"

#include <SFML/System/Sleep.hpp>

namespace at
{

LogServer::LogServer(unsigned short port)
: mThread(&LogServer::run, this)
, mWaitingThreadEnd(false)
, mFile("server.log", std::ios::app)
, mPeers(1)
, mClients(0)
{
    mPeers[0].reset(new RemotePeer());
	mListener.setBlocking(false);
	if (mListener.listen(port) == sf::TcpListener::Done)
	{
	    std::cout << "Server is running !" << std::endl;
	    mThread.launch();
	}
}

LogServer::~LogServer()
{
    std::cout << "Stopping the server..." << std::endl;
    mWaitingThreadEnd = true;
    mThread.wait();
    mListener.close();
}

void LogServer::run()
{
    while (!mWaitingThreadEnd)
    {
        handlePackets();
        handleConnections();

        sf::sleep(sf::milliseconds(200));
    }
}

void LogServer::handlePackets()
{
    bool detectedTimeout = false;

	for (PeerPtr& peer : mPeers)
	{
		if (peer->ready)
		{
			sf::Packet packet;
			while (peer->socket.receive(packet) == sf::Socket::Done)
			{
                std::string type;
                packet >> type;

                if (type == "Log")
                {
                    std::string message;
                    packet >> message;
                    mFile << message << std::endl;
                    std::cout << message << std::endl;
                }

                if (type == "Disconnect")
                {
                    peer->timedOut = true;
                    detectedTimeout = true;
                }

				peer->lastPacketTime = now();
				packet.clear();
			}

			if (now() >= peer->lastPacketTime + sf::seconds(3600))
			{
				peer->timedOut = true;
				detectedTimeout = true;
			}
		}
	}

	if (detectedTimeout)
		handleDisconnections();
}

void LogServer::handleConnections()
{
    if (mListener.accept(mPeers[mClients]->socket) == sf::TcpListener::Done)
	{
		mPeers[mClients]->ready = true;
		mPeers[mClients]->lastPacketTime = now();
        mClients++;
        mPeers.push_back(PeerPtr(new RemotePeer()));
	}
}

void LogServer::handleDisconnections()
{
    for (auto itr = mPeers.begin(); itr != mPeers.end(); )
	{
		if ((*itr)->timedOut)
		{
			mClients--;
            itr = mPeers.erase(itr);
            mPeers.push_back(PeerPtr(new RemotePeer()));
		}
		else
		{
			++itr;
		}
	}
}

sf::Time LogServer::now() const
{
    return mClock.getElapsedTime();
}

LogServer::RemotePeer::RemotePeer()
: ready(false)
, timedOut(false)
{
	socket.setBlocking(false);
}

} // namespace at


int main()
{
    at::LogServer server(50002);

    bool stop = false;
    while (!stop)
    {
        std::string command;
        std::getline(std::cin, command);
        if (command == "stop")
        {
            stop = true;
        }
    }

    return 0;
}
