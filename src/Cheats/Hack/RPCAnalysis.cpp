//
// Created by Administrator on 2025/2/15.
//

#include "RPCAnalysis.h"
#include "../../Utils.h"
#include "../../encoding.h"
#include "../../Plugin.h"
#include "../../CLog.h"

namespace CheatModule::Hack {

    RPCAnalysis::RPCAnalysis() : CCheatModule("RPC分析记录", "RPC Analysis") {}

    std::unordered_map<int, std::string> RPCAnalysis::rpcIdToString = {
            {23, "ClickPlayer"}, {25, "ClientJoin"}, {26, "EnterVehicle"}, {27, "EnterEditObject"},
            {31, "ScriptCash"}, {50, "ServerCommand"}, {52, "Spawn"}, {53, "Death"}, {54, "NPCJoin"},
            {62, "DialogResponse"}, {83, "ClickTextDraw"}, {96, "SCMEvent"}, {101, "Chat"}, {102, "SrvNetStats"},
            {103, "ClientCheck"}, {106, "DamageVehicle"}, {115, "GiveTakeDamage"}, {116, "EditAttachedObject"},
            {117, "EditObject"}, {118, "SetInteriorId"}, {119, "MapMarker"}, {128, "RequestClass"},
            {129, "RequestSpawn"}, {131, "PickedUpPickup"}, {132, "MenuSelect"}, {136, "VehicleDestroyed"},
            {140, "MenuQuit"}, {154, "ExitVehicle"}, {155, "UpdateScoresPingsIPs"}, {11, "SetPlayerName"},
            {12, "SetPlayerPos"}, {13, "SetPlayerPosFindZ"}, {14, "SetPlayerHealth"}, {15, "TogglePlayerControllable"},
            {16, "PlaySound"}, {17, "SetPlayerWorldBounds"}, {18, "GivePlayerMoney"}, {19, "SetPlayerFacingAngle"},
            {20, "ResetPlayerMoney"}, {21, "ResetPlayerWeapons"}, {22, "GivePlayerWeapon"}, {24, "SetVehicleParamsEx"},
            {28, "CancelEdit"}, {29, "SetPlayerTime"}, {30, "ToggleClock"}, {32, "WorldPlayerAdd"}, {33, "SetPlayerShopName"},
            {34, "SetPlayerSkillLevel"}, {35, "SetPlayerDrunkLevel"}, {36, "Create3DTextLabel"}, {37, "DisableCheckpoint"},
            {38, "SetRaceCheckpoint"}, {39, "DisableRaceCheckpoint"}, {40, "GameModeRestart"}, {41, "PlayAudioStream"},
            {42, "StopAudioStream"}, {43, "RemoveBuildingForPlayer"}, {44, "CreateObject"}, {45, "SetObjectPos"},
            {46, "SetObjectRot"}, {47, "DestroyObject"}, {55, "DeathMessage"}, {56, "SetPlayerMapIcon"},
            {57, "RemoveVehicleComponent"}, {58, "Update3DTextLabel"}, {59, "ChatBubble"}, {60, "UpdateSystemTime"},
            {61, "ShowDialog"}, {63, "DestroyPickup"}, {64, "WeaponPickupDestroy"}, {65, "LinkVehicleToInterior"},
            {66, "SetPlayerArmour"}, {67, "SetPlayerArmedWeapon"}, {68, "SetSpawnInfo"}, {69, "SetPlayerTeam"},
            {70, "PutPlayerInVehicle"}, {71, "RemovePlayerFromVehicle"}, {72, "SetPlayerColor"}, {73, "DisplayGameText"},
            {74, "ForceClassSelection"}, {75, "AttachObjectToPlayer"}, {76, "InitMenu"}, {77, "ShowMenu"}, {78, "HideMenu"},
            {79, "CreateExplosion"}, {80, "ShowPlayerNameTagForPlayer"}, {81, "AttachCameraToObject"}, {82, "InterpolateCamera"},
            {84, "SetObjectMaterial"}, {85, "GangZoneStopFlash"}, {86, "ApplyAnimation"}, {87, "ClearAnimations"},
            {88, "SetPlayerSpecialAction"}, {89, "SetPlayerFightingStyle"}, {90, "SetPlayerVelocity"},
            {91, "SetVehicleVelocity"}, {92, "SetPlayerDrunkVisuals"}, {93, "ClientMessage"}, {94, "SetWorldTime"},
            {95, "CreatePickup"}, {98, "SetVehicleTireStatus"}, {99, "MoveObject"}, {104, "EnableStuntBonusForPlayer"},
            {105, "TextDrawSetString"}, {107, "SetCheckpoint"}, {108, "GangZoneCreate"}, {112, "PlayCrimeReport"},
            {113, "SetPlayerAttachedObject"}, {120, "GangZoneDestroy"}, {121, "GangZoneFlash"}, {122, "StopObject"},
            {123, "SetNumberPlate"}, {124, "TogglePlayerSpectating"}, {126, "PlayerSpectatePlayer"},
            {127, "PlayerSpectateVehicle"}, {133, "SetPlayerWantedLevel"}, {134, "ShowTextDraw"},
            {135, "TextDrawHideForPlayer"}, {137, "ServerJoin"}, {138, "ServerQuit"}, {139, "InitGame"},
            {144, "RemovePlayerMapIcon"}, {145, "SetPlayerAmmo"}, {146, "SetPlayerGravity"}, {147, "SetVehicleHealth"},
            {148, "AttachTrailerToVehicle"}, {149, "DetachTrailerFromVehicle"}, {150, "SetPlayerDrunkHandling"},
            {151, "DestroyPickups"}, {152, "SetWeather"}, {153, "SetPlayerSkin"}, {156, "SetPlayerInterior"},
            {157, "SetPlayerCameraPos"}, {158, "SetPlayerCameraLookAt"}, {159, "SetVehiclePos"}, {160, "SetVehicleZAngle"},
            {161, "SetVehicleParamsForPlayer"}, {162, "SetCameraBehindPlayer"}, {163, "WorldPlayerRemove"},
            {164, "WorldVehicleAdd"}, {165, "WorldVehicleRemove"}, {166, "WorldPlayerDeath"}
    };



    bool RPCAnalysis::onProcess(bool fromServer, int rpc, RakNet::BitStream *bs) {
        static auto GetRPCString = [](int rpcId) {
            auto it = rpcIdToString.find(rpcId);
            if (it != rpcIdToString.end())
            {
                return it->second;
            }
            return "Unknown RPC " + std::to_string(rpcId);
        };

        if (fromServer) {
            auto rs = GetRPCString(rpc);

            static auto logger = [](const char* s) {
                CUtils::logToConsole(s);
                Plugin::getInstance().getLog()->log(s);
            };
            logger(rs.c_str());

            if (rpc == RPC_DisplayGameText) {
                UINT32 dMessageLength;
                bs->ResetReadPointer();
                bs->IgnoreBits(sizeof(UINT32)*2*8);
                bs->Read(dMessageLength);
                auto text = EncodingUtil::readAndConvertUTF8(bs, dMessageLength);
                logger(text.c_str());
            }
            if (rpc == RPC_ClientMessage) {
                UINT32 dMessageLength;
                bs->ResetReadPointer();
                bs->IgnoreBits(sizeof(UINT32)*8);
                bs->Read(dMessageLength);
                auto text = EncodingUtil::readAndConvertUTF8(bs, dMessageLength);
                logger(text.c_str());
            }
        }
        return true;
    }
}