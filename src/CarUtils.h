#ifndef A44DB8EE_8400_4212_BB32_604A20A3BA05
#define A44DB8EE_8400_4212_BB32_604A20A3BA05

#include <sampapi/CVehicle.h>
#include "samp.h"
#include <CVector.h>
#include <set>
#include <unordered_set>

class CarUtils {
private:
    static int last_hacking_vehicle;
    static unsigned int enter_tick;
public:
    static int getHackingVehicle(float distance);
    static int getVehicleFromVector3D(const CVector& vec, float distance, const std::unordered_set<int>* filter = nullptr);

    static CVehicle* getGTAVehicleFromSAMP(int id);
    static void safelyBackInCar();
    static bool areWeSafeToEnter();
};

#endif /* A44DB8EE_8400_4212_BB32_604A20A3BA05 */
