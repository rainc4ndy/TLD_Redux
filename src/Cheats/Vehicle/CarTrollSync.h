#ifndef B1E464B2_B66E_4504_BF2B_363BDA15DA4B
#define B1E464B2_B66E_4504_BF2B_363BDA15DA4B

#include <CVector.h>
#include <CVehicle.h>
namespace CheatModule::Vehicle {

    class CarTrollSync {
    private:
        int m_mode;
        int vehicle;
        bool fixVehicle;
        CVehicle* localVehicle;
        CVector m_vehiclePos {};
        CVector m_vehicleVel {};
        CVector m_vehicleTurnSpeed {};

        void send_incar();

    public:
        enum SYNC_MODE {
            SYNC_INCAR,
            SYNC_INCAR_RPC,
            SYNC_UNOCCUPIED,
            SYNC_TRAILER,
            SYNC_SELF,
            SYNC_LOCAL_TEST
        };
        CarTrollSync(int vehicleid, int mode, bool fix);

        void setPosition(const CVector& vector);
        void setVelocity(const CVector& vector);
        void setTurnSpeed(const CVector& vector);

        void execute() const;
        void executeLocal() const;
        static void processPacketQueue();

        CVehicle* getLocalVehicle() const;
    };


}
#endif /* B1E464B2_B66E_4504_BF2B_363BDA15DA4B */
