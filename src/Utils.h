#ifndef A9AC9AEE_1A24_473A_A842_E395224DF915
#define A9AC9AEE_1A24_473A_A842_E395224DF915

#include <string>
class CVector;
class CPed;

#include "types.h"
// main pool addresses
#define ACTOR_POOL_POINTER		0x00B74490
#define ACTOR_POINTER_SELF		0x00B7CD98
#define ACTOR_SELF				- 1
#define VEHICLE_POOL_POINTER	0x00B74494
#define VEHICLE_POINTER_SELF	0x00B6F980
#define VEHICLE_SELF			- 1
#define OBJECT_POOL_POINTER		0x00B7449C
#define BUILDING_POOL_POINTER	0x00B74498

#define FUNC_GET_CAR_ID				0x424160
#define FUNC_GET_CAR_STRUCT			0x4048E0
#define FUNC_GET_ACTOR_ID			0x4442D0
#define FUNC_GET_ACTOR_STRUCT		0x404910
#define FUNC_GET_OBJECT_ID			0x465070
#define FUNC_GET_OBJECT_STRUCT		0x465040
#define FUNC_GET_COL_MODEL_STRUCT	0x40FB80
#define FUNC_GET_BUILDING_STRUCT	0x403FA0


class CUtils {
   public:
    static void chatprintf(const char* fmt, ...);
    static void logToConsole(const char* fmt, ...);
    static void errorToConsole(const char* fmt, ...);
    static int mapStringToVK(const std::string& key);
    static void vect3_copy ( const float in[3], float out[3] );

    static uint32_t GetFromPool ( DWORD value, DWORD Pool, DWORD function );
    static int ScriptCarId ( struct vehicle_info *car );
    static int ScriptActorId ( CPed *ped );
    static int ScriptObjectId ( struct object_info *object );

    static int randomInt(int low, int high);
    static float randomFloat(float low, float high);

    static void GetMatrixFromQuaternion(float *fQuaternion, MATRIX4X4 *matrix);
    static void GetOffsetedPosition(float * result, float *input, float *offset, float ang);
    static CVector GetOffsetedPosition(const CVector& in,const CVector& offset, float ang);

};

#endif /* A9AC9AEE_1A24_473A_A842_E395224DF915 */
