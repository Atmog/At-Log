#ifndef AT_LOGSERVER_HPP
#define AT_LOGSERVER_HPP

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>

namespace at
{

class LogServer
{
    public:
        LogServer(unsigned short);
        ~LogServer();

        void run();

        void handlePackets();
        void handleConnections();
        void handleDisconnections();

        sf::Time now() const;

    private:
		struct RemotePeer
		{
			RemotePeer();

			sf::TcpSocket			socket;
			sf::Time				lastPacketTime;
			bool					ready;
			bool					timedOut;
		};
		typedef std::unique_ptr<RemotePeer> PeerPtr;

    private:
        sf::Thread mThread;
        bool mWaitingThreadEnd;

        sf::TcpListener mListener;

        sf::Clock mClock;

        std::ofstream mFile;

        std::vector<PeerPtr> mPeers;
        std::size_t mClients;
};

} // namespace at


int main();

#endif // AT_LOGSERVER_HPP
