#include "pch.h"

#include "net/packets/GameCommandPacket.h"

#include <cstddef>  // std::size_t
#include <cstdint>
#include <string>

#include "commands/GameCommandFactory.h"
#include "utils/BufferUtils.h"
#include "GameCommand.h"

namespace Rival {

GameCommandPacket::GameCommandPacket(std::vector<std::shared_ptr<GameCommand>> commands, int tick)
    : Packet(PacketType::GameCommand)
    , commands(commands)
    , tick(tick)
{
}

void GameCommandPacket::serialize(std::vector<char>& buffer) const
{
    Packet::serialize(buffer);

    BufferUtils::addToBuffer(buffer, static_cast<std::uint8_t>(commands.size()));
    BufferUtils::addToBuffer(buffer, tick);

    for (auto& command : commands)
    {
        command->serialize(buffer);
    }
}

std::shared_ptr<GameCommandPacket>
GameCommandPacket::deserialize(const std::vector<char> buffer, const GameCommandFactory& commandFactory)
{
    std::size_t offset = relayedPacketHeaderSize;

    std::uint8_t numCommands = 0;
    BufferUtils::readFromBuffer(buffer, offset, numCommands);

    int tick = 0;
    BufferUtils::readFromBuffer(buffer, offset, tick);

    std::vector<std::shared_ptr<GameCommand>> commands;
    for (std::uint8_t i = 0; i < numCommands; ++i)
    {
        auto command = commandFactory.deserialize(buffer, offset);
        commands.push_back(command);
    }

    return std::make_shared<GameCommandPacket>(commands, tick);
}

}  // namespace Rival
