#include "samp.h"

constexpr int GetCurrentNetgameVersion() {
#ifdef DL_BUILD
    return 4062;
#else
    return 4057;
#endif
}
std::string GetCurrentVersionString() {
#ifdef DL_BUILD
    return "0.3.DL";
#else
    return "0.3.7-R1";
#endif
}