#include "PluginRPC.h"

#include <BitStream.h>
#include <string>

#include "CarUtils.h"
#include "CCheatManager.h"
#include "CLog.h"
#include "Plugin.h"
#include "Utils.h"
#include "encoding.h"
#include "samp.h"

template <typename T>
std::string readWithSize(RakNet::BitStream& bs) {
    T size;
    if (!bs.Read(size) || size <= 0) return {};
    std::string str(size, '\0');
    bs.Read(str.data(), size);
    return str;
}

template <typename T>
void writeWithSize(RakNet::BitStream& bs, std::string_view str) {
    T size = static_cast<T>(str.size());
    bs.Write(size);
    bs.Write(str.data(), size);
}

bool PluginRPC::onServerMessage(unsigned char& id, RakNet::BitStream* bs) {
    auto manager = Plugin::getInstance().getCheat();
    auto r = manager->getAll();
    
    if (id == RPC_ShowDialog) {
        unsigned short index = -1;
        bs->Read(index);
        manager->dialogSpam.last_dialog = index;
    }
    if (id == RPC_DisplayGameText) {
        UINT32 dMessageLength;
        bs->ResetReadPointer();
        bs->IgnoreBits(sizeof(UINT32)*2*8);
        bs->Read(dMessageLength);
        auto text = EncodingUtil::readAndConvertUTF8(bs, dMessageLength);
        int j = 0;
        while (j < text.size()) {
            if (text[j] == '~') {
                if (j + 2 < text.size() && text[j+2] == '~') {
                    //ok
                    j += 2;
                } else {
                    CUtils::chatprintf("[x] Server is sending corrupted game text: %s", text.c_str());
                    return false;
                }
            }
            j++;
        }
    }

    bool ret = true;
    for (auto& it : r) {
        if (it->isEnabled())
            if (!(ret = it->onProcess(true, (int)id, bs))) break;
    }
    if (!ret) return false;
    return true;
}

bool PluginRPC::onClientMessage(int& id, RakNet::BitStream* bs,
                                PacketPriority& priority,
                                PacketReliability& reliability,
                                char& ord_channel, bool& sh_timestamp) {
    auto manager = Plugin::getInstance().getCheat();
    auto r = manager->getAll();
    
    bool ret = true;
    for (auto& it : r) {
        if (it->isEnabled())
            if (!(ret = it->onProcess(false, (int)id, bs))) break;
    }
    if (!ret) return false;
    return true;
}

bool PluginRPC::onIncomingPacket(Packet* p) {
    if (p->data == nullptr || p->length == 0) return true;

    auto manager = Plugin::getInstance().getCheat();
    auto r = manager->getAll();

    if (p->data[0] == ID_PLAYER_SYNC)  // packetId
    {
        RakNet::BitStream bs(p->data, p->length, false);
        UINT8 playerID = static_cast<UINT8>(-1);
        //bs.Read(playerID);

        stOnFootData pck;
        bs.Read((char*)&pck, sizeof(stOnFootData));

        for (auto& it : r) {
            if (it->isEnabled())
                it->onProcess(playerID, &pck);
        }
    } else if (p->data[0] == ID_BULLET_SYNC)  // packetId
    {
        RakNet::BitStream bs(p->data, p->length, false);
        bs.IgnoreBits(8);
        UINT8 playerID = static_cast<UINT8>(-1);
        bs.Read(playerID);

        stBulletData pck;
        bs.Read((char*)&pck, sizeof(stBulletData));

        for (auto& it : r) {
            if (it->isEnabled())
                it->onProcess(playerID, &pck);
        }
    } else if (p->data[0] == ID_AIM_SYNC)  // packetId
    {
        RakNet::BitStream bs(p->data, p->length, false);
        bs.IgnoreBits(8);
        UINT8 playerID = static_cast<UINT8>(-1);
        bs.Read(playerID);

        stAimData pck;
        bs.Read((char*)&pck, sizeof(stAimData));

        for (auto& it : r) {
            if (it->isEnabled())
                it->onProcess(playerID, &pck);
        }
    }
    return true;
}

bool PluginRPC::onOutcomingPacket(RakNet::BitStream* bs,
                                  PacketPriority& priority,
                                  PacketReliability& reliability,
                                  char& ord_channel) {
    uint8_t packetId;
    bs->Read(packetId);

    auto manager = Plugin::getInstance().getCheat();
    auto r = manager->getAll();

    switch (packetId) {
        case ID_PLAYER_SYNC: {
            stOnFootData pck;
            bs->Read((char*)&pck, sizeof(stOnFootData));

            bool ret = true;
            for (auto& it : r) {
                if (it->isEnabled())
                    if (!(ret = it->onProcess(&pck))) break;
            }
            if (!ret) return false;

            bs->Reset();
            bs->Write((BYTE)ID_PLAYER_SYNC);
            bs->Write((PCHAR)&pck, sizeof(stOnFootData));
            break;
        }
        case ID_AIM_SYNC: {
            stAimData pck;
            bs->Read((char*)&pck, sizeof(stAimData));

            bool ret = true;
            for (auto& it : r) {
                if (it->isEnabled())
                    if (!(ret = it->onProcess(&pck))) break;
            }
            if (!ret) return false;

            bs->Reset();
            bs->Write((BYTE)ID_AIM_SYNC);
            bs->Write((PCHAR)&pck, sizeof(stAimData));
            break;
        }
        case ID_VEHICLE_SYNC: {
            stInCarData pck;
            bs->Read((char*)&pck, sizeof(stInCarData));
            bool ret = true;
            for (auto& it : r) {
                if (it->isEnabled())
                    if (!(ret = it->onProcess(&pck))) break;
            }
            if (!ret) return false;

            if (!CarUtils::areWeSafeToEnter())
                return false;

            bs->Reset();
            bs->Write((BYTE)ID_VEHICLE_SYNC);
            bs->Write((PCHAR)&pck, sizeof(stInCarData));
            break;
        }
        case ID_BULLET_SYNC: {
            stBulletData pck;
            bs->Read((char*)&pck, sizeof(stBulletData));
            bool ret = true;
            for (auto& it : r) {
                if (it->isEnabled())
                    if (!(ret = it->onProcess(&pck))) break;
            }
            if (!ret) return false;

            bs->Reset();
            bs->Write((BYTE)ID_BULLET_SYNC);
            bs->Write((PCHAR)&pck, sizeof(stBulletData));
            break;
        }
    }

    return true;
}
