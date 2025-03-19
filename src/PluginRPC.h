#pragma once

#include <RakHook/rakhook.hpp>

class PluginRPC {
public:
    bool onServerMessage(unsigned char& id, RakNet::BitStream* bs);
    bool onClientMessage(int &id, RakNet::BitStream *bs, PacketPriority &priority, PacketReliability &reliability, char &ord_channel, bool &sh_timestamp);
    bool onIncomingPacket(Packet *p);
    bool onOutcomingPacket(RakNet::BitStream *bs, PacketPriority &priority, PacketReliability &reliability, char &ord_channel);
};