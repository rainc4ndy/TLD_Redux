#ifndef BDD2ACFC_EF06_4022_A5F1_5443DB10D0F3
#define BDD2ACFC_EF06_4022_A5F1_5443DB10D0F3

#include <Windows.h>

class CLog;
class CMenu;
class CConfig;
class CTexture;
class CTextureManager;
class CCheatModule;
class CCheatManager;
class HueCycler;
class CRenderer;
class CTargetSelector;
class CProxyManager;
class CLicenseChecker;
namespace CheatModule::Bot {
	class CBotManager;
	class CBot;
}

namespace httplib {
	class Result;
}

// Start a new packing alignment
#pragma pack(push, 1)

typedef struct _VECTOR {
	float X, Y, Z;
} VECTOR, *PVECTOR;

typedef struct _MATRIX4X4 {
	VECTOR right;
	DWORD flags;
	VECTOR up;
	float pad_u;
	VECTOR at; // front
	float pad_a;
	VECTOR pos;
	float pad_p;
} MATRIX4X4, *PMATRIX4X4;

// Restore the previous packing alignment
#pragma pack(pop)

#ifndef M_PI
#define M_PI	3.14159265358979323844f
#endif
#define DEGTORAD( Deg ) ( (Deg * M_PI) / 180.0f )

#endif /* BDD2ACFC_EF06_4022_A5F1_5443DB10D0F3 */
