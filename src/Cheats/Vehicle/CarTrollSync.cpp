#include "CarTrollSync.h"

#include <BitStream.h>
#include <sampapi/CNetGame.h>
#include <CPools.h>
#include <common.h>

#include <queue>

#include <RakHook/rakhook.hpp>
#include "../../samp.h"


namespace CheatModule::Vehicle {

    std::queue<stInCarData> packetQueue;
    int packet_tick = 0;

    void CarTrollSync::send_incar() {
    }

    CarTrollSync::CarTrollSync(int vehicleid, int mode, bool fix) {
        vehicle = vehicleid;
        localVehicle = CPools::GetVehicle(samp::RefNetGame()->GetVehiclePool()->GetRef(vehicleid));
        this->m_mode = mode;
        fixVehicle = fix;
        m_vehicleTurnSpeed = localVehicle->m_vecTurnSpeed;
        m_vehiclePos = localVehicle->GetPosition();
    }

    void CarTrollSync::setPosition(const CVector &vector) {
        m_vehiclePos = vector;
    }

    void CarTrollSync::setVelocity(const CVector &vector) {
        m_vehicleVel = vector;
    }

    void CarTrollSync::setTurnSpeed(const CVector &vector) {
        m_vehicleTurnSpeed = vector;
    }

    void CarTrollSync::execute() const {
        auto ped = FindPlayerPed(-1);
        if (ped == nullptr)
            return;

        switch (m_mode) {
            case SYNC_INCAR: {
                RakNet::BitStream bs;
                stInCarData data{};
                data.sVehicleID = static_cast<short>(vehicle);
                data.fPosition[0] = m_vehiclePos.x;
                data.fPosition[1] = m_vehiclePos.y;
                data.fPosition[2] = m_vehiclePos.z;
                data.fMoveSpeed[0] = m_vehicleVel.x;
                data.fMoveSpeed[1] = m_vehicleVel.y;
                data.fMoveSpeed[2] = m_vehicleVel.z;

                data.bytePlayerHealth = static_cast<BYTE>(ped->m_fHealth);
                data.byteArmor = static_cast<BYTE>(ped->m_fArmour);

                if (fixVehicle)
                    data.fVehicleHealth = 1000.0f;
                else data.fVehicleHealth = localVehicle->m_fHealth;

                bs.Reset();
                bs.Write(static_cast<BYTE>(ID_VEHICLE_SYNC));
                bs.Write(reinterpret_cast<PCHAR>(&data), sizeof(stInCarData));
                rakhook::send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
                break;
            }
            case SYNC_INCAR_RPC: {
                RakNet::BitStream FkbsRPC;
                UINT16 wVehicleID = vehicle; UINT8 bIsPassenger = false;
                FkbsRPC.Reset();
                FkbsRPC.Write(wVehicleID);
                FkbsRPC.Write(bIsPassenger);
                rakhook::send_rpc(RPC_EnterVehicle, &FkbsRPC, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);

                stInCarData data{};
                data.sVehicleID = static_cast<short>(vehicle);
                data.fPosition[0] = m_vehiclePos.x;
                data.fPosition[1] = m_vehiclePos.y;
                data.fPosition[2] = m_vehiclePos.z;
                data.fMoveSpeed[0] = m_vehicleVel.x;
                data.fMoveSpeed[1] = m_vehicleVel.y;
                data.fMoveSpeed[2] = m_vehicleVel.z;

                if (fixVehicle)
                    data.fVehicleHealth = 1000.0f;
                else data.fVehicleHealth = localVehicle->m_fHealth;

                if (packetQueue.empty())
                    packet_tick = GetTickCount();

                packetQueue.push(data);
                break;
            }
            case SYNC_UNOCCUPIED: {
                RakNet::BitStream bs;
                bs.Reset();
                stUnoccupiedData data{};

                data.sVehicleID = static_cast<short>(vehicle);
                data.fPosition[0] = m_vehiclePos.x;
                data.fPosition[1] = m_vehiclePos.y;
                data.fPosition[2] = m_vehiclePos.z;
                data.fMoveSpeed[0] = m_vehicleVel.x;
                data.fMoveSpeed[1] = m_vehicleVel.y;
                data.fMoveSpeed[2] = m_vehicleVel.z;
                data.fTurnSpeed[0] = m_vehicleTurnSpeed.x;
                data.fTurnSpeed[1] = m_vehicleTurnSpeed.y;
                data.fTurnSpeed[2] = m_vehicleTurnSpeed.z;

                data.byteSeatID = 1;
                data.fDirection[1] = 1.0f;
                data.fRoll[0] = 1.0f;

                if (fixVehicle)
                    data.fHealth = 1000.0f;
                else data.fHealth = localVehicle->m_fHealth;

                bs.Write(static_cast<BYTE>(ID_UNOCCUPIED_SYNC));
                bs.Write(reinterpret_cast<PCHAR>(&data), sizeof(stUnoccupiedData));
                rakhook::send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
                break;
            }
            case SYNC_TRAILER: {
                RakNet::BitStream bs;
                bs.Reset();
                stTrailerData data{};
                data.sTrailerID = static_cast<short>(vehicle);
                data.fPosition[0] = m_vehiclePos.x;
                data.fPosition[1] = m_vehiclePos.y;
                data.fPosition[2] = m_vehiclePos.z;
                data.fSpeed[0] = m_vehicleVel.x;
                data.fSpeed[1] = m_vehicleVel.y;
                data.fSpeed[2] = m_vehicleVel.z;

                bs.Write(static_cast<BYTE>(ID_TRAILER_SYNC));
                bs.Write(reinterpret_cast<PCHAR>(&data), sizeof(stTrailerData));
                rakhook::send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
            }
            case SYNC_SELF: {
                break;
            }
            case SYNC_LOCAL_TEST: {
                break;
            }
        }
    }

    void CarTrollSync::executeLocal() const {
        if (localVehicle) {
            localVehicle->m_vecMoveSpeed = m_vehicleVel;
            localVehicle->m_vecTurnSpeed = m_vehicleTurnSpeed;
            localVehicle->m_matrix->pos = m_vehiclePos;

            if (fixVehicle) {
                localVehicle->m_fHealth = 1000.0f;
                localVehicle->Fix();
            }
        }
    }

    CVehicle *CarTrollSync::getLocalVehicle() const {
        return localVehicle;
    }

    void CarTrollSync::processPacketQueue() {
        if (GetTickCount() - packet_tick >= 400) {
            if (!packetQueue.empty()) {
                RakNet::BitStream bs;

                stInCarData packet = packetQueue.front();
                packetQueue.pop();
                auto ped = FindPlayerPed(-1);
                packet.bytePlayerHealth = static_cast<BYTE>(ped->m_fHealth);
                packet.byteArmor = static_cast<BYTE>(ped->m_fArmour);

                bs.Reset();
                bs.Write(static_cast<BYTE>(ID_VEHICLE_SYNC));
                bs.Write(reinterpret_cast<PCHAR>(&packet), sizeof(stInCarData));
                rakhook::send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
            }
            packet_tick = GetTickCount();
        }
    }

}
