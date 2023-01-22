#include "pch.h"

#include "net/Connection.h"

#include "net/packets/AnonymousPacket.h"

namespace Rival {

Connection::Connection(Socket socket, std::shared_ptr<PacketFactory> packetFactory)
    : socket(std::move(socket))
    , packetFactory(packetFactory)
    , receiveThread(&Connection::receiveThreadLoop, this)
{
    sendBuffer.reserve(bufferSize);
    recvBuffer.reserve(bufferSize);
}

Connection::~Connection()
{
    close();
    receiveThread.join();
}

void Connection::close() noexcept
{
    socket.close();
}

void Connection::receiveThreadLoop()
{
    while (!socket.isClosed())
    {
        socket.receive(recvBuffer);

        // If this Connection has no packet factory, just wrap the buffers in generic packets. These are used by the
        // relay server, which doesn't care about their contents.
        std::shared_ptr<const Packet> packet =
                packetFactory ? packetFactory->deserialize(recvBuffer) : std::make_shared<AnonymousPacket>(recvBuffer);

        {
            std::scoped_lock lock(receivedPacketsMutex);
            receivedPackets.push_back(packet);
        }

        recvBuffer.clear();
    }
}

void Connection::send(const Packet& packet)
{
    packet.serialize(sendBuffer);
    socket.send(sendBuffer);
    sendBuffer.clear();
}

std::vector<std::shared_ptr<const Packet>> Connection::getReceivedPackets()
{
    std::vector<std::shared_ptr<const Packet>> packetsToReturn;

    {
        std::scoped_lock lock(receivedPacketsMutex);
        packetsToReturn = receivedPackets;
        receivedPackets.clear();
    }

    return packetsToReturn;
}

}  // namespace Rival
