// Created by Administrator on 2024/10/9.
//

#ifndef CBOT_H
#define CBOT_H

#include <RakClient.h>
#include <string>
#include <SOCKS5.hpp>
#include "CCheatModule.h"
#include "samp.h"

class CCheatManager;
class RakClient;

namespace CheatModule {
    namespace Bot {
        class CBotManager;
        class CBot {
            static CBotManager *manager;
            static std::unordered_set<int> botVehicles;

            std::string name;
            unsigned short playerID;
            int vehicleID; //used in bot follow
            int status;
            unsigned int reconnect_tick;
            unsigned int update_tick;

            bool proxy_require_auth;
            SOCKS5::SOCKS5 proxy;
            RakClient client;

            // Sub modules;
            std::vector<std::unique_ptr<CCheatModule>> BotModules;

            CCheatManager* pCheatManager;

            //Player info
            stOnFootData data_onfoot;
            void resetConnectionStatus();

            void setupRPC();
            void setupModules();
        public:
            CBot(std::string identifier);
            ~CBot();

            enum eBotStatus {
                DISCONNECTED,
                CONNECTING,
                WAIT_FOR_JOIN,
                CONNECTED,
                SPAWNED
            };

            std::string getName();
            unsigned short getPlayerID();
            int getStatus();
            stOnFootData* getData();
            sampapi::CVector getPosition();
            void setupProxy(std::string host, int port);
            void setupProxy(std::string host, int port, std::string username, std::string password);
            bool isUsingProxy();
            bool isConnected();
            bool connect_proxy();
            void connect();
            void disconnect();
            void process();
            void receive();

            void sendRPC(int id, RakNet::BitStream* bs);
            void sendAuthInfo(Packet *packet);
            void sendClientJoin(Packet *packet);
            void sendBullet(stBulletData* data);
            void sendAim(stAimData* data);
            void sendInCar(stInCarData* data);
            void sendEnterVehicle(int vehicleid);


            void sendClassRequest(int classid);
            void sendSpawnRequest();
            void sendChat(std::string text);
            void sendSpawn();

            void sendOnfootSync(stOnFootData *onfoot);

            void updateOnfoot();
            void on_receive_rpc(int id, RakNet::BitStream *bs);
            static void setManager(CBotManager *manager);

            void setPosition(float x, float y, float z);
            void setVehicleID(int vehicleid);
            int getVehicleID();
            void clearVehicle();
            bool isVehicleUsedByBot(int vehicleid);
            static const std::unordered_set<int>* getBotVehicleSet();
        };


    } // Bot
} // CheatModule

#endif //CBOT_H
