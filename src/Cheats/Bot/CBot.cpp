//
// Created by Administrator on 2024/10/9.
//
#include <functional>
#include <rakpeer.h>
#include <RakClientInterface.h>
#include <StringCompressor.h>

#include "CBot.h"

#include <set>

#include "authKey.h"
#include "CBotFollow.h"
#include "CBotManager.h"
#include "CCheatManager.h"
#include "CProxyManager.h"
#include "encoding.h"
#include "samp.h"
#include "Plugin.h"
#include "Utils.h"

namespace CheatModule::Bot {
    SOCKS5::SOCKS5 dummyProxy;

    CBotManager *CBot::manager = nullptr;
    std::unordered_set<int> CBot::botVehicles;

    void CBot::resetConnectionStatus() {
        status = DISCONNECTED;
        reconnect_tick = GetTickCount();
    }

    CBot::CBot(std::string identifier) {
        //client = dynamic_cast<RakClient *>(RakNetworkFactory::GetRakClientInterface());
        client.SetMTUSize(576);
        name = identifier;
        status = DISCONNECTED;
        playerID = -1;
        proxy_require_auth = false;
        update_tick = 0;
        reconnect_tick = 0;
        memset(data_onfoot.fPosition, 0, sizeof(data_onfoot.fPosition));
        data_onfoot.byteHealth = 100;
        data_onfoot.byteArmor = 0;

        vehicleID = -1;

        setupRPC();
        setupModules();

        pCheatManager = Plugin::getInstance().getCheat();
    }

    CBot::~CBot() {
        //RakNetworkFactory::DestroyRakClientInterface(client);
        clearVehicle();
        if (isUsingProxy() && proxy.IsStarted()) {
            proxy.Shutdown();
            Plugin::getInstance().getProxyManager()->reduceProxyRef(proxy.GetProxyIP() + ':' + proxy.GetProxyPort());
        }
    }

    std::string CBot::getName() {
        return name;
    }

    unsigned short CBot::getPlayerID() {
        return playerID;
    }

    int CBot::getStatus() {
        return status;
    }

    stOnFootData* CBot::getData() {
        return &data_onfoot;
    }

    sampapi::CVector CBot::getPosition() {
        return {
                data_onfoot.fPosition[0],
                data_onfoot.fPosition[1],
                data_onfoot.fPosition[2]
                };
    }

    void CBot::setupProxy(std::string host, int port) {
        proxy = SOCKS5::SOCKS5(host, std::to_string(port));
        proxy.SetReceivingByProxy(true);
        proxy_require_auth = false;
    }

    void CBot::setupProxy(std::string host, int port, std::string username, std::string password) {
        proxy = SOCKS5::SOCKS5(host, std::to_string(port), username, password);
        proxy.SetReceivingByProxy(true);
        proxy_require_auth = true;
    }

    void CBot::setupRPC() {
        client.AddRPCHandler(std::bind(&CBot::on_receive_rpc, this, _1, _2));
    }

    void CBot::setupModules() {
        auto captcha= std::make_unique<Hack::CaptchaSolver>(this);
        captcha->setEnabled(true);
        BotModules.emplace_back(std::move(captcha));
    }

    bool CBot::isUsingProxy() {
        return proxy.IsReceivingByProxy();
    }

    bool CBot::isConnected() {
        return status != DISCONNECTED && status != CONNECTING;
    }

    std::string getProxyErrorString(SOCKS5::SOCKS5Err error) {
        using namespace SOCKS5;
        switch ((error)) {
            case SOCKS5Err::SOCKS5_NONE:
                return "No error.";
            case SOCKS5Err::SOCKS5_FAILED_TO_CREATE_SOCKET:
                return "Failed to create socket.";
            case SOCKS5Err::SOCKS5_FAILED_TO_CONNECT_TO_SERVER:
                return "Failed to connect to server.";
            case SOCKS5Err::SOCKS5_AUTHENTICATION_ERROR:
                return "Authentication error.";
            case SOCKS5Err::SOCKS5_INVALID_VERSION_OR_METHOD:
                return "Invalid version or method.";
            case SOCKS5Err::SOCKS5_AUTHORIZATION_ERROR:
                return "Authorization error.";
            case SOCKS5Err::SOCKS5_INVALID_VERSION_OR_STATUS:
                return "Invalid version or status.";
            case SOCKS5Err::SOCKS5_CONNECTION_ERROR:
                return "Connection error.";
            case SOCKS5Err::SOCKS5_INVALID_VERSION_OR_RESULT:
                return "Invalid version or result.";
            case SOCKS5Err::SOCKS5_UNKNOWN_ERROR:
                return "Unknown error.";
            case SOCKS5Err::SOCKS5_INITIALIZED_SUCCESSFULLY:
                return "Initialized successfully.";
            default:
                return "Invalid error code.";
        }
    }

    bool CBot::connect_proxy() {
        if (proxy_require_auth) {
            auto r = proxy.StartWithAuth();
            if (!r.first) {
                CUtils::logToConsole("Proxy failed for bot %s, ERR: %s", name.c_str(),
                                     getProxyErrorString(r.second).c_str());
                resetConnectionStatus();
                return false;
            }
        } else {
            auto r = proxy.StartWithoutAuth();
            if (!r.first) {
                CUtils::logToConsole("Proxy failed for bot %s, ERR: %s", name.c_str(),
                                     getProxyErrorString(r.second).c_str());
                resetConnectionStatus();
                return false;
            }
        }
        return true;
    }

    void CBot::connect() {
        auto addr = samp::RefNetGame()->m_szHostAddress;
        auto port = samp::RefNetGame()->m_nPort;
        if (!proxy.IsStarted() && isUsingProxy()) {
            if (!connect_proxy()) {
                return;
            }
        }

        client.Connect(addr, port, 0, 0, 5, &proxy);
        status = CONNECTING;
    }


    void CBot::disconnect() {
        client.Disconnect(true);
        resetConnectionStatus();
        /*if (isUsingProxy()) {
            proxy.Shutdown();
        }*/
    }

    void CBot::process() {
        if (status == DISCONNECTED) {
            if (manager->auto_recon) {
                if (GetTickCount() - reconnect_tick >= manager->recon_delay) {
                    if (!proxy.IsStarted() && isUsingProxy()) {
                        if (connect_proxy()) {
                            return;
                        }
                    }
                    connect();
                }
            }
        }
        if (status == SPAWNED) {
            if (manager->use_weapon) {
                data_onfoot.byteCurrentWeapon = manager->bot_weapon;
            }
            if (!pCheatManager->botFollow.isEnabled()) {
                if (GetTickCount() - update_tick >= 500) {
                    updateOnfoot();
                    update_tick = GetTickCount();
                }
            }
        }
        receive();
    }

    void CBot::receive() {
        unsigned char packetIdentifier;
        Packet *pkt;
        while (pkt = client.Receive()) {
            if ((unsigned char) pkt->data[0] == ID_TIMESTAMP) {
                if (pkt->length > sizeof(unsigned char) + sizeof(unsigned int))
                    packetIdentifier = (unsigned char) pkt->data[sizeof(unsigned char) + sizeof(unsigned int)];
                else
                    return;
            } else
                packetIdentifier = (unsigned char) pkt->data[0];

            switch (packetIdentifier) {
                case ID_DISCONNECTION_NOTIFICATION:
                    CUtils::errorToConsole("Kick received from client %s", name.c_str());
                    resetConnectionStatus();
                    clearVehicle();
                    break;
                case ID_CONNECTION_BANNED:
                    CUtils::errorToConsole("Banned received from client %s", name.c_str());
                    resetConnectionStatus();
                    clearVehicle();
                    break;
                case ID_CONNECTION_ATTEMPT_FAILED:
                    CUtils::errorToConsole("Connection attempt failed for bot %s", name.c_str());
                    resetConnectionStatus();
                    break;
                case ID_NO_FREE_INCOMING_CONNECTIONS:
                    CUtils::errorToConsole("Server is full, bot %s cannot join", name.c_str());
                    resetConnectionStatus();
                    break;
                case ID_INVALID_PASSWORD:
                    CUtils::errorToConsole("Invalid server password");
                    resetConnectionStatus();
                    break;
                case ID_CONNECTION_LOST:
                    CUtils::errorToConsole("Bot %s has lost connection", name.c_str());
                    resetConnectionStatus();
                    clearVehicle();
                    break;
                case ID_CONNECTION_REQUEST_ACCEPTED:
                    sendClientJoin(pkt);
                    status = CONNECTED;
                    break;
                case ID_AUTH_KEY:
                    sendAuthInfo(pkt);
                    status = WAIT_FOR_JOIN;
                    break;
                case ID_PLAYER_SYNC:
                    break;
                case ID_VEHICLE_SYNC:
                    break;
                case ID_PASSENGER_SYNC:
                    break;
                case ID_AIM_SYNC:
                    break;
                case ID_TRAILER_SYNC:
                    break;
                case ID_UNOCCUPIED_SYNC:
                    break;
                case ID_MARKERS_SYNC:
                    break;
                case ID_BULLET_SYNC:
                    break;
            }
            client.DeallocatePacket(pkt);
        }
    }

    void CBot::sendRPC(int id, RakNet::BitStream *bs) {
        client.RPC(&id, bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
    }

    void CBot::sendAuthInfo(Packet *packet) {
        char *auth_key;
        bool found_key = false;

        for (int x = 0; x < 512; x++) {
            if (!strcmp(((char *) packet->data + 2), AuthKeyTable[x][0])) {
                auth_key = AuthKeyTable[x][1];
                found_key = true;
            }
        }

        if (found_key) {
            RakNet::BitStream bsKey;
            BYTE byteAuthKeyLen;

            byteAuthKeyLen = (BYTE) strlen(auth_key);

            bsKey.Write((BYTE) ID_AUTH_KEY);
            bsKey.Write((BYTE) byteAuthKeyLen);
            bsKey.Write(auth_key, byteAuthKeyLen);
            client.Send(&bsKey, SYSTEM_PRIORITY, RELIABLE, NULL);
        } else {
            CUtils::logToConsole("Unknown AUTH_IN! (%s)", ((char *) packet->data + 2));
        }
    }

    void CBot::sendClientJoin(Packet *packet) {
        RakNet::BitStream bsSuccAuth((unsigned char *) packet->data, packet->length, false);
        decltype(playerID) myPlayerID;
        unsigned int uiChallenge;

        bsSuccAuth.IgnoreBits(8); // ID_CONNECTION_REQUEST_ACCEPTED
        bsSuccAuth.IgnoreBits(32); // binaryAddress
        bsSuccAuth.IgnoreBits(16); // port

        bsSuccAuth.Read(myPlayerID);

        playerID = myPlayerID;
        bsSuccAuth.Read(uiChallenge);

        int iVersion = 4057;
        unsigned int uiClientChallengeResponse = uiChallenge ^ iVersion;
        BYTE byteMod = 1;

        char auth_bs[4 * 16];
        auto my_version = std::string("0.3.7-R5");
        if (manager->fake_mobile) {
            strcpy(auth_bs, FakeMobileKey);
            my_version = std::string("Rn_v8.1.8.0");
        } else {
            gen_gpci(auth_bs, 0x3e9);
        }

        BYTE byteAuthBSLen;
        byteAuthBSLen = (BYTE) strlen(auth_bs);
        BYTE byteNameLen = (BYTE) name.size();
        BYTE iClientVerLen = (BYTE) my_version.size();

        RakNet::BitStream bsSend;
        bsSend.Write(iVersion);
        bsSend.Write(byteMod);
        bsSend.Write(byteNameLen);
        bsSend.Write(name.c_str(), byteNameLen);
        bsSend.Write(uiClientChallengeResponse);
        bsSend.Write(byteAuthBSLen);
        bsSend.Write(auth_bs, byteAuthBSLen);
        bsSend.Write(iClientVerLen);
        bsSend.Write(my_version.c_str(), iClientVerLen);
        sendRPC(RPC_ClientJoin, &bsSend);
    }

    void CBot::sendBullet(stBulletData* data) {
        RakNet::BitStream bs;
        bs.Reset();
        bs.Write((BYTE)ID_BULLET_SYNC);
        bs.Write((PCHAR)data, sizeof(stBulletData));
        client.Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
    }

    void CBot::sendAim(stAimData* data) {
        RakNet::BitStream bs;
        bs.Reset();
        bs.Write((BYTE)ID_AIM_SYNC);
        bs.Write((PCHAR)data, sizeof(stAimData));
        client.Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
    }

    void CBot::sendInCar(stInCarData* data) {
        RakNet::BitStream bs;
        bs.Reset();
        bs.Write((BYTE)ID_VEHICLE_SYNC);
        bs.Write((PCHAR)data, sizeof(stInCarData));
        client.Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
    }

    void CBot::sendEnterVehicle(int vehicleid) {
        RakNet::BitStream FkbsRPC;
        UINT16 wVehicleID = vehicleid; UINT8 bIsPassenger = false;
        FkbsRPC.Reset();
        FkbsRPC.Write(vehicleid);
        FkbsRPC.Write(bIsPassenger);
        int r = RPC_EnterVehicle;
        client.RPC(&r, &FkbsRPC, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
    }

    void CBot::sendClassRequest(int classid) {
        RakNet::BitStream bsSpawnRequest;
        bsSpawnRequest.Reset();
        bsSpawnRequest.Write(classid);
        int r = RPC_RequestClass;
        client.RPC(&r, &bsSpawnRequest, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
    }

    void CBot::sendSpawnRequest() {
        if (manager->fake_spawn)
            return;
        RakNet::BitStream bsSendRequestSpawn;
        bsSendRequestSpawn.Reset();
        int r = RPC_RequestSpawn;
        client.RPC(&r, &bsSendRequestSpawn, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
    }


    void CBot::sendChat(std::string text) {
        if (text.size() == 0)
            return;
        RakNet::BitStream bs;
        bs.Reset();

        if (text[0] == '/') {
            //send server command
            bs.Write((UINT32)text.size());
            bs.Write(text.data(), text.size());
            int r = RPC_ServerCommand;
            client.RPC(&r, &bs, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
        } else {
            bs.Write((UINT8)text.size());
            bs.Write(text.data(), text.size());
            int r = RPC_Chat;
            client.RPC(&r, &bs, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
        }
    }

    void CBot::sendSpawn() {
        if (manager->fake_spawn) {
            status = SPAWNED;
            return;
        }

        RakNet::BitStream bsSendSpawn;
        bsSendSpawn.Reset();
        int r = RPC_Spawn;
        client.RPC(&r, &bsSendSpawn, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
        status = SPAWNED;
    }

    void CBot::sendOnfootSync(stOnFootData *onfoot) {
        RakNet::BitStream bs;
        bs.Reset();
        bs.Write((BYTE)ID_PLAYER_SYNC);
        bs.Write((PCHAR)onfoot, sizeof(stOnFootData));
        client.Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
    }

    void CBot::updateOnfoot() {
        sendOnfootSync(&data_onfoot);
    }

    void CBot::setManager(CBotManager *manager) {
        CBot::manager = manager;
    }

    void CBot::setPosition(float x, float y, float z) {
        data_onfoot.fPosition[0] = x;
        data_onfoot.fPosition[1] = y;
        data_onfoot.fPosition[2] = z;
    }

    void CBot::setVehicleID(int vehicleid) {
        if (vehicleid == -1) {
            clearVehicle();
        } else {
            vehicleID = vehicleid;
            botVehicles.insert(vehicleid);
        }
    }

    int CBot::getVehicleID() {
        return vehicleID;
    }

    void CBot::clearVehicle() {
        if (vehicleID != -1) {
            botVehicles.erase(vehicleID);
            vehicleID = -1;
        }
    }

    bool CBot::isVehicleUsedByBot(int vehicleid) {
        return botVehicles.find(vehicleID) != botVehicles.end();
    }

    const std::unordered_set<int>* CBot::getBotVehicleSet() {
        return &botVehicles;
    }

    void CBot::on_receive_rpc(int id, RakNet::BitStream *bs) {
        bool ret = true;
        for (auto& it : BotModules) {
            if (it->isEnabled())
                if (!(ret = it->onProcess(true, (int)id, bs))) break;
        }
        if (!ret) return;

        switch (id) {
            case RPC_InitGame: {
                CUtils::logToConsole("Bot %s has connected to the server", name.c_str());

                if (manager->auto_disconnect) {
                    disconnect();
                    name = manager->gen_name(manager->name_strategy, manager->name_len, manager->suffix_len);
                    break;
                }
                sendClassRequest(1);
                break;
            }
            case RPC_SetPlayerHealth: {
                if (!manager->invulnerable)
                    bs->Read(data_onfoot.byteHealth);
                break;
            }
            case RPC_SetPlayerArmour: {
                if (!manager->invulnerable)
                    bs->Read(data_onfoot.byteArmor);
                break;
            }
            case RPC_SetPlayerPos: {
                if (!manager->ignore_location_rpc && !pCheatManager->botFollow.isEnabled()) {
                    bs->Read(data_onfoot.fPosition[0]);
                    bs->Read(data_onfoot.fPosition[1]);
                    bs->Read(data_onfoot.fPosition[2]);
                }
                break;
            }
            case RPC_SetSpawnInfo: {
                bs->IgnoreBits((2 * sizeof(UINT8) + sizeof(UINT32)) * 8);
                bs->Read(data_onfoot.fPosition[0]);
                bs->Read(data_onfoot.fPosition[1]);
                bs->Read(data_onfoot.fPosition[2]);
                break;
            }
            case RPC_RequestClass: {
                UINT8 bRequestResponse;
                UINT8 byteTeam;
                UINT32 dSkin;
                UINT8 unused;
                bs->Read(bRequestResponse);
                if (bRequestResponse) {
                    bs->Read(byteTeam);
                    bs->Read(dSkin);
                    bs->Read(unused);
                    bs->Read(data_onfoot.fPosition[0]);
                    bs->Read(data_onfoot.fPosition[1]);
                    bs->Read(data_onfoot.fPosition[2]);
                }
                break;
            }
            case RPC_ShowDialog: {
                if (!manager->auto_login)
                    break;
                bs->ResetReadPointer();
                INT16 wDialogID;
                UINT8 bDialogStyle, bTitleLength, bButton1Len, bButton2Len;
                bs->Read(wDialogID);
                bs->Read(bDialogStyle);
                bs->Read(bTitleLength);
                auto title_converted = EncodingUtil::readAndConvertUTF8(bs, bTitleLength);
                bs->Read(bButton1Len);
                bs->IgnoreBits(bButton1Len * 8);
                bs->Read(bButton2Len);
                bs->IgnoreBits(bButton2Len * 8);

//                char szInfo[257];
//                stringCompressor->DecodeString(szInfo, 256, bs);
//                auto content_converted = std::unique_ptr<char[]>(EncodingUtil::g2u(szInfo));
//                CUtils::logToConsole("Dialog ID: %s", title_converted.c_str());
//                CUtils::logToConsole("content_converted ID: %s", content_converted.get());
                static std::vector<std::string> titles = {
                    "登录","注册","新手资金","Login","Register","login","register","推荐人"
                };
                if (bDialogStyle == DIALOG_STYLE_INPUT || bDialogStyle == DIALOG_STYLE_PASSWORD) {
                    for (auto& it : titles) {
                        if (title_converted.find(it) != std::string::npos) {
                            RakNet::BitStream response;
                            response.Reset();
                            response.Write(wDialogID);
                            response.Write(static_cast<UINT8>(1));
                            response.Write(static_cast<INT16>(-1));
                            response.Write(static_cast<UINT8>(manager->bot_password.size()));
                            response.Write(manager->bot_password.data(), manager->bot_password.size());
                            sendRPC(RPC_DialogResponse, &response);
                            CUtils::logToConsole("Bot %s completed login challenge", name.c_str());
                            break;
                        }
                    }
                }
                break;
            }
            case RPC_ClientMessage: {
                if (!manager->auto_login)
                    break;
                UINT32 dMessageLength;
                bs->ResetReadPointer();
                bs->IgnoreBits(sizeof(UINT32)*8);
                bs->Read(dMessageLength);
                auto text = EncodingUtil::readAndConvertUTF8(bs, dMessageLength);
                if (text.find("/reg") != std::string::npos) {
                    sendChat("/reg " + manager->bot_password);
                    CUtils::logToConsole("Bot %s registered with command", name.c_str());
                }
                if (text.find("/Login") != std::string::npos) {
                    sendChat("/Login " + manager->bot_password);
                    CUtils::logToConsole("Bot %s login with command", name.c_str());
                }
                break;
            }
        }
    }
} // CheatModule::Bot
